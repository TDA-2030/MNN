## 使用MNN部署YoloV5模型  

### 一. 将 ONNX 模型转换为 MNN 模型  

执行命令：  
```
./MNN-1.1.0/build/MNNConvert -f ONNX --modelFile yolov5-sort-cpp/yolov5ss.onnx --MNNModel yolov5ss.mnn --bizCode MNN
```

运行测试：  
```
./build_x86/bin/yolov5.out  demo/exec/mnn-yolov5/checkpoints/yolov5s.mnn demo/exec/mnn-yolov5/coco.names demo/exec/mnn-yolov5/000070.jpg out.jpg
```
