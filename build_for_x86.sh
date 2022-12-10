set -e
set -u

BUILD_DIR="build_x86"

if [ -d "$BUILD_DIR" ] ; then
    echo "$BUILD_DIR directory exists"
else
    mkdir $BUILD_DIR
fi

cd $BUILD_DIR
PWD=$(pwd)

cmake .. \
-DMNN_BUILD_DEMO=ON \
-DMNN_BUILD_CONVERTER=ON \
-DMNN_BUILD_TORCH=ON \
-DMNN_BUILD_TOOL=ON \
-DMNN_BUILD_BENCHMARK=ON \
-DMNN_BUILD_QUANTOOLS=ON \


make -j8
