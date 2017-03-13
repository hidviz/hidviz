#include "libhidx/Interface.hh"

namespace libhidx {

    Interface::Interface(const libusb_interface& interface) : m_interface{interface.altsetting[0]} {
    }

    bool Interface::isHid() const {
        return m_interface.bInterfaceClass == 3;
    }

}
