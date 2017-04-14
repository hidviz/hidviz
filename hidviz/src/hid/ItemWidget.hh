#ifndef HIDVIZ_ITEM_HH
#define HIDVIZ_ITEM_HH

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;

namespace Ui {
    class ItemWidget;
}

namespace hidviz {
namespace hid {
    class ItemWidget : public QWidget {
    public:
        ItemWidget(size_t level);
        virtual ~ItemWidget();
        void appendWidget(QWidget *widget);

    protected:
        void setName(const QString& name);
        void setUsage(const QString& usage);

    private:
        Ui::ItemWidget* ui;
        QList<QWidget*> m_children;

    };
}}


#endif //HIDVIZ_ITEM_HH
