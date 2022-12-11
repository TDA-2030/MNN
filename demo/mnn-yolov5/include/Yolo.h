
#ifndef Yolo_H
#define Yolo_H

#include <string>
#include <MNN/Tensor.hpp>

namespace yolocv {
typedef struct {
    int width;
    int height;
} YoloSize;
}

typedef struct {
    std::string name;
    int stride;
    std::vector<yolocv::YoloSize> anchors;
} YoloLayerData;

class BoxInfo {
public:
    int x1, y1, x2, y2, label, id;
    float score;
};

std::vector<BoxInfo>
decode_infer(MNN::Tensor &data, int stride, const yolocv::YoloSize &frame_size, int net_size, int num_classes,
             const std::vector<yolocv::YoloSize> &anchors, float threshold);

void nms(std::vector<BoxInfo> &result, float nms_threshold);

#if 1

inline float fast_exp(float x)
{
    union {
        uint32_t i;
        float f;
    } v{};
    v.i = (1 << 23) * (1.4426950409 * x + 126.93490512f);
    return v.f;
}

inline float sigmoid(float x)
{
    return 1.0f / (1.0f + fast_exp(-x));
}
#else
static inline float sigmoid(float x)
{
    return static_cast<float>(1.f / (1.f + exp(-x)));
}
#endif

#endif //Yolo_H
