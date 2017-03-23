#ifndef LIBHIDX_HIDLIBDEVICEHANDLE_HH
#define LIBHIDX_HIDLIBDEVICEHANDLE_HH

#include "Device.hh"
#include "DeviceStrings.hh"

namespace libhidx {

    class InterfaceHandle {
    public:
        ~InterfaceHandle();
        DeviceStrings readStrings();
        int controlTransfer(uint8_t requestType,
                            uint8_t request,
                            uint16_t value,
                            uint16_t index,
                            unsigned char *data,
                            uint16_t length,
                            unsigned int timeout);

        int interruptTransfer(unsigned char endpoint,
                              unsigned char* data,
                              int length,
                              int* transferred,
                              unsigned timeout);

        friend class Interface;

    private:
        InterfaceHandle(const Interface& interface);
        const Interface& m_interface;
        uint8_t m_ifaceNumber;

        std::string extractString(uint8_t index) const;

        libusb_device_handle* m_handle;
    };

}


#endif //LIBHIDX_HIDLIBDEVICEHANDLE_HH
