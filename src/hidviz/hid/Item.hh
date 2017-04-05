#ifndef HIDVIZ_ITEM_HH
#define HIDVIZ_ITEM_HH

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;

namespace Ui {
    class Item;
}

namespace hidviz {
namespace hid {
    class Item : public QWidget {
    public:
        Item();
        virtual ~Item(){}
        void setContent(QWidget* widget);

    protected:
        void setName(const QString& name);
        void setUsage(const QString& usage);

    private:
        Ui::Item* ui;

    };
}}


#endif //HIDVIZ_ITEM_HH
