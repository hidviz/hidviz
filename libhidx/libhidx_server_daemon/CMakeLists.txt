cmake_minimum_required(VERSION 3.2)
project(libhidx_server VERSION 0.0.1 LANGUAGES C CXX)

add_executable(libhidx_server_daemon
        src/main.cc
        src/args.hh
        )

target_link_libraries(libhidx_server_daemon
        hidx_server
        )

target_compile_features(libhidx_server_daemon PUBLIC
        cxx_relaxed_constexpr #this should force c++14
        )
target_compile_options(libhidx_server_daemon PRIVATE $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>: -Wall -Wextra>)

if(NOT WIN32)
    include(GNUInstallDirs)
    install(TARGETS libhidx_server_daemon
            RUNTIME DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}
            )
    set_target_properties(libhidx_server_daemon PROPERTIES INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/hidviz)
endif()
