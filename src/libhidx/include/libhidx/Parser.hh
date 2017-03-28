#ifndef LIBHIDX_PARSER
#define LIBHIDX_PARSER

#include "libhidx/hid/Control.hh"

#include <vector>
#include <map>

namespace libhidx {

    namespace hid {
        class Item;
        class Collection;
    }

    class ParserError : public std::runtime_error{
    public:
        explicit ParserError(const std::string& what_arg) :
            std::runtime_error{what_arg}{}
        explicit ParserError(const char* what_arg) :
            std::runtime_error{what_arg}{}
    };

    class Parser {
    public:
        Parser(uint8_t* start, std::size_t size);
        hid::Item* parse();

    private:
        struct Item {
        public:
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

            bool isLongFormat;

            uint8_t type;
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

            union {
                uint8_t* longdata;
                uint8_t u8;
                uint16_t u16;
                uint32_t u32;
                int8_t s8;
                int16_t s16;
                int32_t s32;
            } data = {.u32 = 0};
        };
        Item m_currentItem;

        struct Local {
            std::size_t delimiterDepth = 0;
            std::size_t delimiterBranch = 0;
            std::vector<unsigned> usagesStack;
            unsigned usageMinimum = 0;
        };
        Local m_local;

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
        Global m_global;
        std::vector<Global> m_globalStack;

        uint8_t * const m_start = nullptr;
        const std::size_t m_size = 0;

        std::map<std::pair<unsigned, hid::Control::Type>, std::size_t> m_reportSizes;
        std::vector<hid::Collection*> m_collectionStack;

        uint8_t* fetchItem(uint8_t* start, uint8_t* end);
        void parseMainItem();
        void openCollection();
        void closeCollection();
        void addField(libhidx::hid::Control::Type reportType);

        void parseGlobalItem();
        void parseLocalItem();
        void parseReservedItem();

    };

}
#endif //LIBHIDX_PARSER2
