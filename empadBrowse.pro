#-------------------------------------------------
#
# Project created by QtCreator 2019-07-20T18:02:28
#
#-------------------------------------------------

QT       += core gui charts
CONFIG += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = empadBrowse
TEMPLATE = app
INCLUDEPATH += "D:\boost\boost_1_70_0"
LIBS += -L"D:\boost\boost_1_70_0\stage\lib"

#changing to new build 2019_07_31
LIBS += -L"D:\opencv\sources\build\install\x64\vc16\lib"
INCLUDEPATH += "D:\opencv\sources\build\install\include"
#LIBS += -L"D:\opencv\opencv_2019_07_30\opencv\build_release\install\x64\vc16\lib"
#INCLUDEPATH += "D:\opencv\opencv_2019_07_30\opencv\build_release\install\include"


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        calcdebounceoffsets.cpp \
        empad2lib.cpp \
        histogramareadialog.cpp \
        histogramdialog.cpp \
        main.cpp \
        mainwindow.cpp \
        moviedialog.cpp \
        plotpixels.cpp \
        qcustomplot.cpp \
        sumframesdialog.cpp

HEADERS += \
        calcdebounceoffsets.h \
        empad2lib.h \
        histogramareadialog.h \
        histogramdialog.h \
        mainwindow.h \
        moviedialog.h \
        plotpixels.h \
        qcustomplot.h \
        sumframesdialog.h

FORMS += \
        calcdebounceoffsets.ui \
        histogramareadialog.ui \
        histogramdialog.ui \
        mainwindow.ui \
        moviedialog.ui \
        plotpixels.ui \
        sumframesdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(debug, debug|release): LIBS += -LD:/boost/boost_1_70_0/stage/lib/ -llibboost_iostreams-vc142-mt-gd-x64-1_70
else:win32:CONFIG(release, debug|release): LIBS += -LD:/boost/boost_1_70_0/stage/lib/ -llibboost_iostreams-vc142-mt-x64-1_70

INCLUDEPATH += D:/boost/boost_1_70_0/stage
DEPENDPATH += D:/boost/boost_1_70_0/stage

#win32:!win32-g++: PRE_TARGETDEPS += D:/boost/boost_1_70_0/stage/lib/libboost_iostreams-vc142-mt-gd-x64-1_70.lib
#else:win32-g++: PRE_TARGETDEPS += D:/boost/boost_1_70_0/stage/lib/liblibboost_iostreams-vc142-mt-gd-x64-1_70.a
#win32:!win32-g++: PRE_TARGETDEPS += D:/boost/boost_1_70_0/stage/lib/libboost_iostreams-vc142-mt-x64-1_70.lib
#else:win32-g++: PRE_TARGETDEPS += D:/boost/boost_1_70_0/stage/lib/liblibboost_iostreams-vc142-mt-x64-1_70.a

#win32: LIBS += -LD:/opencv/sources/build/install/x64/vc16/lib/ -lopencv_core410d -lopencv_highgui410d -lopencv_ml410d

#INCLUDEPATH += D:/opencv/sources/build/install/include
#DEPENDPATH += D:/opencv/sources/build/install/include

#win32:!win32-g++: PRE_TARGETDEPS += D:/opencv/sources/build/install/x64/vc16/lib/opencv_core410d.lib
#else:win32-g++: PRE_TARGETDEPS += D:/opencv/sources/build/install/x64/vc16/lib/libopencv_core410d.a


#############################modifying for new opencv build
win32:CONFIG(release, debug|release): LIBS += -LD:/opencv/sources/build_release/install/x64/vc16/lib/ -lopencv_core410 -lopencv_highgui410 -lopencv_ml410 -lopencv_videoio410 -lopencv_imgcodecs410 -lopencv_imgproc410
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv/sources/build_release/install/x64/vc16/lib/ -lopencv_core410d -lopencv_highgui410d -lopencv_ml410d -lopencv_videoio410d -lopencv_imgcodecs410d -lopencv_imgproc410d

INCLUDEPATH += D:/opencv/sources/build_release/install/include
DEPENDPATH += D:/opencv/sources/build_release/install/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/opencv/sources/build_release/install/x64/vc16/lib/libopencv_core410.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/opencv/sources/build_release/install/x64/vc16/lib/libopencv_core410d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/opencv/sources/build_release/install/x64/vc16/lib/opencv_core410.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/opencv/sources/build_release/install/x64/vc16/lib/opencv_core410d.lib

################################

#win32:CONFIG(release, debug|release): LIBS += -LD:/opencv/opencv_2019_07_30/opencv/build_release/install/x64/vc16/lib/ -lopencv_core411 -lopencv_highgui411 -lopencv_ml411
#else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv/opencv_2019_07_30/opencv/build_release/install/x64/vc16/lib/ -lopencv_core411d -lopencv_highgui411d -lopencv_ml411d

#INCLUDEPATH += D:/opencv/opencv_2019_07_30/opencv/build_release/install/include
#DEPENDPATH += D:/opencv/opencv_2019_07_30/opencv/build_release/install/include

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/opencv/opencv_2019_07_30/opencv/build_release/install/x64/vc16/lib/libopencv_core411.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/opencv/opencv_2019_07_30/opencv/build_release/install/x64/vc16/lib/libopencv_core411d.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += D:/opencv/opencv_2019_07_30/opencv/build_release/install/x64/vc16/lib/opencv_core411.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += D:/opencv/opencv_2019_07_30/opencv/build_release/install/x64/vc16/lib/opencv_core411d.lib


win32:RC_ICONS += EMPADBrowser.ico
