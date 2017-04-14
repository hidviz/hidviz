#ifndef HIDVIZ_USAGEWIDGET_HH
#define HIDVIZ_USAGEWIDGET_HH

#include <QFrame>

class QLabel;
class QLineEdit;

namespace libhidx {
namespace hid {
    class Usage;
    class Control;
}
}

namespace Ui {
    class UsageWidget;
}

namespace hidviz {
namespace hid {

    class UsageWidget : public QFrame {
    Q_OBJECT
    public:
        UsageWidget(libhidx::hid::Usage& usage, QWidget* parent = nullptr);
        virtual ~UsageWidget();
        void updateData();

    private:
        Ui::UsageWidget* ui = nullptr;
        libhidx::hid::Usage& m_usage;
        QLabel* m_label = nullptr;
        QLineEdit* m_lineEdit = nullptr;
        const libhidx::hid::Control& m_control;
        bool m_hideInactive = true;

    signals:
        void dataUpdated();
    public slots:
        void updateVisibilitySettings(bool hideInactive);
    };

}}


#endif //HIDVIZ_USAGEWIDGET_HH
