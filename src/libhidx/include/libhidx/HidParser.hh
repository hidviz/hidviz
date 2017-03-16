#ifndef HIDVIZ_PARSER2
#define HIDVIZ_PARSER2

#include <cstdint>


namespace libhidx {
/* LIST */

    struct list_head {
        struct list_head* next, * prev;
    };

/* CONSTANTS */


/*
 * HID report descriptor main item tags
 */
#define HID_MAIN_ITEM_TAG_INPUT            8
#define HID_MAIN_ITEM_TAG_OUTPUT        9
#define HID_MAIN_ITEM_TAG_FEATURE        11
#define HID_MAIN_ITEM_TAG_BEGIN_COLLECTION    10
#define HID_MAIN_ITEM_TAG_END_COLLECTION    12

/*
 * HID report descriptor global item tags
 */
#define HID_GLOBAL_ITEM_TAG_USAGE_PAGE        0
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM    1
#define HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM    2
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM    3
#define HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM    4
#define HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT    5
#define HID_GLOBAL_ITEM_TAG_UNIT        6
#define HID_GLOBAL_ITEM_TAG_REPORT_SIZE        7
#define HID_GLOBAL_ITEM_TAG_REPORT_ID        8
#define HID_GLOBAL_ITEM_TAG_REPORT_COUNT    9
#define HID_GLOBAL_ITEM_TAG_PUSH        10
#define HID_GLOBAL_ITEM_TAG_POP            11

/*
 * HID report descriptor local item tags
 */
#define HID_LOCAL_ITEM_TAG_USAGE        0
#define HID_LOCAL_ITEM_TAG_USAGE_MINIMUM    1
#define HID_LOCAL_ITEM_TAG_USAGE_MAXIMUM    2
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_INDEX    3
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_MINIMUM    4
#define HID_LOCAL_ITEM_TAG_DESIGNATOR_MAXIMUM    5
#define HID_LOCAL_ITEM_TAG_STRING_INDEX        7
#define HID_LOCAL_ITEM_TAG_STRING_MINIMUM    8
#define HID_LOCAL_ITEM_TAG_STRING_MAXIMUM    9
#define HID_LOCAL_ITEM_TAG_DELIMITER        10


#define HID_INPUT_REPORT    0
#define HID_OUTPUT_REPORT    1
#define HID_FEATURE_REPORT    2

#define HID_REPORT_TYPES    3


#define HID_COLLECTION_PHYSICAL        0
#define HID_COLLECTION_APPLICATION    1
#define HID_COLLECTION_LOGICAL        2


#define HID_DEFAULT_NUM_COLLECTIONS    16

#define HID_ITEM_FORMAT_SHORT    0
#define HID_ITEM_FORMAT_LONG    1

#define HID_STAT_PARSED        2

#define HID_ITEM_TAG_LONG    15

    struct hid_item {
        unsigned format;
        uint8_t type;
        uint8_t tag;
        uint8_t size;
        union {
            uint8_t* longdata;
            uint8_t u8;
            uint16_t u16;
            uint32_t u32;
            int8_t s8;
            int16_t s16;
            int32_t s32;
        } data;
    };

    struct hid_collection {

        unsigned type;
        unsigned usage;
        unsigned level;
    };

    struct hid_usage {

        unsigned hid;
        unsigned collection_index;
        unsigned usage_index;
    };

    struct hid_field {

        unsigned physical;
        unsigned logical;
        unsigned application;
        struct hid_usage* usage;
        unsigned maxusage;
        unsigned flags;
        unsigned report_offset;
        unsigned report_type;
        unsigned report_size;
        unsigned report_count;
        int32_t logical_minimum;
        int32_t logical_maximum;
        int32_t physical_minimum;
        int32_t physical_maximum;
        int32_t unit_exponent;
        unsigned unit;
        unsigned index;
        int32_t* value;
        struct hid_report* report;
    };

#define HID_MAX_FIELDS 256

    struct hid_report {

        unsigned maxfield;
        struct hid_field* field[HID_MAX_FIELDS];
        unsigned size;
        unsigned id;
        unsigned type;
        struct hid_device* device;
        struct list_head list;
    };

#define HID_MAX_IDS 256

    struct hid_report_enum {

        struct hid_report* report_id_hash[HID_MAX_IDS];
        struct list_head report_list;
        unsigned numbered; //TODO: change to bool
    };

    struct hid_device {
        uint8_t* rdesc;
        unsigned rsize;
        struct hid_collection* collection;
        unsigned collection_size;
        unsigned status;
        struct hid_report_enum report_enum[HID_REPORT_TYPES];
        unsigned maxcollection;
        unsigned maxapplication;
    };

#define HID_MAX_USAGES            12288

    struct hid_local {

        unsigned delimiter_depth;
        unsigned delimiter_branch;
        unsigned usage_minimum;
        unsigned usage_index;
        unsigned usage[HID_MAX_USAGES];
        unsigned collection_index[HID_MAX_USAGES];
    };

    struct hid_global {

        unsigned usage_page;
        int32_t logical_minimum;
        int32_t logical_maximum;
        int32_t physical_minimum;
        int32_t physical_maximum;
        int32_t unit_exponent;
        unsigned unit;
        unsigned report_size;
        unsigned report_count;
        unsigned report_id;
    };

#define HID_GLOBAL_STACK_SIZE 4
#define HID_COLLECTION_STACK_SIZE 4

    struct hid_parser {

        struct hid_device* device;
        unsigned collection_stack_ptr;
        struct hid_local local;
        struct hid_global global;
        struct hid_global global_stack[HID_GLOBAL_STACK_SIZE];
        unsigned global_stack_ptr;
        unsigned collection_stack[HID_COLLECTION_STACK_SIZE];
    };

    int
    hid_open_report(struct hid_device* device, uint8_t* start, unsigned size);

}
#endif //HIDVIZ_PARSER2
