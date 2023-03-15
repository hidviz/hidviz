cmake_minimum_required(VERSION 3.2)

# Cmake 3.17 changed automoc behaviour for header files with .hh extension.
# Now these files are passed to automoc. When policy is set to <3.17, these
# file are no more passed to it. This is very weird, because this just worked
# in cmake <3.17. Setting the policies to 3.17 fixes this, because we want
# .hh to be included.
if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.17)
  cmake_policy(SET CMP0100 NEW)
endif()

# Find the QtWidgets library
find_package(Qt6 REQUIRED COMPONENTS Widgets)

find_package(Protobuf REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(hidviz WIN32
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
        src/hid/UsageWidget.cc src/hid/UsageWidget.hh
        src/WaitDialog.cc src/WaitDialog.hh
        src/Global.hh src/WindowsDriverDialog.cc src/WindowsDriverDialog.hh)

target_link_libraries(hidviz
        hidx
        Qt6::Widgets
        )
target_compile_features(hidviz PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(hidviz PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)

target_include_directories(hidviz PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${PROTOBUF_INCLUDE_DIRS})


if(NOT WIN32)
    include(GNUInstallDirs)
    install(TARGETS hidviz
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            )
    set_target_properties(hidviz PROPERTIES INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/hidviz)
endif()
