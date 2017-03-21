#include "libhidx/LibHidx.hh"

namespace libhidx {

    LibHidx::LibHidx() {
        //TODO: check return value
        libusb_init(&ctx);
    }

    LibHidx::~LibHidx() {
        m_devices.clear();
        libusb_free_device_list(m_deviceList, 0);
        if(ctx) {
            libusb_exit(ctx);
        }
    }

    void LibHidx::loadDevices() {
        auto deviceNum = libusb_get_device_list(ctx, &m_deviceList);

        m_devices.clear();

        for (auto i = 0; i < deviceNum; ++i) {
            m_devices.emplace_back(std::make_unique<Device>(m_deviceList[i]));
        }
    }

}
