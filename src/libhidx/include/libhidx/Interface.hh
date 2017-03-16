#ifndef HIDVIZ_INTERFACE_HH
#define HIDVIZ_INTERFACE_HH

#include "Device.hh"

#include <libusb-1.0/libusb.h>

namespace libhidx {

    class Device;

    class Interface {
    public:
        Interface(const libusb_interface&, Device& device);
        Interface(const Interface&) = delete;
        Interface(Interface&&);

        bool isHid() const;
        int getNumber() const {return m_interface.bInterfaceNumber;}
        const Device& getDevice() const {return m_device;}
        std::string getName() const;

    private:
        const libusb_interface_descriptor& m_interface;
        Device& m_device;

        void loadHidDescriptor();
    };

}


#endif //HIDVIZ_INTERFACE_HH
