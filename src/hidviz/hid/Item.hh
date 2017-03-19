#ifndef HIDVIZ_ITEM_HH
#define HIDVIZ_ITEM_HH

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;

namespace hidviz {
namespace hid {
    class Item : public QWidget {
    public:
        Item();

    protected:
        QVBoxLayout* m_mainLayout;
        QHBoxLayout* m_headerLayout;
        QLabel* m_name;
        QLabel* m_usage;
    };
}}


#endif //HIDVIZ_ITEM_HH
