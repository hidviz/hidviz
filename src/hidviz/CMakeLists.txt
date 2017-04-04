# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(hidviz
        main.cc
        DeviceSelector.cc
        Window.cc
        DeviceSelectionListItem.cc
        hid/Item.cc
        hid/Collection.cc
        hid/Control.cc
        FlowLayout.cc
        resources.qrc
        DeviceView.cc
        )

target_link_libraries(hidviz
        hidx
        Qt5::Widgets
        Threads::Threads
)
