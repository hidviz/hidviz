cmake_minimum_required(VERSION 3.6)
project(hidviz)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(PkgConfig)
pkg_check_modules(LIBUSB libusb-1.0)


include_directories(${LIBUSB_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(SOURCE_FILES main.cc TreeItem.cc TreeModel.cc Window.cc Window.hh DeviceSelector.cc DeviceSelector.hh HidLib.cc HidLib.hh HidLibDevice.cc HidLibDevice.hh HidLibDeviceHandle.cc HidLibDeviceHandle.hh HidLibDeviceStrings.hh)
add_executable(hidviz ${SOURCE_FILES})
target_link_libraries(hidviz Qt5::Widgets ${LIBUSB_LIBRARIES})
