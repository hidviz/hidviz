#ifndef LIBHIDX_HIDLIB_HH
#define LIBHIDX_HIDLIB_HH

#include "Device.hh"

#include <libusb-1.0/libusb.h>

#include <vector>
#include <memory>

namespace libhidx {

    class LibHidx {
    public:
        LibHidx();
        LibHidx(const LibHidx&) = delete;
        ~LibHidx();

        void loadDevices();
        const auto& getDevices(){return m_devices;}

    private:
        libusb_context* ctx = nullptr;
        std::vector<std::unique_ptr<Device>> m_devices;
        libusb_device** m_deviceList;
    };

}


#endif //LIBHIDX_HIDLIB_HH
