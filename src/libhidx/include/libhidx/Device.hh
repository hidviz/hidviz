#ifndef LIBHIDX_HIDLIBDEVICE_HH
#define LIBHIDX_HIDLIBDEVICE_HH

#include "DeviceStrings.hh"
#include "Interface.hh"

#include <libusb-1.0/libusb.h>

#include <vector>
#include <memory>

namespace libhidx {
    class Interface;

    class Device {
    public:

        Device(libusb_device* device);
        Device(const Device&) = delete;
        ~Device();

        const auto& getPtr() const { return m_device; }
        const auto& getDesc() const { return m_descriptor; }
        const DeviceStrings& getStrings();
        const auto& getInterfaces() const {return m_interfaces;}


    private:
        libusb_device* m_device = nullptr;
        libusb_device_descriptor m_descriptor;
        libusb_config_descriptor* m_config_descriptor = nullptr;
        std::unique_ptr<DeviceStrings> m_strings;
        std::vector<std::unique_ptr<Interface>> m_interfaces;

        void fillInterfaces();

    };
}


#endif //LIBHIDX_HIDLIBDEVICE_HH
