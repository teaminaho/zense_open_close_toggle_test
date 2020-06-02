#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <opencv2/opencv.hpp>
#include "PicoZense_api.h"

static const int32_t DEVICE_INDEX = 0;
static const uint32_t SLEEP_MS = 33;
static const uint32_t FPS_MEASURE_INTERVAL = 30 * 1;
static const uint32_t TOGGLE_OPEN_INTERVAL = 30 * 10;

using namespace std;
using time_point = chrono::system_clock::time_point;

static void shutdown(int32_t devinceIndex, int exit_status) {
    PsCloseDevice(devinceIndex);
    PsShutdown();
    exit(exit_status);
}

static void update(PsFrame* frame) {
    PsReturnStatus status = PsReadNextFrame(DEVICE_INDEX);
    if (status == PsReturnStatus::PsRetOK) {
        PsGetFrame(DEVICE_INDEX, PsWDRDepthFrame, frame);
    }
}

double duration_ms(time_point& t1, time_point& t2) {
    double dt = static_cast<double>(
        chrono::duration_cast<chrono::microseconds>(t2 - t1).count()) / 1000.0;
    return dt;
}

time_point show_fps(uint32_t loop, time_point& t_last) {
    if (loop % FPS_MEASURE_INTERVAL == 0) {
        time_point t_now = chrono::system_clock::now();
        double fps = FPS_MEASURE_INTERVAL / (duration_ms(t_last, t_now) / 1000);
        cout << fps << " [FPS]" << endl;
        return chrono::system_clock::now();
    }
    return t_last;
}

static void open_device(int32_t index) {
    PsReturnStatus status = PsOpenDevice(index);
    if (status != PsReturnStatus::PsRetOK) {
        cout << "OpenDevice failed!" << endl;
        exit(EXIT_FAILURE);
    }

    // Set data mode to PsWDR_Depth, WDR depth frame only output in this mode
    PsSetDataMode(index, PsWDR_Depth);

    // Set WDR Output Mode, two ranges Near(Range0)/Far(Range2) output from device every one frame,
    // no care for range3 and range3Count in PsWDRTotalRange_Two
    PsWDROutputMode wdrMode = { PsWDRTotalRange_Two, PsNearRange, 1, PsFarRange, 1, PsNearRange, 1 };
    PsSetWDROutputMode(index, &wdrMode);

    // Set WDR fusion threshold, no care for threshold2 in PsWDRTotalRange_Two
    PsSetWDRFusionThreshold(index, 1000, 2500);
}

static void close_device(int32_t index) {
    PsReturnStatus status = PsCloseDevice(index);
    if (status != PsReturnStatus::PsRetOK) {
        cout << "CloseDevice failed!" << endl;
        exit(EXIT_FAILURE);
    }
}

static bool toggle_open_device(int32_t index, bool opened) {
    if (opened) {
        close_device(DEVICE_INDEX);
        cout << "closed!" << endl;
    } else {
        open_device(DEVICE_INDEX);
        cout << "opened!" << endl;
    }
    return !opened;
}

int main(int argc, char *argv[]) {
    PsFrame frame;
    cv::Mat mat;
    time_point t_last;
    bool opened = false;

    PsInitialize();

    t_last = chrono::system_clock::now();

    for (uint32_t loop = 1;; loop++) {
        if (loop % TOGGLE_OPEN_INTERVAL == 0) {
            opened = toggle_open_device(DEVICE_INDEX, opened);
        }

        if (opened) {
            update(&frame);
        } else {
            this_thread::sleep_for(chrono::milliseconds(SLEEP_MS));
        }

        t_last = show_fps(loop, t_last);
    }

    shutdown(DEVICE_INDEX, EXIT_SUCCESS);
    return 0;
}
