#ifndef HIDVIZ_HIDLIBDEVICE_HH
#define HIDVIZ_HIDLIBDEVICE_HH

#include "DeviceStrings.hh"

#include <libusb-1.0/libusb.h>

namespace libhidx {
    class Device {
    public:

        Device(libusb_device* device);

        libusb_device* getDevice() const { return m_device; }

        const libusb_device_descriptor&
        getDescriptor() const { return m_descriptor; }

        const DeviceStrings& getStrings() const { return m_strings; }


    private:
        libusb_device* m_device = nullptr;
        libusb_device_descriptor m_descriptor;
        DeviceStrings m_strings;

        void readStrings();

    };
}


#endif //HIDVIZ_HIDLIBDEVICE_HH
