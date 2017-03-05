#ifndef HIDVIZ_HIDLIBDEVICEHANDLE_HH
#define HIDVIZ_HIDLIBDEVICEHANDLE_HH

#include "Device.hh"
#include "DeviceStrings.hh"

namespace libhidx {

    class DeviceHandle {
    public:
        DeviceHandle(const Device& device);

        ~DeviceHandle();

        DeviceStrings readStrings();

    private:
        libusb_device_handle* m_handle = nullptr;
        const Device& m_device;

        std::string extractString(uint8_t index) const;
    };

}


#endif //HIDVIZ_HIDLIBDEVICEHANDLE_HH
