cmake_minimum_required(VERSION 3.6)
project(hidviz VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_modules/")


# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Widgets)

find_package(libusb-1.0 REQUIRED)
add_library(libusb-1.0 SHARED IMPORTED)
set_property(TARGET libusb-1.0 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBUSB_1_INCLUDE_DIRS})
set_property(TARGET libusb-1.0 PROPERTY IMPORTED_LOCATION ${LIBUSB_1_LIBRARY})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_subdirectory(libhidx)
add_subdirectory(hidviz)
