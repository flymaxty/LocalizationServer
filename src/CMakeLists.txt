CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

FIND_PACKAGE(OpenCV REQUIRED)

ADD_EXECUTABLE(localizationServer localizationServer.cpp DataCenter.cpp ColorSegmentation.cpp TeamDetect.cpp)
TARGET_LINK_LIBRARIES(localizationServer ${OpenCV_LIBS})

ADD_EXECUTABLE(capture capture.cpp)
TARGET_LINK_LIBRARIES(capture ${OpenCV_LIBS})

ADD_EXECUTABLE(getColor getColor.cpp ColorSegmentation.cpp)
TARGET_LINK_LIBRARIES(getColor ${OpenCV_LIBS})

ADD_EXECUTABLE(param param.cpp)
TARGET_LINK_LIBRARIES(param ${OpenCV_LIBS})