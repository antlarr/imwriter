cmake_minimum_required(VERSION 2.8)

project(imwriter)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Set version
set(IMWRITER_VERSION_MAJOR 0)
set(IMWRITER_VERSION_MINOR 13)
set(IMWRITER_VERSION_PATCH 0)
set(IMWRITER_VERSION_STRING "${IMWRITER_VERSION_MAJOR}.${IMWRITER_VERSION_MINOR}.${IMWRITER_VERSION_PATCH}")

# Set up packaging
set(CPACK_PACKAGE_NAME "imwriter")
set(CPACK_PACKAGE_VERSION "${IMWRITER_VERSION_STRING}")
set(CPACK_GENERATOR "TGZ")
set(CPACK_SET_DESTDIR FALSE)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
set(CPACK_SOURCE_IGNORE_FILES "/build/;/.git;/*.user;/.tx/;~$;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
add_custom_target(dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

# Definitions
add_definitions(-Wall -std=c++11 -DQT_NO_CAST_FROM_ASCII -DQT_NO_CAST_FROM_BYTEARRAY)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Handle build type
if(CMAKE_BUILD_TYPE MATCHES [Dd]ebug)
    message(STATUS "Debug build")
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    message(STATUS "Release build")
    add_definitions(-DNDEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Standard installation paths
include(GNUInstallDirs)
find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)


# Macros
include(FeatureSummary)

# PKG-CONFIG
find_package(PkgConfig)

# Qt 5
find_package(Qt5 REQUIRED COMPONENTS Core Widgets DBus LinguistTools)
find_package(KF5 REQUIRED COMPONENTS CoreAddons Solid XmlGui I18n Gpgmepp)

# find qt5 imports dir
get_target_property(QMAKE_EXECUTABLE Qt5::qmake LOCATION)
if(NOT QT_IMPORTS_DIR)
    exec_program(${QMAKE_EXECUTABLE} ARGS "-query QT_INSTALL_QML" RETURN_VALUE return_code OUTPUT_VARIABLE QT_IMPORTS_DIR)
endif()

# Add subdirectories
add_subdirectory(src)

# Display feature summary
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
