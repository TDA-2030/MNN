#include <iostream>
#include <string>
#include <stdio.h>
#include <MNN/MNNDefine.h>
#include <MNN/MNNForwardType.h>
#include <MNN/Interpreter.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#include <MNN/ImageProcess.hpp>
#define MNN_OPEN_TIME_TRACE
#include <MNN/AutoTime.hpp>

#include "../include/Yolo.h"

using namespace MNN;

static void show_shape(std::vector<int> shape)
{
    std::cout << shape[0] << " " << shape[1] << " " << shape[2] << " " << shape[3] << " " << shape[4] << " " << std::endl;

}

static void scale_coords(std::vector<BoxInfo> &boxes, int w_from, int h_from, int w_to, int h_to)
{
    float w_ratio = float(w_to) / float(w_from);
    float h_ratio = float(h_to) / float(h_from);


    for (auto &box : boxes) {
        box.x1 *= w_ratio;
        box.x2 *= w_ratio;
        box.y1 *= h_ratio;
        box.y2 *= h_ratio;
    }
    return ;
}

static int draw_box(uint8_t *rgbaPtr, std::vector<BoxInfo> &boxes, const std::vector<std::string> &labels)
{
    // int CNUM = 80;
    // cv::RNG rng(0xFFFFFFFF);
    // cv::Scalar_<int> randColor[CNUM];
    // for (int i = 0; i < CNUM; i++)
    //  rng.fill(randColor[i], cv::RNG::UNIFORM, 0, 256);

    // for(auto box : boxes)
    // {
    //     int width = box.x2-box.x1;
    //     int height = box.y2-box.y1;
    //     int id = box.id;
    //     cv::Point p = cv::Point(box.x1, box.y1);
    //     cv::Rect rect = cv::Rect(box.x1, box.y1, width, height);
    //     cv::rectangle(cv_mat, rect, randColor[box.label]);
    //     string text = labels[box.label] + ":" + std::to_string(box.score) + " ID:" + std::to_string(id);
    //     cv::putText(cv_mat, text, p, cv::FONT_HERSHEY_PLAIN, 1, randColor[box.label]);
    // }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 4) {
        std::cout << "Usage: ./yolov5.out model.mnn input.jpg pose.jpg" << std::endl;
        return -1;
    }

    const auto poseModel           = argv[1];
    const auto inputImageFileName  = argv[2];
    const auto outputImageFileName = argv[3];
    int count=1;
    if(argc >4){
        count = atoi(argv[4]);
    }
    
#if 0
    std::string model_name = "checkpoints/yolov5s.mnn";
    int num_classes = 80;
    std::vector<YoloLayerData> yolov5s_layers{
        {"437",    32, {{116, 90}, {156, 198}, {373, 326}}},
        {"417",    16, {{30,  61}, {62,  45},  {59,  119}}},
        {"output", 8,  {{10,  13}, {16,  30},  {33,  23}}},
    };
    std::vector<YoloLayerData> &layers = yolov5s_layers;
    std::vector<std::string> labels{"person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic light",
                                    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
                                    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
                                    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
                                    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple",
                                    "sandwich", "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "couch",
                                    "potted plant", "bed", "dining table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell phone",
                                    "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear",
                                    "hair drier", "toothbrush"};
#else
    std::string model_name = "checkpoints/yolov5ss.mnn";
    int num_classes = 5;
    std::vector<YoloLayerData> yolov5ss_layers{
        {"415",    32, {{116, 90}, {156, 198}, {373, 326}}},
        {"395",    16, {{30,  61}, {62,  45},  {59,  119}}},
        {"output", 8,  {{10,  13}, {16,  30},  {33,  23}}},
    };
    std::vector<YoloLayerData> &layers = yolov5ss_layers;
    std::vector<std::string> labels{"person", "vehicle", "outdoor", "animal", "accessory"};
#endif
    int net_size = 640;
    std::shared_ptr<MNN::Interpreter> net = std::shared_ptr<MNN::Interpreter>(MNN::Interpreter::createFromFile(poseModel));
    if (nullptr == net) {
        printf("net create failed\n");
        return -1;
    }

    MNN::ScheduleConfig config;
    config.numThread = 8;
    config.type      = static_cast<MNNForwardType>(MNN_FORWARD_CPU);
    MNN::BackendConfig backendConfig;
    backendConfig.precision = (MNN::BackendConfig::PrecisionMode)2;
    // backendConfig.precision =  MNN::PrecisionMode Precision_Normal; // static_cast<PrecisionMode>(Precision_Normal);
    config.backendConfig = &backendConfig;
    MNN::Session *session = net->createSession(config);
    auto inputTensor = net->getSessionInput(session, nullptr);
    int INPUT_SIZE = 640;
    const int targetWidth  = INPUT_SIZE;
    const int targetHeight = INPUT_SIZE;
    int originalWidth;
    int originalHeight;
    int originChannel;

#if 0
    {

        std::string image_name = "000070.jpg";
        // load image
        cv::Mat raw_image    = cv::imread(image_name.c_str());
        cv::Mat image;
        cv::resize(raw_image, image, cv::Size(INPUT_SIZE, INPUT_SIZE));

        // preprocessing
        image.convertTo(image, CV_32FC3);
        // image = (image * 2 / 255.0f) - 1;
        image = image / 255.0f;

        // wrapping input tensor, convert nhwc to nchw
        std::vector<int> dims{1, INPUT_SIZE, INPUT_SIZE, 3};
        auto nhwc_Tensor = MNN::Tensor::create<float>(dims, NULL, MNN::Tensor::TENSORFLOW);
        auto nhwc_data   = nhwc_Tensor->host<float>();
        auto nhwc_size   = nhwc_Tensor->size();
        std::memcpy(nhwc_data, image.data, nhwc_size);

        inputTensor->copyFromHostTensor(nhwc_Tensor);
    }
#else
    auto inputImage = stbi_load(inputImageFileName, &originalWidth, &originalHeight, &originChannel, 4);
    if (nullptr == inputImage) {
        MNN_ERROR("Invalid path: %s\n", inputImageFileName);
        return -1;
    }
    printf("img w:%d, h:%d\n", originalWidth, originalHeight);
    // preprocess input image
    {
        const float means[3] = {127.5f, 127.5f, 127.5f};
        const float norms[3] = {2.0f / 255.0f, 2.0f / 255.0f, 2.0f / 255.0f};
        CV::ImageProcess::Config preProcessConfig;
        ::memcpy(preProcessConfig.mean, means, sizeof(means));
        ::memcpy(preProcessConfig.normal, norms, sizeof(norms));
        preProcessConfig.sourceFormat = CV::RGBA;
        preProcessConfig.destFormat   = CV::RGB;
        preProcessConfig.filterType   = CV::BILINEAR;

        auto pretreat = std::shared_ptr<CV::ImageProcess>(CV::ImageProcess::create(preProcessConfig));
        CV::Matrix trans;

        // Dst -> [0, 1]
        trans.postScale(1.0 / targetWidth, 1.0 / targetHeight);
        //[0, 1] -> Src
        trans.postScale(originalWidth, originalHeight);

        pretreat->setMatrix(trans);
        const auto rgbaPtr = reinterpret_cast<uint8_t *>(inputImage);
        pretreat->convert(rgbaPtr, originalWidth, originalHeight, 0, inputTensor);
    }
#endif
    // run network
    AUTOTIME;
    for (size_t i = 0; i < count; i++)
    {
        net->runSession(session);
    }
    
    

    // get output data
    std::string output_tensor_name0 = layers[2].name ;
    std::string output_tensor_name1 = layers[1].name ;
    std::string output_tensor_name2 = layers[0].name ;

    MNN::Tensor *tensor_scores  = net->getSessionOutput(session, output_tensor_name0.c_str());
    MNN::Tensor *tensor_boxes   = net->getSessionOutput(session, output_tensor_name1.c_str());
    MNN::Tensor *tensor_anchors = net->getSessionOutput(session, output_tensor_name2.c_str());

    MNN::Tensor tensor_scores_host(tensor_scores, tensor_scores->getDimensionType());
    MNN::Tensor tensor_boxes_host(tensor_boxes, tensor_boxes->getDimensionType());
    MNN::Tensor tensor_anchors_host(tensor_anchors, tensor_anchors->getDimensionType());

    tensor_scores->copyToHostTensor(&tensor_scores_host);
    tensor_boxes->copyToHostTensor(&tensor_boxes_host);
    tensor_anchors->copyToHostTensor(&tensor_anchors_host);

    show_shape(tensor_scores_host.shape());
    show_shape(tensor_boxes_host.shape());
    show_shape(tensor_anchors_host.shape());

    yolocv::YoloSize yolosize = yolocv::YoloSize{INPUT_SIZE, INPUT_SIZE};
    float threshold = 0.3;
    float nms_threshold = 0.7;
    std::vector<BoxInfo> result;
    std::vector<BoxInfo> boxes;
    boxes = decode_infer(tensor_scores_host, layers[2].stride,  yolosize, net_size, num_classes, layers[2].anchors, threshold);
    result.insert(result.begin(), boxes.begin(), boxes.end());

    boxes = decode_infer(tensor_boxes_host, layers[1].stride,  yolosize, net_size, num_classes, layers[1].anchors, threshold);
    result.insert(result.begin(), boxes.begin(), boxes.end());

    boxes = decode_infer(tensor_anchors_host, layers[0].stride,  yolosize, net_size, num_classes, layers[0].anchors, threshold);
    result.insert(result.begin(), boxes.begin(), boxes.end());

    nms(result, nms_threshold);

    std::cout << result.size() << std::endl;

    scale_coords(result, INPUT_SIZE, INPUT_SIZE, originalWidth, originalHeight);
    draw_box(inputImage, result, labels);
    stbi_write_png(outputImageFileName, originalWidth, originalHeight, 4, inputImage, 4 * originalWidth);
    stbi_image_free(inputImage);
    return 0;
}
