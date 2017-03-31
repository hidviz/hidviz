#include "Window.hh"

#include "ui_Window.h"

#include "TreeModel.hh"
#include "DeviceSelector.hh"
#include "hid/Collection.hh"

#include "libhidx/hid/Collection.hh"
#include "libhidx/hid/Control.hh"

#include <QSettings>

#include <cassert>

namespace hidviz {

    Window::Window() : QWidget{}, ui{new Ui::Window} {
        ui->setupUi(this);

        QSettings settings{"hidviz"};
        restoreGeometry(settings.value( "geometry", saveGeometry() ).toByteArray());
        move(settings.value( "pos", pos() ).toPoint());
        resize(settings.value( "size", size() ).toSize());

        connect(ui->selectDeviceButton, &QToolButton::clicked, this, &Window::openDeviceSelector);
    }

    Window::~Window() {
        delete ui->deviceView->model();
        ui->deviceView->setModel(nullptr);
        delete ui;
    }

    void Window::openDeviceSelector() {
        auto dialog = new DeviceSelector;
        dialog->show();
        connect(dialog, &DeviceSelector::deviceSelected, this,
                &Window::selectDevice);
        connect(dialog, &DeviceSelector::listCleared, this, &Window::clearModel);
    }

    void Window::selectDevice(libhidx::Interface& interface) {
        assert(interface.isHid());

        if(m_selectedInterface){
            m_selectedInterface->stopReading();
        }

        m_selectedInterface = &interface;

        connect(this, &Window::dataRead, this, &Window::updateData);
        interface.setReadingListener([this]{emit dataRead();});
        interface.beginReading();

        ui->titleLabel->setText(QString::fromStdString(interface.getName()));

        m_model = new TreeModel{interface.getHidReportDesc()};
        ui->deviceView->setModel(m_model);
        m_model->forEach([this](const QModelIndex& index){
            ui->deviceView->expand(index);
            if(index.column() == 0){
                return;
            }
            auto item = static_cast<libhidx::hid::Item*>(index.data(Qt::UserRole).value<void*>());

            if(item->m_collection){
                auto collection = new hid::Collection{static_cast<libhidx::hid::Collection*>(item)};
                ui->deviceView->setIndexWidget(index, collection);
            } else if(item->m_control){
                auto control = new hid::Control{static_cast<libhidx::hid::Control*>(item)};
                ui->deviceView->setIndexWidget(index, control);
            } else {
                assert(false);
            }

        });
    }

    void Window::closeEvent(QCloseEvent* event) {
        QSettings settings{"hidviz"};
        settings.setValue( "geometry", saveGeometry() );
        settings.setValue( "maximized", isMaximized() );
        if ( !isMaximized() ) {
            settings.setValue( "pos", pos() );
            settings.setValue( "size", size() );
        }
        QWidget::closeEvent(event);
    }

    void Window::updateData() {
        m_model->forEach([this](const QModelIndex& i){
            auto w = dynamic_cast<hid::Control*>(ui->deviceView->indexWidget(i));
            if(!w){
                return;
            }
            w->updateData();
        });
    }

    void Window::clearModel() {
        delete ui->deviceView->model();
        ui->deviceView->setModel(nullptr);
    }
}
