cmake_minimum_required(VERSION 3.5.1)
project(yolov5)


SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
# SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/)
set(TARGET ${CMAKE_PROJECT_NAME})


file(GLOB SOURCE_FILES src/*.cpp)
add_executable(${TARGET} ${SOURCE_FILES})

message(STATUS "Build for ${CMAKE_SYSTEM_PROCESSOR}")
if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "aarch64")
    target_link_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/arm64)
else()
    target_link_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib/x86)
endif()

target_include_directories(${TARGET} PUBLIC
${PROJECT_SOURCE_DIR}/include
/home/zhouli/work/MNN/include 
/home/zhouli/work/MNN/3rd_party/imageHelper)

target_link_libraries (${TARGET}
        MNN_Express
        # ${Boost_LIBRARIES}
        pthread
        MNN
        # ncnn
        # vulkan
)
