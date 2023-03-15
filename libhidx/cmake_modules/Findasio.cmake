add_library(asio INTERFACE)

if(EXISTS "/usr/include/asio.hpp")
    target_include_directories(asio INTERFACE "/usr/include/asio")
else()
    if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/asio/asio/include/asio.hpp")
        find_package(Git)

        if(NOT GIT_FOUND)
            message(FATAL_ERROR "Git not found")
        endif()

        execute_process(
                COMMAND "${GIT_EXECUTABLE}" clone --branch asio-1-14-0 --depth=1 https://github.com/chriskohlhoff/asio.git
                WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" RESULT_VARIABLE RESULT
        )

        if(NOT RESULT EQUAL 0)
            message(FATAL_ERROR "Cannot clone asio repository")
        endif()
    endif()
    target_include_directories(asio INTERFACE "${CMAKE_CURRENT_BINARY_DIR}/asio/asio/include/")
endif()

target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

if(WIN32)
    target_link_libraries(asio INTERFACE ws2_32)
endif(WIN32)
