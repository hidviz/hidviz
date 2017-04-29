#include "WaitDialog.hh"

#include "ui_WaitDialog.h"

#include <QTimer>

namespace hidviz {
    WaitDialog::WaitDialog(std::chrono::milliseconds interval, std::function<bool()> predicate, QWidget* parent) : QDialog{
        parent}, ui{new Ui::WaitDialog} {

        ui->setupUi(this);

        connect(ui->abortButton, &QPushButton::pressed, this, &WaitDialog::reject);

        auto timer = new QTimer{this};
        connect(timer, &QTimer::timeout, [this, predicate](){
            if(predicate()){
                accept();
            }
        });

        timer->start(interval.count());
    }
}
