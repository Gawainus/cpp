# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_binary

cc_binary(
    name = "incre_int",
    srcs = ["incre_int.cc"],
    copts = [
    ],
    deps = [
        "@spdlog"
    ],
)


cc_binary(
    name = "chrono",
    srcs = ["chrono.cc"],
    copts = [
    ],
    deps = [
    ],
)


cc_binary(
    name = "move",
    srcs = ["move.cc"],
    copts = [
        "-std=c++20"
    ],
    deps = [
         "@benchmark//:benchmark_main",
    ],
)

cc_binary(
    name = "lambda",
    srcs = ["lambda.cc"],
    deps = [
    ],
    copts = [
        "-std=c++20"
    ],
)

cc_library(
    name = "vector",
    hdrs = ["vector.h"],
    deps = [
    ],
    copts = [
    ],
)


cc_binary(
    name = "bm_vector",
    srcs = ["bm_vector.cc"],
    deps = [
        ":vector",
        "@benchmark//:benchmark_main",
    ],
    copts = [
        "-std=c++20"
    ],
)

cc_binary(
    name = "bm_maps",
    srcs = ["bm_maps.cc"],
    deps = [
        "@benchmark//:benchmark_main",
    ],
    copts = [
        "-std=c++23"
    ],
)

cc_binary(
    name = "bm_span",
    srcs = ["bm_span.cc"],
    deps = [
        "@benchmark//:benchmark_main",
    ],
    copts = [
        "-std=c++23"
    ],
)

cc_binary(
    name = "variadic_template",
    srcs = ["variadic_template.cc"],
    deps = [
        "@benchmark//:benchmark_main",
    ],
    copts = [
        "-std=c++20"
    ],
)

cc_binary(
    name = "thread",
    srcs = ["thread.cc"],
    deps = [
        "@benchmark//:benchmark_main",
    ],
    copts = [
        "-std=c++20"
    ],
)