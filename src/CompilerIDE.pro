QT += core gui pdf pdfwidgets svg svgwidgets network concurrent multimedia multimediawidgets dbus charts webenginewidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RESOURCES += resources.qrc

DEFINES += QT_CHARTS_USE_NAMESPACE

# 这里建议实际运行时删除，我在配置路径时把电脑弄炸了，只有加这个才能让程序正常运行
QT_INSTALL_PREFIX = "D:/Qt/6.11.0/msvc2022_64"
QT_INSTALL_BINS = $$QT_INSTALL_PREFIX/bin
QT_INSTALL_PLUGINS = $$QT_INSTALL_PREFIX/plugins
QMAKE_LIBDIR += $$QT_INSTALL_PREFIX/lib
QMAKE_INCDIR += $$QT_INSTALL_PREFIX/include

CONFIG += c++17
CONFIG -= console

win32 {
    CONFIG -= console
    CONFIG += windows
    LIBS += -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32
}
win32: LIBS += -ldwmapi

SOURCES += main.cpp mainwindow.cpp
HEADERS += mainwindow.h
FORMS += mainwindow.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

STATECHARTS += resources.scxml
