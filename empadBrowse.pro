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

win32:CONFIG(release, debug|release): LIBS += -LD:\vcpkg\vcpkg\installed\x64-windows\lib
else:win32:CONFIG(debug, debug|release): LIBS += -LD:\vcpkg\vcpkg\installed\x64-windows\debug\lib

win32:CONFIG(release, debug|release): LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_video -lopencv_videoio -lboost_iostreams-vc140-mt -lopencv_ml -lboost_filesystem-vc140-mt -lQt5PrintSupport -lQt5Widgets -lQt5Core -lQt5Gui
else:win32:CONFIG(debug, debug|release): LIBS += -lopencv_cored -lopencv_highguid -lopencv_imgprocd -lopencv_videod -lopencv_videoiod -lboost_iostreams-vc140-mtd -lopencv_mld -lboost_filesystem-vc140-mtd -Qt5PrintSupportd -lQt5Widgetsd -lQt5Cored -lQt5Guid


DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        calcdebounceoffsets.cpp \
        calibrationdialog.cpp \
        empad2lib.cpp \
        eventdetectiondialog.cpp \
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
        calibrationdialog.h \
        empad2lib.h \
        eventdetectiondialog.h \
        histogramareadialog.h \
        histogramdialog.h \
        mainwindow.h \
        moviedialog.h \
        plotpixels.h \
        qcustomplot.h \
        sumframesdialog.h

FORMS += \
        calcdebounceoffsets.ui \
        calibrationdialog.ui \
        eventdetectiondialog.ui \
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


win32:RC_ICONS += EMPADBrowser.ico
