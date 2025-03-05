QT       += core gui widgets
CONFIG += c++11

SOURCES += \
    CustomHeaderView.cpp \
    DeviceTableModel.cpp \
    FullScreenDialog.cpp \
    TextItemDelegate.cpp \
    VideoFrame.cpp \
    json/json_reader.cpp \
    json/json_value.cpp \
    json/json_writer.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    CustomHeaderView.h \
    DeviceTableModel.h \
    FullScreenDialog.h \
    MainWindow.h \
    TextItemDelegate.h \
    VideoFrame.h \
    json/autolink.h \
    json/config.h \
    json/features.h \
    json/forwards.h \
    json/json.h \
    json/json_batchallocator.h \
    json/json_internalarray.inl \
    json/json_internalmap.inl \
    json/json_valueiterator.inl \
    json/reader.h \
    json/value.h \
    json/writer.h

FORMS += \
    FullScreenDialog.ui \
    MainWindow.ui


RC_ICONS = main.ico
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

