My own C++ Implementation of the [Ray Tracer Challenge](https://raytracerchallenge.com/).

# Compiling
I assume that you are on Linux, and that you have cmake and make installed. It would also be good to have ImageMagick to view the generated images. Assuming the above, run the following commands in the base directory of the repo:

```
mkdir build
cd build
cmake ../RayTracer/ -DTESTING=True
make
```
 
# Running
## Running main Tracer
Don't. I haven't implemented something cool yet.

## Tests
If you want to run the tests
### Without generating images
./Tracer --gtest_filter=-GenImage.*
### Generating images
./Tracer --gtest_filter=GenImage.*