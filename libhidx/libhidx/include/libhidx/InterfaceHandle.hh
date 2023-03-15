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

#ifndef LIBHIDX_HIDLIBDEVICEHANDLE_HH
#define LIBHIDX_HIDLIBDEVICEHANDLE_HH

#include <libhidx/server/Buffer.hh>

#include <stdexcept>

namespace libhidx {

    class Interface;
    class LibHidx;
    class DeviceStrings;

    /// Exception thrown when there's an error when connecting to interface.
    class ConnectionException : public std::system_error {
    public:
        explicit ConnectionException(int ev, const std::string& err) : std::system_error{ev, std::generic_category(),  err}{}
    };

    /**
     * Handle for open interface.
     *
     * The only constructor of this class is private.
     * If you want to communicate with interface, you can create instance of
     * InterfaceHandle by calling Interface::handle.
     */

    class InterfaceHandle {
        friend class Interface;
    public:
        ~InterfaceHandle();

        /// Read strings from
        DeviceStrings readStrings();

        /**
         * Performs control output transfer.
         *
         * More info can be found in libusb docs.
         */
        int controlOutTransfer(uint8_t requestType,
                               uint8_t request,
                               uint16_t value,
                               uint16_t index,
                               const void* data,
                               size_t length,
                               unsigned int timeout
        );

        /**
         * Performs control input transfer.
         *
         * More info can be found in libusb docs.
         */
        std::pair<int, std::string> controlInTransfer(uint8_t requestType,
                                                      uint8_t request,
                                                      uint16_t value,
                                                      uint16_t index,
                                                      uint16_t length,
                                                      unsigned int timeout
        );

        /**
         * Performs interrupt output transfer.
         *
         * More info can be found in libusb docs.
         */
        buffer::InterruptOutTransfer::Response
        interruptOutTransfer(unsigned char endpoint, const void* data,
                                              size_t length,
                                              unsigned timeout);

        /**
         * Peforms interrupt input transfer.
         *
         * More info can be found in libusb docs.
         */
        buffer::InterruptInTransfer::Response
        interruptInTransfer(unsigned char endpoint, uint16_t length, unsigned timeout);

    private:
        /// Interface this handle belongs to.
        Interface& m_interface;

        /// Number of interface this handle belongs to.
        uint32_t m_ifaceNumber;

        /// Libusb handle.
        uint64_t m_handle;

        /// Libhidx instance.
        LibHidx& m_lib;

        /**
         * Constructs Interface handle.
         *
         * Contructor is private. You can get instance of this class
         * by calling Interface::getHandle method.
         * @param interface Inteface
         */
        explicit InterfaceHandle(Interface& interface);

        /**
         * Extracts string from interface.
         *
         * @param index Index to string.
         * @return Extracted string (empty if fails).
         * @todo Return more meaningful value if call fails.
         */
        std::string extractString(uint32_t index) const;
    };

}


#endif //LIBHIDX_HIDLIBDEVICEHANDLE_HH
