#ifndef LIBHIDX_INTERFACE_HH
#define LIBHIDX_INTERFACE_HH

#include "Device.hh"

#include "libhidx/hid/Item.hh"

#include <libusb-1.0/libusb.h>

#include <atomic>
#include <thread>
#include <functional>

namespace libhidx {

    class Device;
    class InterfaceHandle;

    class Interface {
    public:
        Interface(const libusb_interface&, Device& device);
        Interface(const Interface&) = delete;
        ~Interface();

        bool isHid() const;
        auto getNumber() const {return m_interface.bInterfaceNumber;}
        const auto& getDevice() const {return m_device;}
        std::string getName() const;
        std::shared_ptr<InterfaceHandle> getHandle();
        const auto& getDesc() const {return m_interface;}
        hid::Item* getHidReportDesc();
        void setReadingListener(std::function<void()>);
        void sendData();


        void beginReading();
        void stopReading();

    private:
        const libusb_interface_descriptor& m_interface;
        Device& m_device;
        std::weak_ptr<InterfaceHandle> m_handle;

        std::thread readingThread;
        std::atomic_bool readingRuns;
        std::atomic_bool stopReadingRequest;
        void runner();

        uint8_t m_inputAddress = 0;
        uint16_t m_inputMaxSize = 0;
        uint8_t m_outputAddress = 0;
        bool m_hasOutputAddress = false;

        std::function<void()> m_listener;

        std::unique_ptr<hid::Item> m_hidReportDesc;

        void updateData(const std::vector<unsigned char>& data);
        void sendOutputReport(const std::vector<unsigned char>& data);
    };

}


#endif //LIBHIDX_INTERFACE_HH
