#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <unistd.h>
#include "World.h"
#include "Plane.h"
#include "Checkers.h"
#include "Mandelbulb.h"
#include "Sphereflake.h"
#include "PointSource.h"
#include "Camera.h"
#include "TerminalDisplay.h"
#include "FlyCamera.h"
#include <cmath>

// A gold Mandelbulb and a chrome sphereflake side by side on a reflective checkerboard, under a
// blue sky.
static std::unique_ptr<World> build_scene(){
    auto w = std::make_unique<World>();
    Material floor_mat(0.1, 0.8, 0.2, 100.0, WHITE, 0.25, 0.0, 1.0,
                       std::make_shared<Checkers>(Color({0.15,0.15,0.2}), Color({0.85,0.85,0.9}), MatScaling(0.5,0.5,0.5)));
    w->add_shape(std::make_unique<Plane>(MatIdentity(4), floor_mat));

    Material gold(0.15, 0.75, 0.6, 60.0, Color({1.0, 0.72, 0.25}));
    gold.set_reflectance(0.15);
    // The bulb's symmetry axis is object-space z; rotate it to point up, then lift it off the floor.
    w->add_shape(std::make_unique<Mandelbulb>(MatTranslation(0, 1.55, 0) * MatScaling(1.5, 1.5, 1.5) * MatRotateX(-pi/2), gold));

    Material chrome(0.05, 0.3, 0.9, 200.0, Color(0.75, 0.75, 0.8));
    chrome.set_reflectance(0.6);
    w->add_shape(make_sphereflake(4, chrome, MatTranslation(4.2, 1, 0)));

    w->add_source(std::make_unique<PointSource>(WHITE, Tuple({-6, 9, -7}, TupType::POINT)));
    w->set_sky(Color(0.85, 0.9, 1.0), Color(0.25, 0.45, 0.85));   // pale at the horizon, deep blue overhead
    return w;
}

const double FIELD_OF_VIEW = pi/3.0;

// Starting view: back far enough to frame both objects, centred between them (x = 2.1), looking
// slightly down at (2.1, 1.3, 0).
static FlyCamera starting_camera(){
    return FlyCamera(GenPoint(2.1, 3.0, -8.5), 0.0, std::atan2(1.3 - 3.0, 8.5));
}

// Terminal-sized images are small, so 16x16 tiles leave too few tiles to keep every thread busy
// (an 80x24 terminal gives only 15). Measured median frame times, Mandelbulb scene, 20 threads:
//   tile 16 / 8 / 4 / 2:  80x24: 78 / 46 / 30 / 26 ms   100x30: 74 / 57 / 43 / 46 ms   200x60: 212 / 171 / 161 / 165 ms
constexpr int TERMINAL_TILE_SIZE = 4;

// Shows the scene in the terminal and flies the camera with the keyboard. Re-renders only when
// the camera moved or the terminal was resized.
static int run_in_terminal(World& world){
    TerminalDisplay display;
    FlyCamera fly = starting_camera();
    TerminalDisplay::Size last_size{0, 0};
    bool view_changed = true;
    while(true){
        // Apply every key that arrived since the last frame, then draw once. The terminal sends
        // key repeats while a key is held, so movement speed follows the repeat rate rather than
        // the frame rate, and a slow frame never leaves a backlog. If nothing needs redrawing,
        // wait briefly for input so an idle display doesn't spin the CPU.
        for(char key : display.read_keys(view_changed ? 0 : 100)){
            if(key == 'q' || key == 'Q' || key == 3 /* Ctrl-C */){return 0;}
            if(fly.handle_key(key)){view_changed = true;}
        }
        const TerminalDisplay::Size size = display.size();
        if(size == last_size && !view_changed){continue;}
        last_size = size;
        view_changed = false;

        // One row is kept for the status line; each remaining row shows two pixels.
        const int width = size.cols;
        const int height = 2 * (size.rows - 1);
        if(width < 1 || height < 2){continue;}

        const auto start = std::chrono::steady_clock::now();
        auto image = fly.make_camera(width, height, FIELD_OF_VIEW).render(&world, 5, 0, TERMINAL_TILE_SIZE, false);
        const double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();

        char status[160];
        const Tuple& p = fly.get_position();
        std::snprintf(status, sizeof(status), " %.0f ms | pos %.2f %.2f %.2f yaw %.2f pitch %.2f | wasd move, ijkl look, q quit",
                      ms, p[0], p[1], p[2], fly.get_yaw(), fly.get_pitch());
        display.draw(*image, status);
    }
}

int main(){
    auto world = build_scene();
    if(!::isatty(STDIN_FILENO) || !::isatty(STDOUT_FILENO)){
        // Not running in a terminal (e.g. output redirected): render one image to a file instead.
        starting_camera().make_camera(400, 300, FIELD_OF_VIEW).render(world.get())->save_ppm("MandelbulbAndSphereflake");
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
