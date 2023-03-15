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

#ifndef LIBHIDX_PARSER
#define LIBHIDX_PARSER

#include <libhidx/hid/Control.hh>

#include <vector>
#include <map>
#include <stdexcept>

namespace libhidx {

    namespace hid {
        class Item;
        class Collection;
    }

    /// Parser error.
    class ParserError : public std::runtime_error{
    public:
        explicit ParserError(const std::string& what_arg) :
            std::runtime_error{what_arg}{}
    };

    /// Parser for parsing HID descriptor.
    class Parser {
    public:
        /**
         * Constructs Parser instance.
         * @todo Change interface to C++.
         * @param start Buffer with descriptor to be processed.
         * @param size Buffer size
         */
        Parser(const uint8_t* start, std::size_t size);

        /// Parses the data.
        void parse();

        /// Returns parsed descriptor.
        hid::Item* getParsed(){return m_parsed;}

        /// Returns raw descriptor converted to textual representation.
        const std::string& getRaw(){return m_raw;}

    private:
        /// Helper class for storing Item data during parsing
        struct Item {
        public:
            /// Returns unsigned value.
            uint32_t udata() {
                switch (size) {
                    case 1:
                        return data.u8;
                    case 2:
                        return data.u16;
                    case 4:
                        return data.u32;
                }
                return 0;
            }

            /// Returns signed value.
            int32_t sdata() {
                switch (size) {
                    case 1:
                        return data.s8;
                    case 2:
                        return data.s16;
                    case 4:
                        return data.s32;
                }
                return 0;
            }

            /// Determines if the item is in long format.
            bool isLongFormat;
            uint8_t type;

            /// All possible item tags.
            struct TAG {
                struct GLOBAL {
                    constexpr static uint8_t USAGE_PAGE = 0;
                    constexpr static uint8_t LOGICAL_MINIMUM = 1;
                    constexpr static uint8_t LOGICAL_MAXIMUM = 2;
                    constexpr static uint8_t PHYSICAL_MINIMUM = 3;
                    constexpr static uint8_t PHYSICAL_MAXIMUM = 4;
                    constexpr static uint8_t UNIT_EXPONENT = 5;
                    constexpr static uint8_t UNIT = 6;
                    constexpr static uint8_t REPORT_SIZE = 7;
                    constexpr static uint8_t REPORT_ID = 8;
                    constexpr static uint8_t REPORT_COUNT = 9;
                    constexpr static uint8_t PUSH = 10;
                    constexpr static uint8_t POP = 11;
                };

                struct LOCAL {
                    constexpr static uint8_t USAGE = 0;
                    constexpr static uint8_t USAGE_MINIMUM = 1;
                    constexpr static uint8_t USAGE_MAXIMUM = 2;
                    constexpr static uint8_t DESIGNATOR_INDEX = 3;
                    constexpr static uint8_t DESIGNATOR_MINIMUM = 4;
                    constexpr static uint8_t DESIGNATOR_MAXIMUM = 5;
                    constexpr static uint8_t STRING_INDEX = 7;
                    constexpr static uint8_t STRING_MINIMUM = 8;
                    constexpr static uint8_t STRING_MAXIMUM = 9;
                    constexpr static uint8_t DELIMITER = 10;
                };
                struct MAIN {
                    constexpr static uint8_t INPUT = 8;
                    constexpr static uint8_t OUTPUT = 9;
                    constexpr static uint8_t FEATURE = 11;
                    constexpr static uint8_t BEGIN_COLLECTION = 10;
                    constexpr static uint8_t END_COLLECTION = 12;
                };
                constexpr static uint8_t LONG = 15;
            };

            uint8_t tag = 0;
            uint8_t size = 0;

            /// Union for storing item data.
            union {
                const uint8_t* longdata;
                uint8_t u8;
                uint16_t u16;
                uint32_t u32;
                int8_t s8;
                int16_t s16;
                int32_t s32;
            } data = {.u32 = 0};
        };

        /// Currently processed item.
        Item m_currentItem;

        /// Class for storing local item data.
        struct Local {
            std::size_t delimiterDepth = 0;
            std::size_t delimiterBranch = 0;
            std::vector<unsigned> usagesStack;
            unsigned usageMinimum = 0;
        };

        /// Stores local item data.
        Local m_local;

        /// Class for storing global item data.
        struct Global {
            int32_t logicalMinimum = 0;
            int32_t logicalMaximum = 0;
            int32_t physicalMinimum = 0;
            int32_t physicalMaximum = 0;
            int32_t unitExponent = 0;
            unsigned unit = 0;

            unsigned reportId = 0;
            unsigned reportSize = 0;
            unsigned reportCount = 0;
            unsigned usagePage = 0;
        };

        /// Stores global item data.
        Global m_global;

        /// Stack for storing global item data in case of push/pup items.
        std::vector<Global> m_globalStack;

        /// Buffer.
        const uint8_t * const m_start = nullptr;

        /// Buffer size.
        const std::size_t m_size = 0;

        /// Current report size for each reportID - control type combination.
        std::map<std::pair<unsigned, hid::Control::Type>, std::size_t> m_reportSizes;

        /// Stack of collections.
        ///
        /// @todo Use stack.
        std::vector<hid::Collection*> m_collectionStack;

        /// Pointer to parsed HID descriptor top collection.
        hid::Item* m_parsed;

        /// Text representation of raw HID descriptor.
        std::string m_raw;

        /// Indent to be used when formatting raw descriptor.
        const std::string INDENT = "  ";

        /// Current indentation.
        std::string m_currentIndent;

        /**
         * Fetches next item from descriptor.
         *
         * @param start First unprocessed byte
         * @param end First byte to be nonprocessed (after the end)
         * @return First byte of next unproccesed item.
         */
        const uint8_t* fetchItem(const uint8_t* start, const uint8_t* end);

        /// Parses main item.
        void parseMainItem();

        /// Opens new collection.
        void openCollection();

        /// Closes current collection.
        void closeCollection();

        /// Adds new control.
        void addField(libhidx::hid::Control::Type reportType);

        /// Parses global item.
        void parseGlobalItem();

        /// Parses local item.
        void parseLocalItem();

        /// Parses reserved item.
        void parseReservedItem();

    };

}
#endif //LIBHIDX_PARSER2
