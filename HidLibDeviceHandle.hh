#ifndef HIDVIZ_HIDLIBDEVICEHANDLE_HH
#define HIDVIZ_HIDLIBDEVICEHANDLE_HH

#include "HidLibDevice.hh"
#include "HidLibDeviceStrings.hh"

class HidLibDeviceHandle {
public:
    HidLibDeviceHandle(const HidLibDevice& device);
    ~HidLibDeviceHandle();

    HidLibDeviceStrings readStrings();

private:
    libusb_device_handle* m_handle = nullptr;
    const HidLibDevice& m_device;

    std::string extractString(uint8_t index) const;
};


#endif //HIDVIZ_HIDLIBDEVICEHANDLE_HH
