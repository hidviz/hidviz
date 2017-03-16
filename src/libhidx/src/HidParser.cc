#include "libhidx/HidParser.hh"


#include <cstdlib>
#include <cstring>


namespace libhidx {
    static uint16_t get_unaligned_le16(const void* p) {
        return *((uint16_t*) p);
    }

    static uint32_t get_unaligned_le32(const void* p) {
        return *((uint32_t*) p);
    }

//    static uint64_t get_unaligned_le64(const void* p) {
//        return *((uint64_t*) p);
//    }

    void* zalloc(size_t size) {
        void* mem = malloc(size);
        if (!mem) {
            return NULL;
        }
        memset(mem, 0, size);

        return mem;
    }


/* LIST */

    static inline void INIT_LIST_HEAD(struct list_head* list) {
        list->next = list;
        list->prev = list;
    }

    static inline void __list_add(struct list_head* n,
    struct list_head* prev,
    struct list_head* next
    ) {

    next->
    prev = n;
    n->
    next = next;
    n->
    prev = prev;
    prev->
    next = n;
}


static inline void list_add_tail(struct list_head* n,
struct list_head* head
)
{
__list_add(n, head->prev, head);
}


/* MORE HELPERS */

static uint32_t item_udata(struct hid_item* item) {
    switch (item->size) {
        case 1:
            return item->data.u8;
        case 2:
            return item->data.u16;
        case 4:
            return item->data.u32;
    }
    return 0;
}

static int32_t item_sdata(struct hid_item* item) {
    switch (item->size) {
        case 1:
            return item->data.s8;
        case 2:
            return item->data.s16;
        case 4:
            return item->data.s32;
    }
    return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshift-negative-value"
static int32_t snto32(uint32_t value, unsigned n) {
    switch (n) {
        case 8:
            return ((int8_t) value);
        case 16:
            return ((int16_t) value);
        case 32:
            return ((int32_t) value);
    }
    return value & (1 << (n - 1)) ? value | (-1 << n) : value;
}
#pragma GCC diagnostic pop

int32_t hid_snto32(uint32_t value, unsigned n) {
    return snto32(value, n);
}

/* HID */

static int hid_add_usage(struct hid_parser* parser, unsigned usage) {
    if (parser->local.usage_index >= HID_MAX_USAGES) {
//        hid_err(parser->device, "usage index exceeded\n");
        return -1;
    }
    parser->local.usage[parser->local.usage_index] = usage;
    parser->local.collection_index[parser->local.usage_index] =
        parser->collection_stack_ptr ?
        parser->collection_stack[parser->collection_stack_ptr - 1] : 0;
    parser->local.usage_index++;
    return 0;
}

static int open_collection(struct hid_parser* parser, unsigned type) {
    struct hid_collection* collection;
    unsigned usage;

    usage = parser->local.usage[0];

    if (parser->collection_stack_ptr == HID_COLLECTION_STACK_SIZE) {
//        hid_err(parser->device, "collection stack overflow\n");
//        return -EINVAL;
        return -1;
    }

    if (parser->device->maxcollection == parser->device->collection_size) {
        collection = static_cast<hid_collection*>(malloc(sizeof(struct hid_collection) *
                            parser->device->collection_size * 2));
        if (collection == NULL) {
//            hid_err(parser->device, "failed to reallocate collection array\n");
//            return -ENOMEM;
            return -1;
        }
        memcpy(collection, parser->device->collection,
               sizeof(struct hid_collection) *
               parser->device->collection_size);
        memset(collection + parser->device->collection_size, 0,
               sizeof(struct hid_collection) *
               parser->device->collection_size);
        free(parser->device->collection);
        parser->device->collection = collection;
        parser->device->collection_size *= 2;
    }

    parser->collection_stack[parser->collection_stack_ptr++] =
        parser->device->maxcollection;

    collection = parser->device->collection +
                 parser->device->maxcollection++;
    collection->type = type;
    collection->usage = usage;
    collection->level = parser->collection_stack_ptr - 1;

    if (type == HID_COLLECTION_APPLICATION)
        parser->device->maxapplication++;

    return 0;
}

static int close_collection(struct hid_parser* parser) {
    if (!parser->collection_stack_ptr) {
//        hid_err(parser->device, "collection stack underflow\n");
        return -1;
//        return -EINVAL;
    }
    parser->collection_stack_ptr--;
    return 0;
}

struct hid_report*
hid_register_report(struct hid_device* device, unsigned type, unsigned id) {
    struct hid_report_enum* report_enum = device->report_enum + type;
    struct hid_report* report;

    if (id >= HID_MAX_IDS)
        return NULL;
    if (report_enum->report_id_hash[id])
        return report_enum->report_id_hash[id];

    report = static_cast<hid_report*>(zalloc(sizeof(struct hid_report)));
    if (!report)
        return NULL;

    if (id != 0)
        report_enum->numbered = 1;

    report->id = id;
    report->type = type;
    report->size = 0;
    report->device = device;
    report_enum->report_id_hash[id] = report;

    list_add_tail(&report->list, &report_enum->report_list);

    return report;
}

static struct hid_field*
hid_register_field(struct hid_report* report, unsigned usages,
                   unsigned values) {
    struct hid_field* field;

    if (report->maxfield == HID_MAX_FIELDS) {
//        hid_err(report->device, "too many fields in report\n");
        return NULL;
    }

    field = static_cast<hid_field*>(zalloc((sizeof(struct hid_field) +
                    usages * sizeof(struct hid_usage) +
                    values * sizeof(unsigned))));
    if (!field)
        return NULL;

    field->index = report->maxfield++;
    report->field[field->index] = field;
    field->usage = (struct hid_usage*) (field + 1);
    field->value = (int32_t*) (field->usage + usages);
    field->report = report;

    return field;
}

static unsigned
hid_lookup_collection(struct hid_parser* parser, unsigned type) {
    struct hid_collection* collection = parser->device->collection;
    int n;

    for (n = parser->collection_stack_ptr - 1; n >= 0; n--) {
        unsigned index = parser->collection_stack[n];
        if (collection[index].type == type)
            return collection[index].usage;
    }
    return 0; /* we know nothing about this usage type */
}

static int
hid_add_field(struct hid_parser* parser, unsigned report_type, unsigned flags) {
    struct hid_report* report;
    struct hid_field* field;
    unsigned usages;
    unsigned offset;
    unsigned i;

    report = hid_register_report(parser->device, report_type,
                                 parser->global.report_id);
    if (!report) {
//        hid_err(parser->device, "hid_register_report failed\n");
        return -1;
    }

    /* Handle both signed and unsigned cases properly */
    if ((parser->global.logical_minimum < 0 &&
         parser->global.logical_maximum <
         parser->global.logical_minimum) ||
        (parser->global.logical_minimum >= 0 &&
         (uint32_t) parser->global.logical_maximum <
         (uint32_t) parser->global.logical_minimum)) {
//        dbg_hid("logical range invalid 0x%x 0x%x\n",
//                parser->global.logical_minimum,
//                parser->global.logical_maximum);
        return -1;
    }

    offset = report->size;
    report->size += parser->global.report_size * parser->global.report_count;

    if (!parser->local.usage_index) /* Ignore padding fields */
        return 0;

    // max
    usages = parser->local.usage_index > parser->global.report_count
             ? parser->local.usage_index : parser->global.report_count;

//    usages = max_t(unsigned, parser->local.usage_index,
//        parser->global.report_count);

    field = hid_register_field(report, usages, parser->global.report_count);
    if (!field)
        return 0;

    field->physical = hid_lookup_collection(parser, HID_COLLECTION_PHYSICAL);
    field->logical = hid_lookup_collection(parser, HID_COLLECTION_LOGICAL);
    field->application = hid_lookup_collection(parser,
                                               HID_COLLECTION_APPLICATION);

    for (i = 0; i < usages; i++) {
        unsigned j = i;
        /* Duplicate the last usage we parsed if we have excess values */
        if (i >= parser->local.usage_index)
            j = parser->local.usage_index - 1;
        field->usage[i].hid = parser->local.usage[j];
        field->usage[i].collection_index =
            parser->local.collection_index[j];

        //TODO: most posibbly not needed, delete me
        field->usage[i].usage_index = i;
    }

    field->maxusage = usages;
    field->flags = flags;
    field->report_offset = offset;
    field->report_type = report_type;
    field->report_size = parser->global.report_size;
    field->report_count = parser->global.report_count;
    field->logical_minimum = parser->global.logical_minimum;
    field->logical_maximum = parser->global.logical_maximum;
    field->physical_minimum = parser->global.physical_minimum;
    field->physical_maximum = parser->global.physical_maximum;
    field->unit_exponent = parser->global.unit_exponent;
    field->unit = parser->global.unit;

    return 0;
}

static int hid_parser_main(struct hid_parser* parser, struct hid_item* item) {
    uint32_t data;
    int ret;

    data = item_udata(item);

    switch (item->tag) {
        case HID_MAIN_ITEM_TAG_BEGIN_COLLECTION:
            ret = open_collection(parser, data & 0xff);
            break;
        case HID_MAIN_ITEM_TAG_END_COLLECTION:
            ret = close_collection(parser);
            break;
        case HID_MAIN_ITEM_TAG_INPUT:
            ret = hid_add_field(parser, HID_INPUT_REPORT, data);
            break;
        case HID_MAIN_ITEM_TAG_OUTPUT:
            ret = hid_add_field(parser, HID_OUTPUT_REPORT, data);
            break;
        case HID_MAIN_ITEM_TAG_FEATURE:
            ret = hid_add_field(parser, HID_FEATURE_REPORT, data);
            break;
        default:
//            hid_err(parser->device, "unknown main item tag 0x%x\n", item->tag);
            ret = 0;
    }

    memset(&parser->local, 0,
           sizeof(parser->local));    /* Reset the local parser environment */

    return ret;
}

static int hid_parser_global(struct hid_parser* parser, struct hid_item* item) {
    int32_t raw_value;
    switch (item->tag) {
        case HID_GLOBAL_ITEM_TAG_PUSH:

            if (parser->global_stack_ptr == HID_GLOBAL_STACK_SIZE) {
//                hid_err(parser->device, "global environment stack overflow\n");
                return -1;
            }

            memcpy(parser->global_stack + parser->global_stack_ptr++,
                   &parser->global, sizeof(struct hid_global));
            return 0;

        case HID_GLOBAL_ITEM_TAG_POP:

            if (!parser->global_stack_ptr) {
//                hid_err(parser->device, "global environment stack underflow\n");
                return -1;
            }

            memcpy(&parser->global, parser->global_stack +
                                    --parser->global_stack_ptr,
                   sizeof(struct hid_global));
            return 0;

        case HID_GLOBAL_ITEM_TAG_USAGE_PAGE:
            parser->global.usage_page = item_udata(item);
            return 0;

        case HID_GLOBAL_ITEM_TAG_LOGICAL_MINIMUM:
            parser->global.logical_minimum = item_sdata(item);
            return 0;

        case HID_GLOBAL_ITEM_TAG_LOGICAL_MAXIMUM:
            if (parser->global.logical_minimum < 0)
                parser->global.logical_maximum = item_sdata(item);
            else
                parser->global.logical_maximum = item_udata(item);
            return 0;

        case HID_GLOBAL_ITEM_TAG_PHYSICAL_MINIMUM:
            parser->global.physical_minimum = item_sdata(item);
            return 0;

        case HID_GLOBAL_ITEM_TAG_PHYSICAL_MAXIMUM:
            if (parser->global.physical_minimum < 0)
                parser->global.physical_maximum = item_sdata(item);
            else
                parser->global.physical_maximum = item_udata(item);
            return 0;

        case HID_GLOBAL_ITEM_TAG_UNIT_EXPONENT:
            /* Many devices provide unit exponent as a two's complement
             * nibble due to the common misunderstanding of HID
             * specification 1.11, 6.2.2.7 Global Items. Attempt to handle
             * both this and the standard encoding. */
            raw_value = item_sdata(item);
            if (!(raw_value & 0xfffffff0))
                parser->global.unit_exponent = hid_snto32(raw_value, 4);
            else
                parser->global.unit_exponent = raw_value;
            return 0;

        case HID_GLOBAL_ITEM_TAG_UNIT:
            parser->global.unit = item_udata(item);
            return 0;

        case HID_GLOBAL_ITEM_TAG_REPORT_SIZE:
            parser->global.report_size = item_udata(item);
            if (parser->global.report_size > 128) {
//                hid_err(parser->device, "invalid report_size %d\n",
//                        parser->global.report_size);
                return -1;
            }
            return 0;

        case HID_GLOBAL_ITEM_TAG_REPORT_COUNT:
            parser->global.report_count = item_udata(item);
            if (parser->global.report_count > HID_MAX_USAGES) {
//                hid_err(parser->device, "invalid report_count %d\n",
//                        parser->global.report_count);
                return -1;
            }
            return 0;

        case HID_GLOBAL_ITEM_TAG_REPORT_ID:
            parser->global.report_id = item_udata(item);
            if (parser->global.report_id == 0 ||
                parser->global.report_id >= HID_MAX_IDS) {
//                hid_err(parser->device, "report_id %u is invalid\n",
//                        parser->global.report_id);
                return -1;
            }
            return 0;

        default:
//            hid_err(parser->device, "unknown global tag 0x%x\n", item->tag);
            return -1;
    }
}

static int hid_parser_local(struct hid_parser* parser, struct hid_item* item) {
    uint32_t data;
    unsigned n;
    uint32_t count;

    data = item_udata(item);

    switch (item->tag) {
        case HID_LOCAL_ITEM_TAG_DELIMITER:

            if (data) {
                /*
                 * We treat items before the first delimiter
                 * as global to all usage sets (branch 0).
                 * In the moment we process only these global
                 * items and the first delimiter set.
                 */
                if (parser->local.delimiter_depth != 0) {
//                    hid_err(parser->device, "nested delimiters\n");
                    return -1;
                }
                parser->local.delimiter_depth++;
                parser->local.delimiter_branch++;
            } else {
                if (parser->local.delimiter_depth < 1) {
//                    hid_err(parser->device, "bogus close delimiter\n");
                    return -1;
                }
                parser->local.delimiter_depth--;
            }
            return 0;

        case HID_LOCAL_ITEM_TAG_USAGE:

            if (parser->local.delimiter_branch > 1) {
//                dbg_hid("alternative usage ignored\n");
                return 0;
            }

            if (item->size <= 2)
                data = (parser->global.usage_page << 16) + data;

            return hid_add_usage(parser, data);

        case HID_LOCAL_ITEM_TAG_USAGE_MINIMUM:

            if (parser->local.delimiter_branch > 1) {
//                dbg_hid("alternative usage ignored\n");
                return 0;
            }

            if (item->size <= 2)
                data = (parser->global.usage_page << 16) + data;

            parser->local.usage_minimum = data;
            return 0;

        case HID_LOCAL_ITEM_TAG_USAGE_MAXIMUM:

            if (parser->local.delimiter_branch > 1) {
//                dbg_hid("alternative usage ignored\n");
                return 0;
            }

            if (item->size <= 2)
                data = (parser->global.usage_page << 16) + data;

            count = data - parser->local.usage_minimum;

            // TODO: expand this dynamically...
            if (count + parser->local.usage_index >= HID_MAX_USAGES) {
                /*
                 * We do not warn if the name is not set, we are
                 * actually pre-scanning the device.
                 */
                data = HID_MAX_USAGES - parser->local.usage_index +
                       parser->local.usage_minimum - 1;
                if (data <= 0) {
//                    hid_err(parser->device,
//                            "no more usage index available\n");
                    return -1;
                }
            }

            for (n = parser->local.usage_minimum; n <= data; n++)
                if (hid_add_usage(parser, n)) {
//                    dbg_hid("hid_add_usage failed\n");
                    return -1;
                }
            return 0;

        default:

//            dbg_hid("unknown local item tag 0x%x\n", item->tag);
            return 0;
    }
    return 0;
}

static int
hid_parser_reserved(struct hid_parser*, struct hid_item*) {
//    dbg_hid("reserved item type, tag 0x%x\n", item->tag);
    // TODO
    return 0;
}

static uint8_t*
fetch_item(uint8_t* start, uint8_t* end, struct hid_item* item) {
    uint8_t b;

    if ((end - start) <= 0)
        return NULL;

    b = *start++;

    item->type = (uint8_t) ((b >> 2) & 3);
    item->tag = (uint8_t) ((b >> 4) & 15);

    if (item->tag == HID_ITEM_TAG_LONG) {

        item->format = HID_ITEM_FORMAT_LONG;

        if ((end - start) < 2)
            return NULL;

        item->size = *start++;
        item->tag = *start++;

        if ((end - start) < item->size)
            return NULL;

        item->data.longdata = start;
        start += item->size;
        return start;
    }

    item->format = HID_ITEM_FORMAT_SHORT;
    item->size = (uint8_t) (b & 3);

    switch (item->size) {
        case 0:
            return start;

        case 1:
            if ((end - start) < 1)
                return NULL;
            item->data.u8 = *start++;
            return start;

        case 2:
            if ((end - start) < 2)
                return NULL;
            item->data.u16 = get_unaligned_le16(start);
            start = (uint8_t*) ((uint16_t*) start + 1);
            return start;

        case 3:
            item->size++;
            if ((end - start) < 4)
                return NULL;
            item->data.u32 = get_unaligned_le32(start);
            start = (uint8_t*) ((uint32_t*) start + 1);
            return start;
    }

    return NULL;
}

static void hid_free_report(struct hid_report* report) {
    unsigned n;

    for (n = 0; n < report->maxfield; n++)
        free(report->field[n]);
    free(report);
}

static void hid_close_report(struct hid_device* device) {
    unsigned i, j;

    for (i = 0; i < HID_REPORT_TYPES; i++) {
        struct hid_report_enum* report_enum = device->report_enum + i;

        for (j = 0; j < HID_MAX_IDS; j++) {
            struct hid_report* report = report_enum->report_id_hash[j];
            if (report)
                hid_free_report(report);
        }
        memset(report_enum, 0, sizeof(*report_enum));
        INIT_LIST_HEAD(&report_enum->report_list);
    }

//    free(device->rdesc);
    device->rdesc = NULL;
    device->rsize = 0;

    free(device->collection);
    device->collection = NULL;
    device->collection_size = 0;
    device->maxcollection = 0;
    device->maxapplication = 0;

    device->status &= ~HID_STAT_PARSED;
}


int hid_open_report(struct hid_device* device, uint8_t* start, unsigned size) {
    struct hid_parser* parser;
    struct hid_item item;
//    unsigned int size;
//    uint8_t *start;
//    uint8_t *buf;
    uint8_t* end;
//    int ret;
    int ret = 1;
    static int (* dispatch_type[])(struct hid_parser* parser,
                                   struct hid_item* item) = {
        hid_parser_main,
        hid_parser_global,
        hid_parser_local,
        hid_parser_reserved
    };

    for (int i = 0; i < HID_REPORT_TYPES; i++) {
        struct hid_report_enum* report_enum = device->report_enum + i;
        INIT_LIST_HEAD(&report_enum->report_list);
    }

    device->rdesc = start;
    device->rsize = size;

    parser = static_cast<hid_parser*>(zalloc(sizeof(struct hid_parser)));
    if (!parser) {
        // TODO
//        ret = -ENOMEM;
        goto err;
    }

    parser->device = device;

    end = start + size;

    device->collection = static_cast<hid_collection*>(calloc(HID_DEFAULT_NUM_COLLECTIONS,
                                sizeof(struct hid_collection)));
    if (!device->collection) {
        // TODO
        goto err;
    }
    device->collection_size = HID_DEFAULT_NUM_COLLECTIONS;

    // TODO
//    ret = -EINVAL;
    while ((start = fetch_item(start, end, &item)) != NULL) {

        if (item.format != HID_ITEM_FORMAT_SHORT) {
//            hid_err(device, "unexpected long global item\n");
            goto err;
        }

        if (dispatch_type[item.type](parser, &item)) {
//            hid_err(device, "item %u %u %u %u parsing failed\n",
//                    item.format, (unsigned)item.size,
//                    (unsigned)item.type, (unsigned)item.tag);
            goto err;
        }

        if (start == end) {
            if (parser->collection_stack_ptr) {
//                hid_err(device, "unbalanced collection at end of report description\n");

                goto err;
            }
            if (parser->local.delimiter_depth) {
//                hid_err(device, "unbalanced delimiter at end of report description\n");
                goto err;
            }
            free(parser);
            device->status |= HID_STAT_PARSED;
            return 0;
        }
    }

//    hid_err(device, "item fetching failed at offset %d\n", (int)(end - start));
    err:
    free(parser);
    hid_close_report(device);
    return ret;
}

}
