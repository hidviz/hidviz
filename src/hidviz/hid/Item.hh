#ifndef HIDVIZ_ITEM_HH
#define HIDVIZ_ITEM_HH

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QLabel;

namespace hidviz {
namespace hid {
    class Item : public QWidget {
    Q_OBJECT
    public:
        Item();
        virtual ~Item(){}

    protected:
        QLabel* m_name;
        QLabel* m_usage;
        QWidget *m_content;

    private:
        QVBoxLayout* m_mainLayout;
        QHBoxLayout* m_headerLayout;

    public slots:
        virtual void expand(int newState);

    };
}}


#endif //HIDVIZ_ITEM_HH
