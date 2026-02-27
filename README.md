My own C++ Implementation of the [Ray Tracer Challenge](https://raytracerchallenge.com/).

# Compiling
I assume that you are on Linux, and that you have bazel installed. It would also be good to have ImageMagick to view the generated images. Assuming the above, run the following commands in the base directory of the repo:

```
bazel build Tracer 
```
 
# Running
## Running main Tracer

```
bazel run Tracer
```

## Tests
If you want to run the tests
### Without generating images
bazel test Tracer_tests --test_filter=-GenImage.*
### Generating images
bazel test Tracer_tests --test_filter=GenImage.*
