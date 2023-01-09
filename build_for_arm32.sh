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
-DCMAKE_TOOLCHAIN_FILE=/home/zhouli/work/tools/toolchain-arm32.cmake \
-DCMAKE_PREFIX_PATH=/home/zhouli/work/opencv/build_arm32

make -j8

cp ../export.sh ./bin/
cp express/libMNN_Express.so ./bin/
cp libMNN.so ./bin/
cp -r ../demo/exec/mnn-yolov5/checkpoints ../demo/exec/mnn-yolov5/000070.jpg ../demo/exec/mnn-yolov5/coco.names ./bin/
