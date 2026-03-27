cmake_minimum_required(VERSION 3.16)

project(qt_qml_weather_station VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Network REQUIRED)

qt_add_executable(appqt_qml_weather_station
    main.cpp
)

qt_add_qml_module(appqt_qml_weather_station
    URI qt_qml_weather_station
    VERSION 1.0
    QML_FILES main.qml settings.qml
    SOURCES weather_station.h weather_station.cpp app_settings.h app_settings.cpp
)

set_target_properties(appqt_qml_weather_station PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appqt_qml_weather_station
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqt_qml_weather_station
    PRIVATE Qt6::Quick Qt6::Network)

include(GNUInstallDirs)
install(TARGETS appqt_qml_weather_station
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
