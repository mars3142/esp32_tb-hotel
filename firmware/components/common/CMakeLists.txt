cmake_minimum_required(VERSION 3.30)

if (SDKCONFIG)
    message(STATUS "Building for ESP32")

    idf_component_register(SRCS
            INCLUDE_DIRS "include"
    )
else ()
    message(STATUS "Building for Desktop")

    project(common VERSION 1.0.0 DESCRIPTION "Common Interface for Geocache")

    add_library(common SHARED
            wled.cpp
    )
    add_library(GC::COMMON ALIAS common)

    set_target_properties(common PROPERTIES VERSION ${PROJECT_VERSION})
    target_include_directories(common PUBLIC include)
endif ()
