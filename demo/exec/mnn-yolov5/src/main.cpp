#include <iostream>
#include <string>
#include <stdio.h>

#include "loguru.hpp"
#include "detector.h"
#include "clipp.h"
using namespace clipp;

int main(int argc, char *argv[])
{
    if (argc < 4) {
        std::cout << "Usage: ./yolov5.out model.mnn class.names input.jpg out.jpg" << std::endl;
        return -1;
    }

    // bool rec = false, utf16 = false;
    // string infile = "", fmt = "csv";

    // auto cli = (
    //                value("input file", infile),
    //                option("-r", "--recursive").set(rec).doc("convert files recursively"),
    //                option("-o") & value("output format", fmt),
    //                option("-utf16").set(utf16).doc("use UTF-16 encoding")
    //            );

    // if (!parse(argc, argv, cli)) {
    //     cout << make_man_page(cli, argv[0]);
    //     return -1;
    // }

    loguru::init(argc, argv);

    const auto model_path           = argv[1];
    const auto cls_names_path       = argv[2];
    const auto inputImageFileName  = argv[3];
    const auto outputImageFileName = argv[4];

    Config config = {
        .confThreshold = 0.25f,
        .nmsThreshold = 0.45f,
        .weightPath = model_path,
        .classNamePath = cls_names_path,
        .size = Size(640, 640),
        ._auto = false
    };
    LOG_F(INFO, "Start main process");
    Detector detector(config);
    LOG_F(INFO, "Load model done ..");
    LOG_F(INFO, "Read image from %s", inputImageFileName);
    Detection detection = detector.detect(inputImageFileName);
    LOG_F(INFO, "Detect process finished");
    Colors cl = Colors();
    detector.postProcess(detection, cl);

    return 0;
}
