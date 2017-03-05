#ifndef HIDVIZ_HIDLIB_HH
#define HIDVIZ_HIDLIB_HH

#include "Device.hh"

#include <libusb-1.0/libusb.h>

#include <vector>

namespace libhidx {

    class LibHidx {
    public:
        LibHidx();

        ~LibHidx();

        std::vector<Device> enumerateDevices();

    private:
        libusb_context* ctx;
    };

}


#endif //HIDVIZ_HIDLIB_HH
