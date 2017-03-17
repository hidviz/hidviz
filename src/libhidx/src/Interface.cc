#include "libhidx/Interface.hh"

#include "libhidx/DeviceHandle.hh"
#include "libhidx/Parser.hh"

#include <iostream>

namespace libhidx {

    Interface::Interface(const libusb_interface& interface, Device& device) : m_interface{interface.altsetting[0]}, m_device{device} {
        loadHidDescriptor();
    }

    bool Interface::isHid() const {
        return m_interface.bInterfaceClass == 3;
    }

    void Interface::loadHidDescriptor() {
        if(!isHid()){
            return;
        }

        parseHidDescriptor();

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
        if(size <= 0){
            return;
        }

        unsigned length = m_interface.extra[8] << 8  | m_interface.extra[7];

        std::cerr << "returned: " << size << std::endl;
        std::cerr << "desc:     " << length << std::endl;

        auto parser = Parser{buffer, length};
        m_rootCollection = parser.parse();
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
        name += m_rootCollection ? "rdy" : "not";

        return name;
    }

    void Interface::parseHidDescriptor() {
//        const auto& length = m_interface.extra_length;
//        const auto& extra = m_interface.extra;
//        if(length < 9){
////            DEBUG("unknown additional register (size is too low), ending");
//            // todo: return something meaningful
//        }
//
//        // todo: extract me to a constant
//        if(extra[1] != 0x21){
////            DEBUG("unknown additional register (type is bad), ending");
////            // todo: return something meaningful
////            return desc;
//        }
//
////        desc.descriptor_type = extra[1];
//
//        desc.length = extra[0];
//        DEBUG("size: " << unsigned(desc.length));
//
//        if((length - 6) % 3 != 0){
//            DEBUG("weird length of additional register, continuing, may crash...");
//        }
//
//        desc.bcd_hid = (extra[3] << 8) | extra[2];
//        DEBUG("bcd_hid: " << std::hex << unsigned(desc.bcd_hid) << std::dec);
//
//        desc.country_code = extra[4];
//        DEBUG("country code: " << unsigned(desc.country_code));
//
//        desc.num_descriptors = extra[5];
//        DEBUG("num descriptors: " << unsigned(desc.num_descriptors));
//
//        // todo: make C++ wrapper
//        desc.extra_descriptors = (hid_extra_descriptor*)malloc(
//            desc.num_descriptors * sizeof(hid_extra_descriptor)
//        );
//
//        auto descriptors = &extra[6];
//
//        for(unsigned i = 0; i < desc.num_descriptors; ++i){
//            desc.extra_descriptors[i].descriptor_type = descriptors[0];
//            desc.extra_descriptors[i].descriptor_length =
//                (descriptors[2] << 8) | descriptors[1];
//            descriptors += 3;
//            DEBUG("#" << i << " extra descriptor");
//            DEBUG("type: " << std::hex << ((unsigned)desc.extra_descriptors[i].descriptor_type)) << std::dec;
//            DEBUG("length: " << desc.extra_descriptors[i].descriptor_length);
//        }
//
//        return desc;
    }
}
