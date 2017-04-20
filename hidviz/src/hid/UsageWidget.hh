#ifndef HIDVIZ_USAGEWIDGET_HH
#define HIDVIZ_USAGEWIDGET_HH

#include <QFrame>

class QLabel;
class QLineEdit;
class QPushButton;

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
        explicit UsageWidget(libhidx::hid::Usage& usage, QWidget* parent = nullptr);
        ~UsageWidget() override;
        void updateData();

    private:
        void initInput();
        void initOutput();

        Ui::UsageWidget* ui = nullptr;
        libhidx::hid::Usage& m_usage;
        QLabel* m_label = nullptr;
        QLineEdit* m_lineEdit = nullptr;
        const libhidx::hid::Control& m_control;
        bool m_hideInactive = true;
        QPushButton* m_button;

    signals:
        void dataUpdated();
    public slots:
        void updateVisibilitySettings(bool hideInactive);
        void initBinaryButton(bool enabled);
    };

}}


#endif //HIDVIZ_USAGEWIDGET_HH
