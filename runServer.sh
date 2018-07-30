#!/bin/sh

root_folder=$(cd "$(dirname "$0")";pwd)

echo "run login system server:"
cd ${root_folder}
cd out
./server