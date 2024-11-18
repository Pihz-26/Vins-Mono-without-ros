#include <iostream>
#include "libobsensor/ObSensor.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <mutex>
#include <thread>
#include "frame_process.hpp"


OBStreamType SensorTypeToStreamType(OBSensorType sensorType) {
    switch(sensorType) {
    case OB_SENSOR_COLOR:
        return OB_STREAM_COLOR;
    case OB_SENSOR_DEPTH:
        return OB_STREAM_DEPTH;
    case OB_SENSOR_IR:
        return OB_STREAM_IR;
    case OB_SENSOR_IR_LEFT:
        return OB_STREAM_IR_LEFT;
    case OB_SENSOR_IR_RIGHT:
        return OB_STREAM_IR_RIGHT;
    case OB_SENSOR_GYRO:
        return OB_STREAM_GYRO;
    case OB_SENSOR_ACCEL:
        return OB_STREAM_ACCEL;
    default:
        return OB_STREAM_UNKNOWN;
    }
}

// 定义转换函数
std::string FrameTypeToString(OBFrameType type) {
    switch (type) {
        case OB_FRAME_UNKNOWN:   return "Unknown frame type";
        case OB_FRAME_VIDEO:     return "Video frame";
        case OB_FRAME_IR:        return "IR frame";
        case OB_FRAME_COLOR:     return "Color frame";
        case OB_FRAME_DEPTH:     return "Depth frame";
        case OB_FRAME_ACCEL:     return "Accelerometer data frame";
        case OB_FRAME_SET:       return "Frame collection";
        case OB_FRAME_POINTS:    return "Point cloud frame";
        case OB_FRAME_GYRO:      return "Gyroscope data frame";
        case OB_FRAME_IR_LEFT:   return "Left IR frame";
        case OB_FRAME_IR_RIGHT:  return "Right IR frame";
        case OB_FRAME_RAW_PHASE: return "Raw phase frame";
        default:                 return "Invalid frame type";
    }
}

ob::Context ctx;

int main()
{
    ctx.setDeviceChangedCallback([](std::shared_ptr<ob::DeviceList> removedList, std::shared_ptr<ob::DeviceList> addedList) {});
    std::shared_ptr<ob::DeviceList> devices = ctx.queryDeviceList();
    auto device = devices->getDevice(0);
    auto deviceInfo = device->getDeviceInfo();
    // 创建 Pipeline
    std::shared_ptr<ob::Pipeline> pipe(new ob::Pipeline(device));
    std::shared_ptr<ob::Config> config = std::make_shared<ob::Config>();

    // // 深度图像设置
    // auto depth_profiles = pipe->getStreamProfileList(OB_SENSOR_DEPTH);
    // std::shared_ptr<ob::VideoStreamProfile> depthProfile = nullptr;
    // depthProfile = depth_profiles->getVideoStreamProfile(640, 400, OB_FORMAT_Y16, 30);
    // // 导入depth's config
    // config->enableStream(depthProfile);

    // 彩色图像设置
    auto color_profiles = pipe->getStreamProfileList(OB_SENSOR_COLOR);
    std::shared_ptr<ob::VideoStreamProfile> colorProfile = nullptr;
    colorProfile = color_profiles->getVideoStreamProfile(640, 400, OB_FORMAT_RGB888, 30);
    // 导入color's config
    config->enableStream(colorProfile);


    // // 双目图像设置
    // auto ir_profiles = pipe->getStreamProfileList(OB_SENSOR_IR);
    // std::shared_ptr<ob::VideoStreamProfile> irProfile = nullptr;
    // irProfile = ir_profiles->getVideoStreamProfile(640, 400, OB_FORMAT_Y8, 30);
    // // 导入ir's config
    // config->enableStream(irProfile);


    // 开启相机的回调
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::mutex  frameMutex;
    std::map<OBFrameType, std::shared_ptr<ob::Frame>> frameMap;
    pipe->start(config, [&](std::shared_ptr<ob::FrameSet> frameset) {
        auto count = frameset->frameCount();
        for(int i = 0; i < count; i++) {
            auto frame = frameset->getFrame(i);
            std::unique_lock<std::mutex> lk(frameMutex);
            frameMap[frame->type()] = frame;
            switch (frame->type())
            {
            case OB_FRAME_COLOR:
                color_process(frame, 400, 640);
                break;
            
            default:
                break;
            }
            

        }
    });

    

    // 陀螺仪(Gyro)设置
    // 对于IMU数据单独开启一个pipeline进行接收
    auto  dev         = pipe->getDevice();
    auto  imuPipeline = std::make_shared<ob::Pipeline>(dev);
    std::shared_ptr<ob::Config> imuConfig = std::make_shared<ob::Config>();
    

    auto gyroSensor = dev->getSensor(OB_SENSOR_GYRO);
    auto gyro_profiles = imuPipeline->getStreamProfileList(OB_SENSOR_GYRO);
    std::shared_ptr<ob::GyroStreamProfile> gyroProfile = nullptr;
    gyroProfile = gyro_profiles->getGyroStreamProfile(OB_GYRO_FS_1000dps, OB_SAMPLE_RATE_200_HZ);
    
    // 加速度计(Accel)设置
    auto accelSensor = dev->getSensor(OB_SENSOR_ACCEL);
    auto accel_profiles = imuPipeline->getStreamProfileList(OB_SENSOR_ACCEL);
    std::shared_ptr<ob::AccelStreamProfile> accelProfile = nullptr;
    accelProfile = accel_profiles->getAccelStreamProfile(OB_ACCEL_FS_UNKNOWN, OB_SAMPLE_RATE_200_HZ);

    // 开流
    imuConfig->enableStream(gyroProfile);
    imuConfig->enableStream(accelProfile);
    std::this_thread::sleep_for(std::chrono::seconds(2));
    imuPipeline->start(imuConfig, [&](std::shared_ptr<ob::FrameSet> frameset) {
        imu_process(frameset);
        });


    // 开始接受数据
    while (true)
    {   
        // auto frameSet = pipe->waitForFrames(100);
        // if (frameSet == nullptr)
        // {
        //     continue;
        // }

        // auto colorFrame = frameSet->colorFrame();
        // auto depthFrame = frameSet->depthFrame();
        // auto irFrame = frameSet->irFrame();
        
        // // 对于各种帧进行存储
        // if (colorFrame)
        // {
        //     color_process();
        // }
        // if (depthFrame)
        // {
        //     depth_process();
        // }
        // if (irFrame)
        // {
        //     ir_process();
        // }
    }
    // Stop the Pipeline, no frame data will be generated
    pipe->stop();

    return 0;
}