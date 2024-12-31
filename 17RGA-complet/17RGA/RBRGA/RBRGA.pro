QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -Wa,-mbig-obj
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): CONFIG += c++11
lessThan(QT_MAJOR_VERSION, 5): QMAKE_CXXFLAGS += -std=c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 优化设置（可选）
CONFIG += release
QMAKE_CXXFLAGS_RELEASE += -O3 -g0  # 优化并禁用调试信息

QT += core network

SOURCES += \
    amu_chart.cpp \
    api.cpp \
    chart.cpp \
    create_signal_item_dialog.cpp \
    home.cpp \
    leak_mode_page.cpp \
    main.cpp \
    qcustomplot.cpp \
    rb_rga.cpp \
    rga_display.cpp \
    rga_item.cpp \
    set_communication_dialog.cpp \
    singal_chart_item.cpp \
    sweep_mode_page.cpp

HEADERS += \
    API_Communication.h \
    API_DigitalOutput.h \
    API_ScanInfo.h \
    API_ScanSetup.h \
    API_SensorFilter.h \
    API_Sensor_Information.h \
    API_Status.h \
    amu_chart.h \
    api.h \
    chart.h \
    create_signal_item_dialog.h \
    home.h \
    leak_mode_page.h \
    qcustomplot.h \
    rb_rga.h \
    rga_display.h \
    rga_item.h \
    set_communication_dialog.h \
    singal_chart_item.h \
    sweep_mode_page.h

FORMS += \
    amu_chart.ui \
    api.ui \
    chart.ui \
    create_signal_item_dialog.ui \
    home.ui \
    leak_mode_page.ui \
    rb_rga.ui \
    rga_display.ui \
    rga_item.ui \
    set_communication_dialog.ui \
    singal_chart_item.ui \
    sweep_mode_page.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icon.qrc
