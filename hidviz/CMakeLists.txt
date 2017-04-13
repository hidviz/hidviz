cmake_minimum_required(VERSION 3.2)
project(hidviz VERSION 0.0.1 LANGUAGES C CXX)

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(hidviz
        src/main.cc
        src/DeviceSelector.cc
        src/Window.cc
        src/DeviceSelectionListItem.cc
        src/hid/ItemWidget.cc
        src/hid/CollectionWidget.cc
        src/hid/ControlWidget.cc
        src/FlowLayout.cc
        src/DeviceView.cc
        images/images.qrc
        src/hid/UsageWidget.cc src/hid/UsageWidget.hh)

target_link_libraries(hidviz
        hidx
        Qt5::Widgets
        )
target_compile_features(hidviz PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(hidviz PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)
