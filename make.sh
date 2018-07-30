#!/bin/sh

root_folder=$(cd "$(dirname "$0")";pwd)

cd src
make server

cd ${root_folder}
cd login_client_ios
pod install
open login_client_ios.xcworkspace

echo "run login system server:"
cd ${root_folder}
cd out
./server