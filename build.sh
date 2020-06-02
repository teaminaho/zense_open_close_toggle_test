#!/bin/bash

PICOZENSE_INSTALL_DIR="/usr/local/PicoZenseSDK"
FLAGS="-I${PICOZENSE_INSTALL_DIR}/Include -I${PICOZENSE_INSTALL_DIR}/Thirdparty/opencv-3.4.1/include -Wl,-rpath,./:../../Lib/x64/:../../Thirdparty/opencv-3.4.1/lib/x64:../../Thirdparty/ffmpeg/lib/x64 -Wall -Wconversion -O3 -L../../Lib/x64 -lpicozense_api -L$(PICOZENSE_INSTALL_DIR)/Thirdparty/opencv-3.4.1/lib/x64 -lopencv_core -lopencv_highgui -lopencv_imgproc"
g++ -std=c++11 -o openCloseToggle openCloseToggle.cpp ${FLAGS}