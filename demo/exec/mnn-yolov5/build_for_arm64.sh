set -e
set -u

#
# wget -c https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64-linux-gnu/gcc-linaro-7.5.0-2019.12-x86_64_aarch64-linux-gnu.tar.xz
#

BUILD_DIR="build_arm64"

if [ -d "$BUILD_DIR" ] ; then
    echo "$BUILD_DIR directory exists"
else
    mkdir $BUILD_DIR
fi

cd $BUILD_DIR


cmake .. \
-DCMAKE_SYSTEM_NAME=Linux \
-DCMAKE_SYSTEM_VERSION=1 \
-DCMAKE_SYSTEM_PROCESSOR=aarch64 \
-DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
-DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++ \

# -DMNN_BUILD_CONVERTER=ON \
# -DMNN_BUILD_TORCH=ON \
# -DMNN_BUILD_TOOL=ON \
# -DMNN_BUILD_BENCHMARK=ON \
# -DMNN_BUILD_QUANTOOLS=ON \


make -j8

