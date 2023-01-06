set -e
set -u


BUILD_DIR="build_arm32"

if [ -d "$BUILD_DIR" ] ; then
    echo "$BUILD_DIR directory exists"
else
    mkdir $BUILD_DIR
fi

cd $BUILD_DIR


cmake .. \
-DMNN_BUILD_DEMO=ON \
-DMNN_BUILD_TOOLS=OFF \
-DCMAKE_SYSTEM_NAME=Linux \
-DCMAKE_SYSTEM_VERSION=1 \
-DCMAKE_SYSTEM_PROCESSOR=arm \
-DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc \
-DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ \
-DCMAKE_PREFIX_PATH=/home/zhouli/work/opencv/build_arm32

# -DMNN_BUILD_CONVERTER=ON \
# -DMNN_BUILD_TORCH=ON \
# -DMNN_BUILD_TOOL=ON \
# -DMNN_BUILD_BENCHMARK=ON \
# -DMNN_BUILD_QUANTOOLS=ON \


make -j8

cp ../export.sh ./bin/
cp express/libMNN_Express.so ./bin/
cp libMNN.so ./bin/
cp -r ../demo/exec/mnn-yolov5/checkpoints ../demo/exec/mnn-yolov5/000070.jpg ../demo/exec/mnn-yolov5/coco.names ./bin/
