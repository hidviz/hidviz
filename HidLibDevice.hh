#ifndef HIDVIZ_HIDLIBDEVICE_HH
#define HIDVIZ_HIDLIBDEVICE_HH

#include "HidLibDeviceStrings.hh"

#include <libusb.h>

class HidLibDevice {
public:

    HidLibDevice(libusb_device* device);
    libusb_device* getDevice() const {return m_device;}
    const libusb_device_descriptor& getDescriptor() const {return m_descriptor;}
    const HidLibDeviceStrings& getStrings() const {return m_strings;}


private:
    libusb_device* m_device = nullptr;
    libusb_device_descriptor m_descriptor;
    HidLibDeviceStrings m_strings;

    void readStrings();

};


#endif //HIDVIZ_HIDLIBDEVICE_HH
