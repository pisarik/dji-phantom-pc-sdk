#-------------------------------------------------
#
# Project created by QtCreator 2016-11-08T12:35:44
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DJI_PC_CONTROLLER
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        videogetter.cpp

HEADERS  += mainwindow.h \
         videogetter.h

FORMS    += mainwindow.ui

#INCLUDEPATH += D:/Projects/Git/ffmpeg/

INCLUDEPATH += D:/Libs/opencv-3.1/build2013_x64/install/include/

CONFIG(release, debug|release){
# LIBS += "D:/Libs/opencv-3.1/build2013_x64/install/x64/vc12/bin/*.dll"
 LIBS += -LD:/Libs/opencv-3.1/build2013_x64/install/x64/vc12/lib  \
        -lopencv_cudabgsegm310 \
        -lopencv_cudaobjdetect310 \
        -lopencv_cudastereo310 \
        -lopencv_shape310 \
        #-lopencv_stitching310 \
        -lopencv_cudafeatures2d310 \
        -lopencv_superres310 \
        -lopencv_cudacodec310 \
        -lopencv_videostab310 \
        #-lippicv \
        -lopencv_cudaoptflow310 \
        -lopencv_cudalegacy310 \
        -lopencv_calib3d310 \
        -lopencv_features2d310 \
        -lopencv_objdetect310 \
        -lopencv_highgui310 \
        -lopencv_videoio310 \
        -lopencv_photo310 \
        -lopencv_imgcodecs310 \
        -lopencv_cudawarping310 \
        -lopencv_cudaimgproc310 \
        -lopencv_cudafilters310 \
        -lopencv_video310 \
        -lopencv_ml310 \
        -lopencv_imgproc310 \
        -lopencv_flann310 \
        -lopencv_cudaarithm310 \
        -lopencv_core310 \
        -lopencv_cudev310 \
        -lopencv_xfeatures2d310
}
else:CONFIG(debug, debug|release){
# LIBS += "D:/Libs/opencv-3.1/build2013_x64/install/x64/vc12/bin/*d.dll"
 LIBS += -LD:/Libs/opencv-3.1/build2013_x64/install/x64/vc12/lib  \
        -lopencv_cudabgsegm310d \
        -lopencv_cudaobjdetect310d \
        -lopencv_cudastereo310d \
        -lopencv_shape310d \
        #-lopencv_stitching310d \
        -lopencv_cudafeatures2d310d \
        -lopencv_superres310d \
        -lopencv_cudacodec310d \
        -lopencv_videostab310d \
        #-lippicv \
        -lopencv_cudaoptflow310d \
        -lopencv_cudalegacy310d \
        -lopencv_calib3d310d \
        -lopencv_features2d310d \
        -lopencv_objdetect310d \
        -lopencv_highgui310d \
        -lopencv_videoio310d \
        -lopencv_photo310d \
        -lopencv_imgcodecs310d \
        -lopencv_cudawarping310d \
        -lopencv_cudaimgproc310d \
        -lopencv_cudafilters310d \
        -lopencv_video310d \
        -lopencv_ml310d \
        -lopencv_imgproc310d \
        -lopencv_flann310d \
        -lopencv_cudaarithm310d \
        -lopencv_core310d \
        -lopencv_cudev310d \
        -lopencv_xfeatures2d310d
}
