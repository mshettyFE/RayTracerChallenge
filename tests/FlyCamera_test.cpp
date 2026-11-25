#include <gtest/gtest.h>
#include <cmath>
#include "FlyCamera.h"
#include "Constants.h"

TEST(FlyCamera, MovementKeysFollowTheView){
    FlyCamera fly(GenPoint(0, 0, 0), 0.0, 0.0, 1.0);
    EXPECT_TRUE(fly.handle_key('w'));
    EXPECT_EQ(fly.get_position(), GenPoint(0, 0, 1));       // yaw 0 looks along +z
    fly.handle_key('d');
    EXPECT_EQ(fly.get_position(), GenPoint(1, 0, 1));
    fly.handle_key('s');
    fly.handle_key('a');
    EXPECT_EQ(fly.get_position(), GenPoint(0, 0, 0));
    EXPECT_TRUE(fly.handle_key('W'));                         // caps lock still works
    EXPECT_FALSE(fly.handle_key('x'));                        // not a movement key: view unchanged
}

// What "right" and "up" mean must match the rendered image, not just the math: the rightmost
// pixel's ray should point along right(), and after turning right the view should swing that way.
TEST(FlyCamera, KeysMatchScreenDirections){
    FlyCamera fly(GenPoint(0, 1, -5), 0.4, -0.2);
    Camera cam = fly.make_camera(21, 21, pi/3);
    EXPECT_GT(cam.ray_for_pixel(20, 10).get_direction().dot(fly.right()), 0.0);
    EXPECT_LT(cam.ray_for_pixel(0, 10).get_direction().dot(fly.right()), 0.0);
    EXPECT_GT(cam.ray_for_pixel(10, 0).get_direction()[1], cam.ray_for_pixel(10, 20).get_direction()[1]);   // row 0 is the top

    Tuple right_before = fly.right();
    Tuple forward_before = fly.forward();
    fly.handle_key('l');
    EXPECT_GT(fly.forward().dot(right_before), forward_before.dot(right_before));   // 'l' turns right
    fly.handle_key('i');
    EXPECT_GT(fly.forward()[1], forward_before[1]);                                  // 'i' looks up
}

TEST(FlyCamera, PitchStopsShortOfVertical){
    FlyCamera fly(GenPoint(0, 0, 0));
    for(int i = 0; i < 100; ++i){fly.handle_key('i');}
    EXPECT_DOUBLE_EQ(fly.get_pitch(), FlyCamera::MAX_PITCH);
    for(int i = 0; i < 100; ++i){fly.handle_key('k');}
    EXPECT_DOUBLE_EQ(fly.get_pitch(), -FlyCamera::MAX_PITCH);
    fly.make_camera(10, 10, pi/3);   // still a valid view (up not parallel to forward)
}
