#ifndef PROJECT_WAITDIALOG_HH
#define PROJECT_WAITDIALOG_HH

#include <QDialog>

#include <functional>
#include <chrono>

namespace Ui {
    class WaitDialog;
}
namespace hidviz {
    class WaitDialog : public QDialog {
    public:
        WaitDialog(std::chrono::milliseconds interval, std::function<bool()> predicate, QWidget* parent = nullptr);

    private:
        Ui::WaitDialog* ui;
    };
}


#endif //PROJECT_WAITDIALOG_HH
