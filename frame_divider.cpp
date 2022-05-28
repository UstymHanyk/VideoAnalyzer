#include "frame_divider.h"

void divide_on_frames(cv::VideoCapture &cap, const int inBetweenFrameTime, oneapi::tbb::concurrent_bounded_queue<cv::Mat>& frames) {

    int frameCount = 0;
    cv::Mat frame;

    while (true) {
        cap.read(frame);

        if (!frame.empty()) {
            frameCount += inBetweenFrameTime;

            cap.set(cv::CAP_PROP_POS_FRAMES, frameCount);
            resize(frame, frame, cv::Size(256, 144), cv::INTER_LINEAR);

            frames.push(frame);
        }
        else {
            frames.push(frame);
            break;
        }
    }

    std::cout << "divider_thread_ended\n";

}