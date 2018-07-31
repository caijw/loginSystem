load("@org_pubref_rules_protobuf//cpp:rules.bzl", "GRPC_COMPILE_DEPS")




cc_library(
    name = "crypto_impl",
    srcs = ["crypto_impl.cpp"],
    hdrs = ["crypto_impl.hpp"],
    deps = ["//djinni:crypto"],
    copts = ["-Idjinni/generated-src/cpp/"],
)

cc_library(
    name = "util",
    srcs = ["util.h"],
)

cc_library(
    name = "Database",
    srcs = ["Database.cpp"],
    hdrs = ["Database.h"],
    copts = ["-I/usr/local/include/mongocxx/v_noabi", "-I/usr/local/include/bsoncxx/v_noabi"],
)

cc_library(
  name = "loginStreamManager",
  srcs = ["loginStreamManager.cpp"],
  hdrs = ["loginStreamManager.h"],
  copts = ["-I/usr/include", "-I/usr/local/include"],
)

cc_library(
    name = "user_id",
    srcs = ["user_id.cpp"],
    hdrs = ["user_id.h"],
    copts = [ "-I/usr/local/include/mongocxx/v_noabi", "-I/usr/local/include/bsoncxx/v_noabi"],
    deps = ["Database"],
)

cc_binary(
  name = "client",
  srcs = ['client.cpp'],
  deps = [
    "//protos:login_system_proto", "util", "crypto_impl"
  ] + GRPC_COMPILE_DEPS,
  copts = ["-Idjinni/generated-src/cpp/"],
  linkopts = ["-L/usr/local/lib", "-lmongocxx", "-lbsoncxx"],
)



cc_binary(
  name = "server",
  srcs = ['server.cpp'],
  deps = [
    "//protos:login_system_proto", "util", "crypto_impl", "Database", "loginStreamManager", "user_id"
  ] + GRPC_COMPILE_DEPS,
  copts = ["-I/usr/local/include/mongocxx/v_noabi", "-I/usr/local/include/bsoncxx/v_noabi", "-Idjinni/generated-src/cpp/"],
  linkopts = ["-L/usr/local/lib", "-lmongocxx", "-lbsoncxx"],
)