#include <gtest/gtest.h>
#include <cmath>
#include <random>
#include "Mandelbulb.h"
#include "Constants.h"

TEST(Mandelbulb, RayMissingBoundingSphereMisses){
    Mandelbulb mb;
    Ray r({0, 2, -5}, {0, 0, 1});
    EXPECT_TRUE(mb.intersect(r).empty());
    Ray behind({0, 0, 5}, {0, 0, 1});                         // pointing away from the bulb
    EXPECT_TRUE(mb.intersect(behind).empty());
}

// Sphere tracing can overshoot: a step that is too large jumps past a thin feature and reports
// the hit too far along (or misses entirely). Check the march against a brute-force walk in
// tiny fixed steps, which only uses the distance estimate as an inside/outside test.
TEST(Mandelbulb, MarchAgreesWithFixedStepReference){
    Mandelbulb mb;
    const double step = 1e-4;
    for(double x = -1.0; x <= 1.0; x += 0.25){
        for(double y = -1.0; y <= 1.0; y += 0.25){
            Ray r({x, y, -2}, {0.1 * y, -0.1 * x, 1});
            double reference = -1;
            for(double t = 0; t < 4; t += step){
                Tuple p = r.position(t);
                if(mb.distance_estimate(p[0], p[1], p[2]) < mb.get_surface_epsilon()){
                    reference = t;
                    break;
                }
            }
            auto hits = mb.intersect(r);
            if(reference < 0){
                EXPECT_TRUE(hits.empty()) << "march hit where reference missed, ray x=" << x << " y=" << y;
            }
            else{
                ASSERT_EQ(hits.size(), 1u) << "march missed where reference hit, ray x=" << x << " y=" << y;
                // The march's last step can land anywhere inside the surface_epsilon shell, while
                // the reference stops at the shell's edge, so they legitimately differ by up to
                // surface_epsilon (converted to t units). An overshoot past a feature would be far larger.
                const double tolerance = mb.get_surface_epsilon() / r.get_direction().L2Norm() + step;
                EXPECT_NEAR(hits[0].get_t(), reference, tolerance) << "ray x=" << x << " y=" << y;
            }
        }
    }
}

TEST(Mandelbulb, RespectsTransform){
    Mandelbulb unit;
    Mandelbulb moved(MatTranslation(3, 0, 0) * MatScaling(2, 2, 2));
    Ray r_unit({0, 0, -5}, {0, 0, 1});
    Ray r_moved({3, 0, -10}, {0, 0, 1});                     // same ray in the moved bulb's frame
    auto h_unit = unit.intersect(r_unit);
    auto h_moved = moved.intersect(r_moved);
    ASSERT_EQ(h_unit.size(), 1u);
    ASSERT_EQ(h_moved.size(), 1u);
    // Scaling by 2 doubles the distance to the surface (5 - t_unit) as measured from the origin.
    EXPECT_NEAR(10 - h_moved[0].get_t(), 2 * (5 - h_unit[0].get_t()), 1e-3);
}

TEST(Mandelbulb, NormalFacesTheRay){
    Mandelbulb mb(MatRotateY(0.7) * MatRotateX(0.4));
    for(double x : {-0.3, 0.0, 0.25}){
        Ray r({x, 0.1, -5}, {0, 0, 1});
        auto hits = mb.intersect(r);
        ASSERT_EQ(hits.size(), 1u);
        Tuple n = mb.normal_at(r.position(hits[0].get_t()));
        EXPECT_LT(n.dot(r.get_direction()), 0.0);            // points back toward the viewer
    }
}

// A ray leaving the surface from the tracer's over-point must not hit the point it started
// from; otherwise every lit point would be in shadow ("shadow acne").
TEST(Mandelbulb, NoSelfHitLeavingSurface){
    Mandelbulb mb;
    Ray r({0, 0, -5}, {0, 0, 1});
    auto hits = mb.intersect(r);
    ASSERT_EQ(hits.size(), 1u);
    Tuple p = r.position(hits[0].get_t());
    Tuple n = mb.normal_at(p);
    Tuple over = p + n * glob_resolution;
    EXPECT_TRUE(mb.intersect(Ray(over, n)).empty());
}

TEST(Mandelbulb, TrigFreeEstimateMatchesTrigFormula){
    std::mt19937 g(7);
    std::uniform_real_distribution<double> u(-1.3, 1.3);
    for(double power : {2.0, 4.0, 8.0}){
        Mandelbulb mb(MatIdentity(4), Material(), nullptr, power);
        for(int i = 0; i < 20000; ++i){
            double x = u(g), y = u(g), z = u(g);
            if(i % 100 == 0){x = 0; y = 0;}
            ASSERT_NEAR(mb.distance_estimate(x, y, z), mb.distance_estimate_trig(x, y, z), 1e-9)
                << "power " << power << " at " << x << ", " << y << ", " << z;
        }
    }
}
