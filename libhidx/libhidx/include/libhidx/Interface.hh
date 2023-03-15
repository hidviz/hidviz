//
// hidviz - Tool for in-depth analysis of USB HID devices communication
// Copyright (C) 2017 Ondrej Budai <ondrej@budai.cz>
//
// This file is part of hidviz.
//
// hidviz is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// hidviz is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with hidviz.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LIBHIDX_INTERFACE_HH
#define LIBHIDX_INTERFACE_HH

#include <atomic>
#include <thread>
#include <functional>
#include <vector>
#include <string>
#include <memory>

namespace libhidx {

    namespace buffer {
        class Interface;
        class InterfaceDescriptor;
    };

    namespace hid {
        class Item;
    }

    class Device;
    class InterfaceHandle;

    /// Class representing interface in USB device.
    class Interface {
    public:
        /**
         * Constructs new Interface.
         * @param interfaceDesc Descriptor of this interface
         * @param device Parent device
         */
        Interface(const buffer::Interface& interfaceDesc, Device& device);
        Interface(const Interface&) = delete;
        ~Interface();

        /// Returns true if this interface is of HID class.
        bool isHid() const;

        /// Returns number of this interface.
        uint32_t getNumber() const;

        /// Returns parent device.
        auto& getDevice() {return m_device;}

        /// Returns interface nice name (device name + interface number).
        std::string getName() const;

        /**
         * Returns handle to this interface.
         *
         * You can have more handles to one interface, just think of thread safety.
         * This method can return empty shared_ptr if libhidx cannot connect to device.
         * Shared ptr is used to determine the live of the handle.
         * @return Handle to device
         */
        std::shared_ptr<InterfaceHandle> getHandle();

        /// Returns descriptor of this interface.
        const auto& getDesc() const {return m_interface;}


        // HID STUFF
        /// Returns processed HID descriptor.
        hid::Item& getParsedHidReportDesc();

        /// Returns unprocessed HID descriptor.
        const std::string& getRawHidReportDesc();

        /**
         * Sets reading listener for this interface.
         *
         * Reading listener functor is called when new data is received from device.
         * @param readingListener Reading listener functor
         */
        void setReadingListener(std::function<void()> readingListener);

        /// Collects data from all HID items and sends them to device.
        void sendData();

        /// Starts reading of input reports.
        void beginReading();

        /// Stops reading of input reports.
        void stopReading();

    private:
        /// Interface descriptor.
        const buffer::InterfaceDescriptor& m_interface;

        /// Reference to parent device.
        Device& m_device;

        /**
         * Pointer to handle.
         *
         * This pointer is weak, because we don't want Interface to have
         * ownership of InterfaceHandle. We want to delete the interface
         * handle when all shared pointers to it are deleted.
         */
        std::weak_ptr<InterfaceHandle> m_handle;

        /**
         * Thread responsible for reading from interface.
         *
         * @todo This is really bad design, extract readingThread to another class.
         */
        std::thread readingThread;

        /// Flag denoting if reading thread runs.
        std::atomic_bool readingRuns{false};

        /// Flag denoting if reading thread should stop.
        std::atomic_bool stopReadingRequest{false};

        /// Runner method for reading thread.
        void runner();

        /// Address of input endpoint
        uint32_t m_inputAddress = 0;

        /// Max length of input messages (HID reports)
        uint32_t m_inputMaxSize = 0;

        /// Address of output endpoint
        uint32_t m_outputAddress = 0;

        /**
         * Denotes if interface has interrupt output endpoint.
         *
         * If interface doesn't have interrupt output endpoint,
         * control output is used instead to send HID output reports.
         */
        bool m_hasOutputAddress = false;

        /// Listener for reading thread
        std::function<void()> m_listener;

        /// Denotes if HID descriptor has been parsed yet.
        bool m_hidParsed = false;

        /// Pointer to parsed hid report descriptor.
        std::unique_ptr<hid::Item> m_parsedHidReportDesc;

        /// Raw hid descriptor.
        std::string m_rawHidReportDesc;

        /**
         * Updates data of controls belonging to this Interface.
         * @param data Received data from device.
         */
        void updateData(std::vector<unsigned char>&& data);

        /**
         * Sends HID report to device.
         * @param data HID report.
         */
        void sendOutputReport(const std::vector<unsigned char>& data);

        /// Parses hid report descriptor.
        void parseHidReportDesc();
    };

}


#endif //LIBHIDX_INTERFACE_HH
