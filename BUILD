load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library", "cc_test")

# 1. Create a library for your core logic (shared between main and tests)
cc_library(
    name = "tracer_lib",
    srcs = glob(["src/*.cpp"], exclude = ["src/main.cpp"]),
    hdrs = glob(["include/*.h"]),
    includes = ["include"],
    copts = ["-Wall"], # Equivalent to your GCC_COVERAGE_COMPILE_FLAGS
    linkopts = ["-pthread"],
)

# 2. The Main Executable (Your "Tracer" app)
cc_binary(
    name = "Tracer",
    srcs = ["src/main.cpp"],
    data = ["obj/Apple.obj", "obj/teapot.obj"],
    deps = [":tracer_lib",
        "@bazel_tools//tools/cpp/runfiles"],
)

# 3. The Test Suite
cc_test(
    name = "Tracer_tests",
    srcs = glob(["tests/*.cpp"]),
    data = ["obj/Apple.obj", "obj/teapot.obj"],
    deps = [
        ":tracer_lib",
        "@googletest//:gtest",
        "@googletest//:gtest_main",
        "@bazel_tools//tools/cpp/runfiles"
    ],
)
