Skip to content
 
Search or jump to…

Pull requests
Issues
Marketplace
Explore
 @caijw Sign out
You don’t have any verified emails. We recommend verifying at least one email.
Email verification helps our support team verify ownership if you lose account access and allows you to receive all the notifications you ask for.
1
0 0 caijw/loginSystem
 Code  Issues 0  Pull requests 0  Projects 0  Wiki  Insights  Settings
loginSystem/src/Makefile
7044d38  2 days ago
 kingweicai add auto install sh
      
114 lines (98 sloc)  3.23 KB
mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))

HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)

${warning ${SYSTEM}}

CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf grpc libmongocxx`
CXXFLAGS += -std=c++11 -I./../deps/plog/include/ -I./../deps/cryptopp/ -I./../djinni/generated-src/cpp

ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc libmongocxx`\
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib `pkg-config --libs protobuf grpc++ grpc libmongocxx`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
endif
INC = -I./../deps/plog/include/ -I./../deps/cryptopp/
PROTOC = protoc
GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

PROTOS_PATH = ../protos

vpath %.proto $(PROTOS_PATH)

all: server

client: login_system.pb.o login_system.grpc.pb.o crypto_impl.o client.o
	$(CXX) $^ $(LDFLAGS) -o $@
	@[ -d ../out ] || mkdir ../out; 	\
	cp client ../out/client; 			\
	rm client; 							\
	cp *.crt ../out/;					\
	cp *.csr ../out/;					\
	cp *.key ../out/;
server: login_system.pb.o login_system.grpc.pb.o user_id.o Database.o crypto_impl.o loginStreamManager.o  server.o
	$(CXX) $^ $(LDFLAGS) -o $@
	@[ -d ../out ] || mkdir ../out;		\
	cp server ../out/server;			\
	rm server;							\
	cp *.crt ../out/;					\
	cp *.csr ../out/;					\
	cp *.key ../out/;
.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

clean:
	rm -f *.o *.pb.cc *.pb.h; \
	rm -rf ../out


# The following is to test your system and ensure a smoother experience.
# They are by no means necessary to actually compile a grpc-enabled software.

PROTOC_CMD = which $(PROTOC)
PROTOC_CHECK_CMD = $(PROTOC) --version | grep -q libprotoc.3
PLUGIN_CHECK_CMD = which $(GRPC_CPP_PLUGIN)
HAS_PROTOC = $(shell $(PROTOC_CMD) > /dev/null && echo true || echo false)
ifeq ($(HAS_PROTOC),true)
HAS_VALID_PROTOC = $(shell $(PROTOC_CHECK_CMD) 2> /dev/null && echo true || echo false)
endif
HAS_PLUGIN = $(shell $(PLUGIN_CHECK_CMD) > /dev/null && echo true || echo false)

SYSTEM_OK = false
ifeq ($(HAS_VALID_PROTOC),true)
ifeq ($(HAS_PLUGIN),true)
SYSTEM_OK = true
endif
endif

system-check:
ifneq ($(HAS_VALID_PROTOC),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have protoc 3.0.0 installed in your path."
	@echo "Please install Google protocol buffers 3.0.0 and its compiler."
	@echo "You can find it here:"
	@echo
	@echo "   https://github.com/google/protobuf/releases/tag/v3.0.0"
	@echo
	@echo "Here is what I get when trying to evaluate your version of protoc:"
	@echo
	-$(PROTOC) --version
	@echo
	@echo
endif
ifneq ($(HAS_PLUGIN),true)
	@echo " DEPENDENCY ERROR"
	@echo
	@echo "You don't have the grpc c++ protobuf plugin installed in your path."
	@echo "Please install grpc. You can find it here:"
	@echo
	@echo "   https://github.com/grpc/grpc"
	@echo
	@echo "Here is what I get when trying to detect if you have the plugin:"
	@echo
	-which $(GRPC_CPP_PLUGIN)
	@echo
	@echo
endif
ifneq ($(SYSTEM_OK),true)
	@false
endif