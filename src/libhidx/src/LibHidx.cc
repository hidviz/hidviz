#include "libhidx/LibHidx.hh"

namespace libhidx {

    LibHidx::LibHidx() {
        //TODO: check return value
        libusb_init(&ctx);
    }

    LibHidx::~LibHidx() {
        libusb_exit(ctx);
    }

    std::vector<Device> LibHidx::enumerateDevices() {
        libusb_device** deviceList;
        auto deviceNum = libusb_get_device_list(ctx, &deviceList);
        auto devices = std::vector<Device>{};

        for (decltype(deviceNum) i = 0; i < deviceNum; ++i) {
            devices.push_back(Device{deviceList[i]});

        }

        return devices;
    }

}
