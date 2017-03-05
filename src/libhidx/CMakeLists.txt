project(libhidx VERSION 0.0.1 LANGUAGES CXX)

add_library(hidx
        src/HidLib.cc
        src/HidLibDevice.cc
        src/HidLibDeviceHandle.cc
        )

target_include_directories(hidx PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

target_link_libraries(hidx
        libusb-1.0)

# 'make install' to the correct location
install(TARGETS hidx EXPORT LibHidxConfig
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

# This makes the project importable from the install directory
# Put config file in per-project dir (name MUST match), can also
# just go into <prefix>/cmake.
install(EXPORT LibHidxConfig DESTINATION share/LibHidx/cmake)

# This makes the project importable from the build directory
export(TARGETS hidx FILE LibHidxConfig.cmake)
