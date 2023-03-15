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

#ifndef HIDVIZ_USAGE_H
#define HIDVIZ_USAGE_H

#include <cstdint>
#include <string>

namespace libhidx {
namespace hid {
    class Control;

    /// Class representing usage with value in control.
    class Usage {
    public:
        /**
         * Constructs new Usage.
         * @param usageId Id of usage
         * @param control Parent control
         */
        Usage(unsigned usageId, Control* control);

        auto getId() const { return m_id;}
        const auto &getName() const { return m_name; }

        /// Returns logical value.
        auto getLogicalValue() const { return m_logicalValue; }

        /// Sets logical value.
        void setLogicalValue(uint32_t rawLogicalValue);
        /**
         * Sets logical value base on input.
         * @todo This method should take physical value instead.
         * @todo This method does no checking on input data/
         * @param dataString
         * @return
         */
        bool setLogicalValueFromUser(const std::string& dataString);

        /// Returns physical value.
        auto getPhysicalValue() const {return m_physicalValue;}
        const auto& getControl() const {return *m_control;}

    private:
        /// Usage id (aka usage number).
        unsigned m_id = 0;

        /// Usage name (usage number converted to text representation).
        std::string m_name;

        /// Parent control
        Control* m_control = nullptr;

        /**
         * Logical value
         *
         * Value can be both int32 and uint32, so we need to use int64
         * to handle correctly both variants.
         * @todo Think about using variant
         */
        int64_t m_logicalValue = 0;

        /// Physical value
        double m_physicalValue = 0;

        /**
         * Convert logical value to physical
         * @param logicalValue Logical value
         * @return Physical value
         */
        double logicalToPhysical(int64_t logicalValue);
        //TODO uint32_t physicalToLogical(double);

        /**
         * Converts received logical value and sorts our properly signedness.
         *
         * Received data can be both signed or unsigned, so it's needed
         * to convert them to a proper type.
         * @param value Received value
         * @param size Number of bits (important because of negative number interpretation)
         * @return Converted value
         */
        int64_t convertLogicalValue(uint32_t value, unsigned int size);
    };
}
}


#endif //HIDVIZ_USAGE_H
