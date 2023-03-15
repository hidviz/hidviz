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

#ifndef LIBHIDX_CONTROL_HH
#define LIBHIDX_CONTROL_HH

#include <mutex>
#include "Item.hh"
#include "Usage.hh"

namespace libhidx {
    class Parser;

namespace hid {

    /// Representation of HID control (input, output, feature)
    class Control : public Item {
        friend class libhidx::Parser;
    public:
        /// Control types
        enum class Type {
            /// Not defined
            UNDEFINED = -1,
            /// Input
            INPUT = 0,
            /// Output
            OUTPUT  = 1,
            /// Feature
            FEATURE = 2,
        };

        /**
         * Contructs Control instance
         * @param parent Parent collection
         */
        explicit Control(Item* parent = nullptr) : Item{parent} {}

        /**
         * Sets new data for this control.
         * @param rawData Raw received data.
         * @param reportId ID of received report
         */
        void setData(const std::vector<unsigned char>& rawData, unsigned reportId);

        /// Returns data for this control.
        uint32_t getData() const;
        auto getReportType() const {return m_reportType;}

        /// Returns all usage instances.
        const auto& getUsages() const {return m_usages;}

        /// Return offset in HID report.
        auto getOffset() const {return m_offset;};
        auto getLogicalMinimum() const {return m_logicalMinimum;}
        auto getLogicalMaximum() const {return m_logicalMaximum;}
        auto getPhysicalMinimum() const {return m_physicalMinimum;}
        auto getPhysicalMaximum() const {return m_physicalMaximum;}
        auto getUnitExponent() const {return m_unitExponent;}
        auto getReportId() const {return m_reportId;}
        auto getUnit() const {return m_unit;}
        auto getReportSize() const {return m_reportSize;}
        auto getReportCount() const {return m_reportCount;}

        /// Flags for control
        enum Flag {
            CONSTANT = 0x01,
            VARIABLE = 0x02,
            // do not use word RELATIVE, as it's defined as macro in msys2 env
            FRELATIVE = 0x04,
            WRAP = 0x08,
            NONLINEAR = 0x10,
            NO_PREFERRED = 0x20,
            NULL_STATE = 0x40,
            VOLATILE = 0x80,
        };
        bool isConstant() const {return (m_flags & Flag::CONSTANT) != 0;}
        bool isVariable() const {return (m_flags & Flag::VARIABLE) != 0;}
        bool isRelative() const {return (m_flags & Flag::FRELATIVE) != 0;}
        bool isWrap() const {return (m_flags & Flag::WRAP) != 0;}
        bool isNonlinear() const {return (m_flags & Flag::NONLINEAR) != 0;}
        bool isNoPreferred() const {return (m_flags & Flag::NO_PREFERRED) != 0;}
        bool isNullState() const {return (m_flags & Flag::NULL_STATE) != 0;}
        bool isVolatile() const {return (m_flags & Flag::VOLATILE) != 0;}

        /// Returns true if this value is binary (has two states - 0 and 1)
        bool isBinary() const;

    private:
        /// Control's offset in HID report
        std::size_t m_offset = 0;

        /// Vector of all child usages
        std::vector<std::unique_ptr<Usage>> m_usages;
        uint32_t m_flags = 0;
        Type m_reportType = Type::UNDEFINED;
        unsigned m_reportSize = 0;
        unsigned m_reportCount = 0;
        int32_t m_logicalMinimum = 0;
        int32_t m_logicalMaximum = 0;
        int32_t m_physicalMinimum = 0;
        int32_t m_physicalMaximum = 0;
        int32_t m_unitExponent = 0;
        unsigned m_unit = 0;
        unsigned int m_reportId = 0;

        /**
         * Extracts data from HID report to this control
         *
         * Vector of booleans is used as abstraction for bitfield.s
         * @param vector HID report
         * @return This control's data
         */
        std::vector<bool> extractData(const std::vector<unsigned char>& vector);

        /**
         * Extracts data for usage from control's data
         * @param data Data for current control
         * @param index Index of usage to be extracted
         * @return Data for usage at specific index
         */
        uint32_t extractVariableUsageData(const std::vector<bool>& data, unsigned index);

        /**
         * Finds usage by its ID in current Control
         * @param id
         * @return Found usage or nullptr if wasn't found
         */
        Usage* findUsageById(uint32_t id);
    };
}
}


#endif //LIBHIDX_CONTROL__HH
