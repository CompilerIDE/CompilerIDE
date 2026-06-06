cmake_minimum_required(VERSION 3.21)
project(CompilerIDE_Open_source VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core Gui Widgets
    Pdf PdfWidgets Svg SvgWidgets
    Network Concurrent Multimedia MultimediaWidgets DBus
    Charts WebEngineWidgets
    Qml Quick
)

set(SOURCES main.cpp mainwindow.cpp)
set(HEADERS mainwindow.h)
set(FORMS mainwindow.ui)
set(RESOURCES resources.qrc)

if(WIN32)
    add_executable(CompilerIDE_Open_source WIN32 ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES}
    )
else()
    add_executable(CompilerIDE_Open_source ${SOURCES} ${HEADERS} ${FORMS} ${RESOURCES})
endif()

target_link_libraries(CompilerIDE_Open_source PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets
    Qt6::Pdf Qt6::PdfWidgets Qt6::Svg Qt6::SvgWidgets
    Qt6::Network Qt6::Concurrent Qt6::Multimedia Qt6::MultimediaWidgets
    Qt6::DBus Qt6::Charts Qt6::WebEngineWidgets
    Qt6::Qml Qt6::Quick Qt6::WebChannel
)

target_compile_definitions(CompilerIDE_Open_source PRIVATE QT_CHARTS_USE_NAMESPACE)

if(WIN32)
    target_link_libraries(CompilerIDE_Open_source PRIVATE
        kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32 dwmapi
    )
endif()