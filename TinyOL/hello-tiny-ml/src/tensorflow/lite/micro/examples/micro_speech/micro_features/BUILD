# Library for generating feature vectors from audio data
package(
    default_visibility = ["//visibility:public"],
    # Disabling layering_check because of http://b/177257332
    features = ["-layering_check"],
    licenses = ["notice"],
)

cc_library(
    name = "micro_model_settings",
    srcs = [
        "micro_model_settings.cc",
    ],
    hdrs = [
        "micro_model_settings.h",
    ],
)

cc_library(
    name = "micro_features_test_data",
    srcs = [
        "no_micro_features_data.cc",
        "yes_micro_features_data.cc",
    ],
    hdrs = [
        "no_micro_features_data.h",
        "yes_micro_features_data.h",
    ],
)

cc_library(
    name = "static_alloc",
    hdrs = ["static_alloc.h"],
)

cc_library(
    name = "micro_features_generator",
    srcs = [
        "micro_features_generator.cc",
    ],
    hdrs = [
        "micro_features_generator.h",
    ],
    deps = [
        ":micro_model_settings",
        "//tensorflow/lite/c:common",
        "//tensorflow/lite/experimental/microfrontend/lib:frontend",
        "//tensorflow/lite/micro:micro_error_reporter",
    ],
)

cc_library(
    name = "micro_features_generator_test_data",
    srcs = [
        "no_feature_data_slice.cc",
        "yes_feature_data_slice.cc",
    ],
    hdrs = [
        "no_feature_data_slice.h",
        "yes_feature_data_slice.h",
    ],
)

cc_test(
    name = "micro_features_generator_test",
    size = "small",
    srcs = [
        "micro_features_generator_test.cc",
    ],
    tags = [
        "noasan",  # TODO(b/179930607): Fix with asan.
    ],
    deps = [
        ":micro_features_generator",
        ":micro_features_generator_test_data",
        ":micro_model_settings",
        "//tensorflow/lite/c:common",
        "//tensorflow/lite/micro:micro_error_reporter",
        "//tensorflow/lite/micro:micro_framework",
        "//tensorflow/lite/micro/examples/micro_speech:audio_sample_test_data",
        "//tensorflow/lite/micro/testing:micro_test",
    ],
)
