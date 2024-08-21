cmake_minimum_required(VERSION 3.16)

project(rayboxclient VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FLUENTUI_BUILD_EXAMPLES OFF)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Widgets Quick Sql)


add_subdirectory(src)
add_subdirectory(FluentUI)

include(GNUInstallDirs)
install(TARGETS apprayboxclient
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
