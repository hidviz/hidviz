#ifndef HIDVIZ_ITEM_HH
#define HIDVIZ_ITEM_HH

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;

namespace hidviz {
namespace hid {
    class Item : public QWidget {
    public:
        Item();
        virtual ~Item(){}
        void setContent(QWidget* widget);

    protected:
        QLabel* m_name;
        QLabel* m_usage;
        QWidget *m_content;

    private:
        QGridLayout* m_mainLayout;

    };
}}


#endif //HIDVIZ_ITEM_HH
