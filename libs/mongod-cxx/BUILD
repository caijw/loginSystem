
cc_library(
  name = "bson-cxx",
  srcs = ["lib/libbsoncxx.3.4.0-rc0-pre.dylib"],
  hdrs = glob(["include/bsoncxx/v_noabi/bsoncxx/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/array/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/builder/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/builder/basic/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/builder/stream/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/config/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/document/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/enums/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/exception/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/stdx/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/string/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/third_party/mnmlstc/core/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/types/*.hpp"])
  		+glob(["include/bsoncxx/v_noabi/bsoncxx/util/*.hpp"]),
  visibility = ["//visibility:public"],
  deps = ["//libs/mongod-c:bson-c", "//libs/mongod-c:mongoc-c"],
)



cc_library(
  name = "mongoc-cxx",
  srcs = ["lib/libmongocxx.3.4.0-rc0-pre.dylib"],
  hdrs = glob(["include/mongocxx/v_noabi/mongocxx/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/config/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/exception/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/gridfs/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/model/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/options/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/options/gridfs/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/result/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/result/gridfs/*.hpp"])
  		+glob(["include/mongocxx/v_noabi/mongocxx/test/*.hpp"]),
  visibility = ["//visibility:public"],
  deps = ["//libs/mongod-c:bson-c", "//libs/mongod-c:mongoc-c"],
)