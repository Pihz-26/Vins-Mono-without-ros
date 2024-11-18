#include <iostream>
#include "libobsensor/ObSensor.hpp"

#include "libobsensor/hpp/Pipeline.hpp"
#include "libobsensor/hpp/Error.hpp"
#include <mutex>
#include <thread>

void color_process(std::shared_ptr<ob::Frame> frame, int height , int width);
void depth_process();
void ir_process();
void imu_process(std::shared_ptr<ob::FrameSet> frameset);
