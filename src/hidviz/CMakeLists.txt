# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

add_executable(hidviz
        main.cc
        TreeModel.cc
        TreeItem.cc
        DeviceSelector.cc
        Window.cc
        DeviceSelectionListItem.cc)

target_link_libraries(hidviz
        hidx
        Qt5::Widgets
)
