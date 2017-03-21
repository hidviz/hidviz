#include "libhidx/Device.hh"

#include "libhidx/InterfaceHandle.hh"

namespace libhidx {

    Device::Device(libusb_device* device) {
        m_device = device;
        libusb_get_device_descriptor(device, &m_descriptor);

        if(libusb_get_config_descriptor(device, 0, &m_config_descriptor)) {
            m_config_descriptor = nullptr;
        }

        fillInterfaces();
    }

    Device::~Device() {
        if(m_config_descriptor != nullptr) {
            libusb_free_config_descriptor(m_config_descriptor);
        }
    }

    const DeviceStrings& Device::getStrings() {
        if(!m_strings) {
            auto deviceHandle = m_interfaces.front()->getHandle();
            m_strings = std::make_unique<DeviceStrings>(deviceHandle->readStrings());
        }

        return *m_strings.get();
    }

    void Device::fillInterfaces() {
        const auto& numInterfaces = m_config_descriptor->bNumInterfaces;
        for(auto i = 0; i < numInterfaces; ++i){
            const auto& interface_descriptor = m_config_descriptor->interface[i];
            m_interfaces.emplace_back(std::make_unique<Interface>(interface_descriptor, *this));
        }
    }
}
