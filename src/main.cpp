#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <unistd.h>
#include "World.h"
#include "Plane.h"
#include "Checkers.h"
#include "Mandelbulb.h"
#include "PointSource.h"
#include "Camera.h"
#include "TerminalDisplay.h"

// A gold Mandelbulb, pole pointing up, over a reflective checkerboard.
static std::unique_ptr<World> build_scene(){
    auto w = std::make_unique<World>();
    Material floor_mat(0.1, 0.8, 0.2, 100.0, WHITE, 0.25, 0.0, 1.0,
                       std::make_shared<Checkers>(Color({0.15,0.15,0.2}), Color({0.85,0.85,0.9}), MatScaling(0.5,0.5,0.5)));
    w->add_shape(std::make_unique<Plane>(MatIdentity(4), floor_mat));

    Material gold(0.15, 0.75, 0.6, 60.0, Color({1.0, 0.72, 0.25}));
    gold.set_reflectance(0.15);
    // The bulb's symmetry axis is object-space z; rotate it to point up, then lift it off the floor.
    w->add_shape(std::make_unique<Mandelbulb>(MatTranslation(0, 1.55, 0) * MatScaling(1.5, 1.5, 1.5) * MatRotateX(-pi/2), gold));

    w->add_source(std::make_unique<PointSource>(WHITE, Tuple({-6, 9, -7}, TupType::POINT)));
    return w;
}

static Camera make_camera(int width, int height){
    return Camera(width, height, pi/3.0, Tuple({0, 2.6, -5.2}, TupType::POINT), Tuple({0, 1.3, 0}, TupType::POINT), Tuple({0, 1, 0}));
}

// Terminal-sized images are small, so 16x16 tiles leave too few tiles to keep every thread busy
// (an 80x24 terminal gives only 15). Measured median frame times, Mandelbulb scene, 20 threads:
//   tile 16 / 8 / 4 / 2:  80x24: 78 / 46 / 30 / 26 ms   100x30: 74 / 57 / 43 / 46 ms   200x60: 212 / 171 / 161 / 165 ms
constexpr int TERMINAL_TILE_SIZE = 4;

// Shows the scene in the terminal, re-rendering whenever the terminal is resized.
static int run_in_terminal(World& world){
    TerminalDisplay display;
    TerminalDisplay::Size last_size{0, 0};
    while(true){
        // Wait briefly for keys so an idle display doesn't spin the CPU; resizes are picked up
        // within that interval.
        for(char key : display.read_keys(100)){
            if(key == 'q' || key == 'Q' || key == 3 /* Ctrl-C */){return 0;}
        }
        const TerminalDisplay::Size size = display.size();
        if(size == last_size){continue;}   // static scene: nothing changed, nothing to redraw
        last_size = size;

        // One row is kept for the status line; each remaining row shows two pixels.
        const int width = size.cols;
        const int height = 2 * (size.rows - 1);
        if(width < 1 || height < 2){continue;}

        const auto start = std::chrono::steady_clock::now();
        auto image = make_camera(width, height).render(&world, 20, 0, TERMINAL_TILE_SIZE, false);
        const double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();

        char status[160];
        std::snprintf(status, sizeof(status), " %dx%d px | %.0f ms | q to quit", width, height, ms);
        display.draw(*image, status);
    }
}

int main(){
    auto world = build_scene();
    if(!::isatty(STDIN_FILENO) || !::isatty(STDOUT_FILENO)){
        // Not running in a terminal (e.g. output redirected): render one image to a file instead.
        make_camera(400, 300).render(world.get())->save_ppm("Mandelbulb");
        return 0;
    }
    try{
        return run_in_terminal(*world);
    }
    catch(const std::exception& e){
        // The TerminalDisplay has been destroyed by now, so the terminal is back to normal.
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
