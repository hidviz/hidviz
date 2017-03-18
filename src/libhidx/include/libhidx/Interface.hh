#ifndef LIBHIDX_INTERFACE_HH
#define LIBHIDX_INTERFACE_HH

#include "Device.hh"

#include <libusb-1.0/libusb.h>

namespace libhidx {
namespace hid {
    class Item;
}

    class Device;

    class Interface {
    public:
        Interface(const libusb_interface&, Device& device);
        Interface(const Interface&) = delete;

        bool isHid() const;
        auto getNumber() const {return m_interface.bInterfaceNumber;}
        const auto& getDevice() const {return m_device;}
        std::string getName() const;
        const auto& getRootCollection() const {return m_rootCollection;}

    private:
        const libusb_interface_descriptor& m_interface;
        Device& m_device;

        void loadHidDescriptor();
        hid::Item* m_rootCollection;

        void parseHidDescriptor();
    };

}


#endif //LIBHIDX_INTERFACE_HH
