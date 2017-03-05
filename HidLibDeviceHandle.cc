#include "HidLibDeviceHandle.hh"

HidLibDeviceHandle::HidLibDeviceHandle(const HidLibDevice& device) : m_device{device} {
    // TODO: check value
    libusb_open(device.getDevice(), &m_handle);

    // TODO: not supported everywhere :(
    libusb_set_auto_detach_kernel_driver(m_handle, true);
    libusb_claim_interface(m_handle, 0);
}

HidLibDeviceHandle::~HidLibDeviceHandle() {
    libusb_release_interface(m_handle, 0);

    libusb_close(m_handle);
    m_handle = nullptr;
}

HidLibDeviceStrings HidLibDeviceHandle::readStrings() {
    auto deviceStrings = HidLibDeviceStrings{};
    auto deviceDescriptor = m_device.getDescriptor();

    deviceStrings.manufacturer = extractString(deviceDescriptor.iManufacturer);
    deviceStrings.product = extractString(deviceDescriptor.iProduct);
    deviceStrings.serialNumber = extractString(deviceDescriptor.iSerialNumber);

    return deviceStrings;
}

std::string HidLibDeviceHandle::extractString(uint8_t index) const {
    constexpr auto dataSize = 1024;
    unsigned char data[dataSize];
    auto size = libusb_get_string_descriptor_ascii(
        m_handle,
        index,
        data,
        dataSize
    );

    if(size <= 0){
        return "";
    }

    return std::string{reinterpret_cast<char*>(data), static_cast<size_t>(size)};
}
