#include "libhidx/HidLib.hh"

HidLib::HidLib() {
    //TODO: check return value
    libusb_init(&ctx);
}

HidLib::~HidLib() {
    libusb_exit(ctx);
}

std::vector<HidLibDevice> HidLib::enumerateDevices() {
    libusb_device** deviceList;
    auto deviceNum = libusb_get_device_list(ctx, &deviceList);
    auto devices = std::vector<HidLibDevice>{};

    for(decltype(deviceNum) i = 0; i < deviceNum; ++i){
        devices.push_back(HidLibDevice{deviceList[i]});

    }

    return devices;
}
