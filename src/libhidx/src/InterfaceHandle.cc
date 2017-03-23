#include <iostream>
#include "libhidx/InterfaceHandle.hh"

namespace libhidx {

    InterfaceHandle::InterfaceHandle(const Interface& interface)
        : m_interface{interface}, m_ifaceNumber{interface.getDesc().bInterfaceNumber} {

        auto status = libusb_open(m_interface.getDevice().getPtr(), &m_handle);
        if(status){
            std::cerr << "libusb_open failed: " << status << std::endl;
        }

        if(libusb_kernel_driver_active(m_handle, m_ifaceNumber)){
            auto s = libusb_detach_kernel_driver(m_handle, m_ifaceNumber);
            if(s){
                std::cerr << "libusb_detach_kernel_driver: " << s << std::endl;
            }
        }

        status = libusb_claim_interface(m_handle, m_ifaceNumber);

        if(status){
            std::cerr << "libusb_claim_interface failed: " << status << std::endl;
        }
    }

    InterfaceHandle::~InterfaceHandle() {
        int status;

        status = libusb_release_interface(m_handle, m_ifaceNumber);
        if(status){
            std::cerr << "libusb_release_interface failed: " << status  << std::endl;
        }
        libusb_attach_kernel_driver(m_handle, m_ifaceNumber);
        libusb_close(m_handle);
    }

    DeviceStrings InterfaceHandle::readStrings() {
        auto deviceStrings = DeviceStrings{};
        auto deviceDescriptor = m_interface.getDevice().getDesc();

        deviceStrings.manufacturer = extractString(
            deviceDescriptor.iManufacturer);
        deviceStrings.product = extractString(deviceDescriptor.iProduct);
        deviceStrings.serialNumber = extractString(
            deviceDescriptor.iSerialNumber);

        return deviceStrings;
    }

    std::string InterfaceHandle::extractString(uint8_t index) const {
        constexpr auto dataSize = 1024;
        unsigned char data[dataSize];
        auto size = libusb_get_string_descriptor_ascii(
            m_handle,
            index,
            data,
            dataSize
        );

        if (size <= 0) {
            return "";
        }

        return std::string{reinterpret_cast<char*>(data),
                           static_cast<size_t>(size)};
    }

    int InterfaceHandle::controlTransfer(uint8_t requestType, uint8_t request,
                                      uint16_t value, uint16_t index,
                                      unsigned char* data, uint16_t length,
                                      unsigned int timeout) {
        return libusb_control_transfer(
            m_handle,
            requestType,
            request,
            value,
            index,
            data,
            length,
            timeout
        );
    }

    int
    InterfaceHandle::interruptTransfer(unsigned char endpoint, unsigned char* data,
                                    int length, int* transferred,
                                    unsigned timeout) {
        int tmp;
        if(!transferred){
            transferred = &tmp;
        }

        return libusb_interrupt_transfer(
            m_handle,
            endpoint,
            data,
            length,
            transferred,
            timeout
        );
    }
}
