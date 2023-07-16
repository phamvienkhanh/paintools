cmake_minimum_required(VERSION 3.16)

project(painttools VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(apppainttools
    main.cpp
)

qt_add_qml_module(apppainttools
    URI painttools
    VERSION 1.0
    QML_FILES
        Main.qml
        tools/TextInputZone.qml
        tools/RectangleSelectZone.qml
    SOURCES
        snimageeditor.h snimageeditor.cpp
        toolfactory.h toolfactory.cpp
        tools/snabstracttool.h tools/snabstracttool.cpp
        tools/snpenciltool.h tools/snpenciltool.cpp
        tools/snlinetool.h tools/snlinetool.cpp
        tools/sncircletool.h tools/sncircletool.cpp
        tools/snrectangletool.h tools/snrectangletool.cpp
        tools/sntexttool.h tools/sntexttool.cpp
        tools/sncroptool.h tools/sncroptool.cpp
)

set_target_properties(apppainttools PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(apppainttools
    PRIVATE Qt6::Quick
)
target_include_directories(apppainttools PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tools)

install(TARGETS apppainttools
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
