#ifndef LIBHIDX_HIDLIBDEVICEHANDLE_HH
#define LIBHIDX_HIDLIBDEVICEHANDLE_HH

#include "Device.hh"
#include "DeviceStrings.hh"

namespace libhidx {

    class DeviceHandle {
    public:
        DeviceHandle(const Device& device);
        ~DeviceHandle();
        DeviceStrings readStrings();
        int controlTransfer(uint8_t requestType,
                            uint8_t request,
                            uint16_t value,
                            uint16_t index,
                            unsigned char *data,
                            uint16_t length,
                            unsigned int timeout);

    private:
        libusb_device_handle* m_handle = nullptr;
        const Device& m_device;

        std::string extractString(uint8_t index) const;
    };

}


#endif //LIBHIDX_HIDLIBDEVICEHANDLE_HH
