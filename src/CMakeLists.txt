CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(OpenCV REQUIRED)

ADD_EXECUTABLE(LocalizationServer LocalizationServer.cpp DataCenter.cpp ColorSegmentation.cpp TeamDetect.cpp Image2World.cpp Broadcaster.cpp cJSON/cJSON.c)
TARGET_LINK_LIBRARIES(LocalizationServer ${OpenCV_LIBS} mosquittopp pthread)

ADD_EXECUTABLE(GetField GetField.cpp DataCenter.cpp ColorSegmentation.cpp TeamDetect.cpp Image2World.cpp)
TARGET_LINK_LIBRARIES(GetField ${OpenCV_LIBS})

ADD_EXECUTABLE(GetColor GetColor.cpp ColorSegmentation.cpp DataCenter.cpp)
TARGET_LINK_LIBRARIES(GetColor ${OpenCV_LIBS})

ADD_EXECUTABLE(Calibration Calibration.cpp)
TARGET_LINK_LIBRARIES(Calibration ${OpenCV_LIBS})

ADD_EXECUTABLE(Broadcaster_test Broadcaster_test.cpp Broadcaster.cpp cJSON/cJSON.c)
TARGET_LINK_LIBRARIES(Broadcaster_test mosquittopp)