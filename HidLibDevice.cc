#include "HidLibDevice.hh"

#include "HidLibDeviceHandle.hh"

HidLibDevice::HidLibDevice(libusb_device* device) {
    m_device = device;
    libusb_get_device_descriptor(device, &m_descriptor);
    readStrings();
}

void HidLibDevice::readStrings() {
    auto deviceHandle = HidLibDeviceHandle{*this};
    m_strings = deviceHandle.readStrings();
}
