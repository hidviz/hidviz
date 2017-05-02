#ifndef HIDVIZ_USAGEWIDGET_HH
#define HIDVIZ_USAGEWIDGET_HH

#include <QFrame>

class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;

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
        QProgressBar* m_label = nullptr;
        QLineEdit* m_lineEdit = nullptr;
        const libhidx::hid::Control& m_control;
        QPushButton* m_button = nullptr;
        int m_clampMinValue = 0;
        int m_clampMaxValue = 0;

    signals:
        void dataUpdated();
    public slots:
        void initBinaryButton(bool enabled);
    };

}}


#endif //HIDVIZ_USAGEWIDGET_HH
