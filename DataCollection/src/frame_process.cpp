#include <iostream>
#include "libobsensor/ObSensor.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <mutex>
#include <thread>
#include "frame_process.hpp"
#include <opencv2/opencv.hpp>
#include <fstream>
#include <memory>  
#include <string>

void color_process(std::shared_ptr<ob::Frame> frame, int height , int width)
{
    void* dataPtr = frame->data();
    uint32_t dataSize = frame->dataSize();
    uint64_t time = frame->timeStampUs();

    std::cout<<time<<std::endl;

    cv::Mat img(height, width, CV_8UC3, dataPtr);
    cv::Mat imgRGB;
    cv::cvtColor(img, imgRGB, cv::COLOR_BGR2RGB);

    // 设置保存路径和文件名
    std::string folderPath = "../../dataset/11_18_data/cam/data/";  // 指定文件夹路径
    std::string fileName = std::to_string(time) + ".png";  // 使用时间戳作为文件名
    std::string filePath = folderPath + fileName;
    if (!cv::imwrite(filePath, imgRGB)) {
        std::cerr << "Failed to save image at: " << filePath << std::endl;
        return;
    }

    std::cout << "Image saved as: " << filePath << std::endl;
    // 将时间戳和文件名写入 CSV 文件
    std::string csvFilePath = "../../dataset/11_18_data/cam/data.csv";
    std::ofstream csvFile;

    // 打开 CSV 文件，使用 append 模式来追加写入
    csvFile.open(csvFilePath, std::ios::out | std::ios::app);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << csvFilePath << std::endl;
        return;
    }

    // 写入时间戳和文件名
    csvFile << time << "," << fileName << std::endl;

    // 关闭 CSV 文件
    csvFile.close();

    std::cout << "Time and filename written to CSV: " << csvFilePath << std::endl;
}

void depth_process()
{
    std::cout << "depth" << std::endl;
}
void ir_process()
{
    std::cout << "ir" << std::endl;
}


std::mutex imuFrameMutex;
std::map<OBFrameType, std::shared_ptr<ob::Frame>> imuFrameMap;
void imu_process(std::shared_ptr<ob::FrameSet> frameset)
{
    std::ofstream csvFile;
    std::string csvFilePath = "../../dataset/11_18_data/imu/data.csv";
    bool fileExists = static_cast<bool>(std::ifstream(csvFilePath));
    csvFile.open(csvFilePath, std::ios::out | std::ios::app);

    if (!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << csvFilePath << std::endl;
        return;
    }

    auto count = frameset->frameCount();
    double w_x = 0.0, w_y = 0.0, w_z = 0.0;            // 陀螺仪数据
    double a_x = 0.0, a_y = 0.0, a_z = 0.0;            // 加速度计数据
    uint64_t gyroTimestamp = 0;
    uint64_t accelTimestamp = 0;
    uint64_t timeStamp = 0;

    for(int i = 0; i < count; i++) 
    {
        auto frame = frameset->getFrame(i);
        
        imuFrameMap[frame->type()] = frame;
        uint64_t timeStamp = frame->timeStampUs(); // 转换为纳秒（1微秒 = 1000纳秒）
        imuFrameMutex.lock();
        if (frame->type() == OB_FRAME_GYRO)
        {
            uint64_t timeStamp = frame->timeStampUs();
            auto gyroFrame = frame->as<ob::GyroFrame>();
            auto value = gyroFrame->value();
            w_x = value.x;
            w_y = value.y;
            w_z = value.z;
            gyroTimestamp = frame->timeStampUs();
        }

        if (frame->type() == OB_FRAME_ACCEL)
        {
            uint64_t timeStamp = frame->timeStampUs();
            auto accelFrame = frame->as<ob::AccelFrame>();
            auto value = accelFrame->value();
            a_x = value.x;
            a_y = value.y;
            a_z = value.z;
            accelTimestamp = frame->timeStampUs();
        }
        if (gyroTimestamp == accelTimestamp && gyroTimestamp != 0)
        {
            csvFile << timeStamp << ","
            << w_x << "," << w_y << "," << w_z << ","
            << a_x << "," << a_y << "," << a_z << std::endl;
        }
        imuFrameMutex.unlock();

    }
    csvFile.close();
}