workspace(name = "cpp")

# new_local_repository(
#     name = "libbenchmark",
#     path = "/usr",
#     build_file = "third_party/libbenchmark/libbenchmark.BUILD",
# )


load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "gtest",
    url = "https://github.com/google/googletest/archive/release-1.10.0.zip",
    sha256 = "94c634d499558a76fa649edb13721dce6e98fb1e7018dfaeba3cd7a083945e91",
    strip_prefix = "googletest-release-1.10.0",
)

http_archive(
    name = "benchmark",
    url = "https://github.com/google/benchmark/archive/refs/tags/v1.7.1.zip",
    sha256 = "aeec52381284ec3752505a220d36096954c869da4573c2e1df3642d2f0a4aac6",
    strip_prefix = "benchmark-1.7.1",
)
