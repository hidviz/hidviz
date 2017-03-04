cmake_minimum_required(VERSION 3.6)
project(hidviz)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cc TreeItem.cc TreeModel.cc Window.cc Window.hh DeviceSelector.cc DeviceSelector.hh)
add_executable(hidviz ${SOURCE_FILES})
target_link_libraries(hidviz Qt5::Widgets)
