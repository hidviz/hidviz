#include "libhidx/Device.hh"

#include "libhidx/DeviceHandle.hh"

namespace libhidx {

    Device::Device(libusb_device* device) {
        m_device = device;
        libusb_get_device_descriptor(device, &m_descriptor);

        if(libusb_get_config_descriptor(device, 0, &m_config_descriptor)) {
            m_config_descriptor = nullptr;
        }

        readStrings();
        fillInterfaces();
    }

    Device::~Device() {
        if(m_config_descriptor != nullptr) {
            libusb_free_config_descriptor(m_config_descriptor);
        }

        libusb_unref_device(m_device);
    }

    void Device::readStrings() {
        auto deviceHandle = DeviceHandle{*this};
        m_strings = deviceHandle.readStrings();
    }

    void Device::fillInterfaces() {
        const auto& numInterfaces = m_config_descriptor->bNumInterfaces;
        for(auto i = 0; i < numInterfaces; ++i){
            const auto& interface_descriptor = m_config_descriptor->interface[i];
            m_interfaces.emplace_back(std::make_unique<Interface>(interface_descriptor, *this));
        }
    }

    Device::Device(Device&& d) {
        m_device = d.m_device;
        d.m_device = nullptr;

        m_id = d.m_id;

        m_config_descriptor = d.m_config_descriptor;
        d.m_config_descriptor = nullptr;

        m_descriptor = d.m_descriptor;
        m_strings = d.m_strings;
        m_interfaces = std::move(d.m_interfaces);

    }
}
