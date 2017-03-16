#include "libhidx/DeviceHandle.hh"
#include "libhidx/HidParser.hh"

namespace libhidx {

    Interface::Interface(const libusb_interface& interface, Device& device) : m_interface{interface.altsetting[0]}, m_device{device} {
        loadHidDescriptor();
    }

    Interface::Interface(Interface&& o) : m_interface{o.m_interface}, m_device{o.m_device} {
        loadHidDescriptor();
    }

    bool Interface::isHid() const {
        return m_interface.bInterfaceClass == 3;
    }

    void Interface::loadHidDescriptor() {
        if(!isHid()){
            return;
        }

        constexpr uint16_t bufferLength = 1024;
        unsigned char buffer[bufferLength];

        auto handle = DeviceHandle{m_device};
        auto size = handle.controlTransfer(
            0x81,
            LIBUSB_REQUEST_GET_DESCRIPTOR,
            ((LIBUSB_DT_REPORT << 8) | 0),
            m_interface.bInterfaceNumber,
            buffer,
            bufferLength,
            1000
        );

        hid_device device{};

        hid_open_report(&device, buffer, static_cast<unsigned>(size));
        for(int i = 0; i < HID_MAX_IDS; ++i){
            auto tmp = device.report_enum[HID_INPUT_REPORT].report_id_hash[i];
            if(!tmp){
                continue;
            }

            for(int j = 0; j < HID_MAX_FIELDS; ++j){
                auto tmp2 = tmp->field[j];
                if(!tmp2){
                    continue;
                }
            }
        }

        for(unsigned i = 0; i < device.collection_size; ++i){
            const auto& col = device.collection[i];
        }
    }

    std::string Interface::getName() const {
        auto name = std::string{};
        const auto devStrings = m_device.getStrings();

        name += devStrings.manufacturer;
        name += " ";
        name += devStrings.product;

        name += " (interface ";
        name += std::to_string(getNumber());
        name += ")";

        return name;
    }
}
