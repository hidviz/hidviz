cmake_minimum_required(VERSION 3.2)
project(libhidx_server VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")

find_package(libusb-1.0 REQUIRED)
add_library(libusb-1.0 UNKNOWN IMPORTED)
set_property(TARGET libusb-1.0 PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${LIBUSB_1_INCLUDE_DIRS})
set_property(TARGET libusb-1.0 PROPERTY IMPORTED_LOCATION ${LIBUSB_1_LIBRARY})

find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS src/Buffer.proto)

add_library(hidx_server
        src/Server.cc include/libhidx/server/Server.hh
        src/Utils.cc include/libhidx/server/Utils.hh
        include/libhidx/server/Buffer.hh
        ${PROTO_SRCS} ${PROTO_HDRS}
        )

target_include_directories(hidx_server PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${CMAKE_CURRENT_BINARY_DIR}
        PRIVATE src)

target_link_libraries(hidx_server
        libusb-1.0
        asio
        ${PROTOBUF_LIBRARIES}
        )

target_compile_features(hidx_server PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(hidx_server PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)

get_target_property(HIDX_SERVER_TYPE hidx TYPE)

if(NOT WIN32 AND ${HIDX_SERVER_TYPE} STREQUAL SHARED_LIBRARY)
    include(GNUInstallDirs)
    install(TARGETS hidx_server
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/hidviz
            )
    set_target_properties(hidx_server PROPERTIES INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/hidviz)
endif()
