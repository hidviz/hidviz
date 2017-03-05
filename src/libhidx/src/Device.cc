#include "libhidx/Device.hh"

#include "libhidx/DeviceHandle.hh"

namespace libhidx {

    Device::Device(libusb_device* device) {
        m_device = device;
        libusb_get_device_descriptor(device, &m_descriptor);
        readStrings();
    }

    void Device::readStrings() {
        auto deviceHandle = DeviceHandle{*this};
        m_strings = deviceHandle.readStrings();
    }

}
