cmake_minimum_required(VERSION 3.2)
project(hidviz VERSION 0.0.1 LANGUAGES C CXX)

add_subdirectory(libhidx)

# Find the QtWidgets library
find_package(Qt5 REQUIRED COMPONENTS Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(hidviz
        hidviz/src/main.cc
        hidviz/src/DeviceSelector.cc
        hidviz/src/Window.cc
        hidviz/src/DeviceSelectionListItem.cc
        hidviz/src/hid/Item.cc
        hidviz/src/hid/Collection.cc
        hidviz/src/hid/Control.cc
        hidviz/src/FlowLayout.cc
        hidviz/src/DeviceView.cc
        hidviz/images/images.qrc
        )

target_link_libraries(hidviz
        hidx
        Qt5::Widgets
        )
target_compile_features(hidviz PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(hidviz PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)
