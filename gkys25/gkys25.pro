QT       += core gui widgets
CONFIG += c++11

SOURCES += \
    CustomHeaderView.cpp \
    DeviceTableModel.cpp \
    FullScreenDialog.cpp \
    TextItemDelegate.cpp \
    VideoFrame.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    CustomHeaderView.h \
    DeviceTableModel.h \
    FullScreenDialog.h \
    MainWindow.h \
    TextItemDelegate.h \
    VideoFrame.h

FORMS += \
    FullScreenDialog.ui \
    MainWindow.ui



DESTDIR =$$PWD/bin64
UI_DIR = $$PWD/tmp/release/ui
MOC_DIR = $$PWD/tmp/release/moc
OBJECTS_DIR = $$PWD/tmp/release/obj
RCC_DIR = $$PWD/tmp/release/rcc

INCLUDEPATH += $$PWD/third/ezpc/include
INCLUDEPATH += $$PWD/third/curl/include
LIBS += -L$$PWD/third/ezpc/lib/x64 -lOpenNetStream
LIBS += -L$$PWD/third/curl/lib/x64 -llibcurl_imp
LIBS += -luser32

RESOURCES += \
    image.qrc

