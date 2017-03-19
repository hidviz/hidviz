#include "Control.hh"

#include <QLabel>
#include <libhidx/hid/Control.hh>

namespace hidviz{
namespace hid {
    Control::Control(libhidx::hid::Control* control) : Item{}, m_control{control} {
        const char *types[] = {
            "Input", "Output", "Feature"
        };

        m_name->setText(types[m_control->m_reportType]);

        m_usage->setText("Placeholder");
    }
}
}
