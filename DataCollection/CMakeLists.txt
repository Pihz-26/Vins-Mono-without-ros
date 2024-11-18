cmake_minimum_required(VERSION 3.1.15)
project(OrbbecSDKTest)

add_executable("data_collection" src/main.cpp src/frame_process.cpp)

# find Orbbec SDK
set(OrbbecSDK_DIR "/workspace/thirdparty/OrbbecSDK")
find_package(OrbbecSDK  REQUIRED)
find_package(OpenCV     REQUIRED)


target_include_directories("data_collection" PUBLIC ${CMAKE_SOURCE_DIR}/include)

# link Orbbec SDK
target_link_libraries("data_collection" OrbbecSDK::OrbbecSDK ${OpenCV_LIBS})
