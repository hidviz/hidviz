#ifndef HIDVIZ_HIDLIB_HH
#define HIDVIZ_HIDLIB_HH

#include "HidLibDevice.hh"

#include <libusb-1.0/libusb.h>

#include <vector>

class HidLib {
public:
    HidLib();
    ~HidLib();

    std::vector<HidLibDevice> enumerateDevices();

private:
    libusb_context* ctx;
};


#endif //HIDVIZ_HIDLIB_HH
