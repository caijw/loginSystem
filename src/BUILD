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
    copts = [ 
      "-Ilibs/mongod-cxx/include/mongocxx/v_noabi", 
      "-Ilibs/mongod-cxx/include/bsoncxx/v_noabi",
    ],
    deps = [
      "//libs/mongod-cxx:bson-cxx",
      "//libs/mongod-cxx:mongoc-cxx"
    ],
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
    copts = [ 
      "-Ilibs/mongod-cxx/include/mongocxx/v_noabi", 
      "-Ilibs/mongod-cxx/include/bsoncxx/v_noabi",
    ],
    deps = ["Database", "//libs/mongod-cxx:bson-cxx", "//libs/mongod-cxx:mongoc-cxx"],
)

cc_binary(
  name = "client",
  srcs = ['client.cpp'],
  deps = [
    "//protos:login_system_proto",
    "util",
    "crypto_impl",
    "//libs/mongod-cxx:bson-cxx",
    "//libs/mongod-cxx:mongoc-cxx"
  ] + GRPC_COMPILE_DEPS,
  copts = [
    "-Idjinni/generated-src/cpp/",
    "-Ilibs/mongod-cxx/include/mongocxx/v_noabi", 
    "-Ilibs/mongod-cxx/include/bsoncxx/v_noabi",

  ],
#  linkopts = ["-L/usr/local/lib", "-lmongocxx", "-lbsoncxx"],
)



cc_binary(
  name = "server",
  srcs = ['server.cpp'],
  deps = [
    "//protos:login_system_proto", 
    "util", "crypto_impl", 
    "Database", 
    "loginStreamManager", 
    "user_id","//libs/mongod-cxx:bson-cxx", 
    "//libs/mongod-cxx:mongoc-cxx"
  ] + GRPC_COMPILE_DEPS,
  copts = [
    "-Ilibs/mongod-cxx/include/mongocxx/v_noabi", 
    "-Ilibs/mongod-cxx/include/bsoncxx/v_noabi",
    "-Idjinni/generated-src/cpp/"
  ],
#  linkopts = ["-L/usr/local/lib"],
)