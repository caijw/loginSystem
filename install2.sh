#!/bin/sh

# rm -rf ./deps
root_folder=$(cd "$(dirname "$0")";pwd)



# protocbuf juegement
HAS_PROTOC=`which protoc > /dev/null && echo true || echo false`

HAS_VALID_PROTOC=`echo false`
HAS_PLUGIN=`echo false`

if [ ${HAS_PROTOC} ]
then
	HAS_VALID_PROTOC=`protoc --version | grep -q libprotoc.3 2> /dev/null && echo true || echo false`
fi

HAS_PLUGIN=`which grpc_cpp_plugin > /dev/null && echo true || echo false`


SYSTEM_OK=false

if [ ${HAS_VALID_PROTOC} ]
then
	if [ ${HAS_PLUGIN} ]
	then
		SYSTEM_OK=true
	fi
fi




mongo_c_driver_path="deps/mongo-c-driver"
mongo_cxx_driver_path="deps/mongo-cxx-driver"
grpc_path="deps/grpc"
djinni_path="deps/djinni"
djinni_def_path="djinni"
plog_path="plog"


echo "root_folder: "${root_folder}

# install mongo-c-driver
if [ ! -d ${mongo_c_driver_path} ]; then
	git clone https://github.com/mongodb/mongo-c-driver.git ${mongo_c_driver_path}
fi
cd ${mongo_c_driver_path}
git checkout 1.12.0
mkdir cmake-build
cd cmake-build
cmake -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..
make
sudo make install
cd ${root_folder}

#install mongo-cxx-driver
if [ ! -d ${mongo_cxx_driver_path} ]; then
	
	git clone https://github.com/mongodb/mongo-cxx-driver.git ${mongo_cxx_driver_path}  --branch releases/stable --depth 1

fi
cd ${mongo_cxx_driver_path}"/build"
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
sudo make EP_mnmlstc_core
make
sudo make install
cd ${root_folder}

#install djinni
if [ ! -d ${djinni_path} ]; then
	git clone https://github.com/dropbox/djinni.git ${djinni_path}
fi

cd ${djinni_def_path}
sh ./run_djinni.sh
cd ${root_folder}

#install grpc and protobuf
if [ ! -d ${grpc_path} ]; then

	git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc ${grpc_path}

fi

cd ${grpc_path}
git submodule update --init

cd third_party/protobuf
./autogen.sh
./configure
make
make check
sudo make install
cd ${root_folder}

cd ${grpc_path}
make
sudo make install
cd ${root_folder}


#install cocoapods
sudo gem install cocoapods

cd login_client_ios
pod install








