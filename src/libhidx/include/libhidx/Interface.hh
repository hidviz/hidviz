#ifndef HIDVIZ_INTERFACE_HH
#define HIDVIZ_INTERFACE_HH


#include <libusb-1.0/libusb.h>

namespace libhidx {

    class Interface {
    public:
        Interface(const libusb_interface&);
        bool isHid() const;
        int getNumber() const {return m_interface.bInterfaceNumber;}

    private:
        libusb_interface_descriptor m_interface;
    };

}


#endif //HIDVIZ_INTERFACE_HH
