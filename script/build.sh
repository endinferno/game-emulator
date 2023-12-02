#!/bin/bash

PrintWithColor() {
	echo -e "$1$2\e[0m\c"
}

PrintBuildType() {
	echo -e "Compiling \c"
	PrintWithColor "\e[31m" $BUILD_TYPE
	echo " Version"
}

CloneSubmodule() {
	PrintWithColor "\e[32m" "Clone Submodule Phase"
	echo ""
	pushd $BUILD_DIR > /dev/null
	git submodule update --init
	popd > /dev/null
}

CMakePhase() {
	PrintWithColor "\e[32m" "CMake Phase"
	echo ""
	pushd $BUILD_DIR > /dev/null
	cmake -B $BUILD_PATH \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=1 \
		-DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
	popd > /dev/null
}

GetSystemCore() {
	osName=$(uname -s)
	osCore=0
	if [[ $osName == "Darwin" ]]
	then
		osCore=$(sysctl -n hw.ncpu)
	elif [[ $osName == "Linux" ]]
	then
		osCore=$(cat /proc/cpuinfo | grep "processor" | wc -l)
	else
		osCore=1
	fi

	return $osCore
}

MakePhase() {
	PrintWithColor "\e[32m" "Make Phase"
	echo ""
	pushd $BUILD_DIR/$BUILD_PATH > /dev/null
	make -j $1
	popd > /dev/null
}

BUILD_DIR=$(pwd)/build
# Create build directory
if [ ! -d $BUILD_DIR ];
then
	mkdir $BUILD_DIR
fi

# Read Build Type
BUILD_TYPE=debug
if [ $# -eq 1 ];
then
	BUILD_PARAM=$1
	BUILD_TYPE=${BUILD_PARAM,,}
	if [ $BUILD_TYPE != "debug" ] && [ $BUILD_TYPE != "release" ];
	then
		BUILD_TYPE=debug
	fi
fi

BUILD_PATH=${BUILD_TYPE}
BUILD_TYPE=${BUILD_TYPE^}

PrintBuildType
CloneSubmodule
CMakePhase
GetSystemCore
MakePhase $?
