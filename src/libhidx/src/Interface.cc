#include "libhidx/Interface.hh"

#include "libhidx/InterfaceHandle.hh"
#include "libhidx/Parser.hh"

#include <iostream>
#include <cassert>

namespace libhidx {

    Interface::Interface(const libusb_interface& interface, Device& device) : m_interface{interface.altsetting[0]}, m_device{device}, readingRuns{false}, stopReadingRequest{false} {
        for(unsigned i = 0; i < m_interface.bNumEndpoints; ++i){
            const auto& endpoint = m_interface.endpoint[i];
            bool isInterrupt =
                (endpoint.bmAttributes & LIBUSB_TRANSFER_TYPE_MASK) == LIBUSB_TRANSFER_TYPE_INTERRUPT;
            bool isInput =
                (endpoint.bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN;

            if(isInterrupt && isInput){
                m_inputAddress = endpoint.bEndpointAddress;
                m_inputMaxSize = endpoint.wMaxPacketSize;
                break;
            }
        }
    }

    Interface::~Interface() {
        if(readingRuns) {
            stopReadingRequest = true;
            readingThread.join();
        }
    }

    bool Interface::isHid() const {
        return m_interface.bInterfaceClass == 3;
    }

    hid::Item* Interface::getHidReportDesc() {
        assert(isHid());

        if(m_hidReportDesc){
            return m_hidReportDesc.get();
        }

        constexpr uint16_t bufferLength = 1024;
        unsigned char buffer[bufferLength];

        auto handle = getHandle();
        auto size = handle->controlTransfer(
            0x81,
            LIBUSB_REQUEST_GET_DESCRIPTOR,
            ((LIBUSB_DT_REPORT << 8) | 0),
            m_interface.bInterfaceNumber,
            buffer,
            bufferLength,
            1000
        );
        if(size <= 0){
            //TODO: throw an exception
            return nullptr;
        }

        auto parser = Parser{buffer, static_cast<size_t>(size)};
        m_hidReportDesc.reset(parser.parse());

        return m_hidReportDesc.get();

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

    void Interface::beginReading() {
        if(readingRuns) {
            std::cerr << "fail begin reading" << std::endl;
            return;
        }
        if(readingThread.joinable()){
            readingThread.join();
        }

        readingRuns = true;
        std::thread t{&Interface::runner, this};

        readingThread = std::move(t);
    }

    void Interface::stopReading() {
        stopReadingRequest = true;
    }

    void Interface::runner() {
        std::cerr << "thread running" << std::endl;

        auto handle = getHandle();

        constexpr int bufferLength = 1000;
        unsigned char buffer[bufferLength];

        while(!stopReadingRequest){
            int size = handle->interruptTransfer(
                m_inputAddress,
                buffer,
                bufferLength,
                nullptr,
                1000
            );

            std::cerr << size << std::endl;
        }

        stopReadingRequest = false;
        readingRuns = false;
    }

    std::shared_ptr<InterfaceHandle> Interface::getHandle() {
        std::shared_ptr<InterfaceHandle> ptr;
        if(m_handle.expired()){
            ptr.reset(new InterfaceHandle{*this});
            m_handle = ptr;
        }

        return m_handle.lock();
    }


}
