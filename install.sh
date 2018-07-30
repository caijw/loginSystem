#!/bin/sh

root_folder=$(cd "$(dirname "$0")";pwd)
cur_user=$USER


if hash brew 2>/dev/null; then
    echo "brew exist"
else
	echo "brew dose not exist"
    /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
fi

if hash git 2>/dev/null; then
    echo "git exist"
else
	echo "git dose not exist"
	brew install git
fi

if hash pkg-config 2>/dev/null; then
    echo "pkg-config exist"
else
	echo "pkg-config dose not exist"
	brew install pkg-config
fi


if hash mongod 2>/dev/null; then
    echo "mongodb exist"
else
	echo "mongodb dose not exist"
	brew install mongodb
fi

if hash cmake 2>/dev/null; then
    echo "cmake exist"
else
	echo "cmake dose not exist"
	brew install cmake
fi

if hash autoconf 2>/dev/null; then
    echo "autoconf exist"
else
	echo "autoconf dose not exist"
	brew install autoconf
fi

if hash automake 2>/dev/null; then
    echo "automake exist"
else
	echo "automake dose not exist"
	brew install automake
fi

if hash libtool 2>/dev/null; then
    echo "libtool exist"
else
	echo "libtool dose not exist"
	brew install libtool
fi

if hash shtool 2>/dev/null; then
    echo "shtool exist"
else
	echo "shtool dose not exist"
	brew install shtool
fi

if hash gflags 2>/dev/null; then
    echo "gflags exist"
else
	echo "gflags dose not exist"
	brew install gflags
fi

if hash bazel 2>/dev/null; then
    echo "bazel exist"
else
	echo "bazel dose not exist"
	brew install bazel
fi

sh ./install2.sh


