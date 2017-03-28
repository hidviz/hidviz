#include "libhidx/Usages.hh"
#include "libhidx/hid/Usage.hh"

namespace libhidx{
namespace hid {
    Usage::Usage(unsigned usageId) : m_value{usageId} {
        m_name = getHidUsageText(usageId);

    }
}
}
