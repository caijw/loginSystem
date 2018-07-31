#!/bin/sh

root_folder=$(cd "$(dirname "$0")";pwd)


cd login_client_ios
pod install
open login_client_ios.xcworkspace