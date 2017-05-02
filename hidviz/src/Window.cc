#include "Window.hh"

#include "ui_Window.h"

#include "DeviceView.hh"
#include "DeviceSelector.hh"
#include "hid/CollectionWidget.hh"
#include "WaitDialog.hh"
#include "Global.hh"

#include <libhidx/hid/Collection.hh>
#include <libhidx/hid/Control.hh>
#include <libhidx/Interface.hh>
#include <libhidx/LibHidx.hh>

#include <QSettings>
#include <QMessageBox>

#include <cassert>

namespace hidviz {

    Window::Window() : QWidget{}, ui{new Ui::Window} {
        ui->setupUi(this);

        QSettings settings{Global::appName};
        restoreGeometry(settings.value( "geometry", saveGeometry() ).toByteArray());
        move(settings.value( "pos", pos() ).toPoint());
        resize(settings.value( "size", size() ).toSize());

        connect(ui->selectDeviceButton, &QToolButton::clicked, this, &Window::openDeviceSelector);

        auto buttonGroup = new QButtonGroup{this};
        buttonGroup->addButton(ui->descriptorViewButton, 0);
        buttonGroup->addButton(ui->deviceViewButton, 1);
        connect(buttonGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked), ui->content, &QStackedWidget::setCurrentIndex);

        connect(ui->hideInactiveUsagesButton, &QAbstractButton::clicked, this, &Window::updateSettings);
        connect(ui->clampValues, &QAbstractButton::clicked, this, &Window::updateSettings);

        loadSettings();
    }

    Window::~Window() {
        delete ui;
    }

    void Window::openDeviceSelector() {
        auto lib = getLibhidx();

        if(!lib){
            QMessageBox::critical(this, "Cannot connect to hidviz daemon.", "Please try another mode.");
            return;
        }

        auto dialog = new DeviceSelector(*lib);
        dialog->show();
        connect(dialog, &DeviceSelector::deviceSelected, this,
                &Window::selectDevice);
        connect(dialog, &DeviceSelector::listCleared, this, &Window::clearModel);
    }

    void Window::selectDevice(libhidx::Interface& interface) {
        assert(interface.isHid());

        if(&interface == m_selectedInterface){
            // do not open already opened interface
            return;
        }

        if(m_selectedInterface){
            m_selectedInterface->setReadingListener(nullptr);
            m_selectedInterface->stopReading();
        }

        m_selectedInterface = &interface;

        ui->titleLabel->setText(QString::fromStdString(interface.getName()));

        m_deviceView = new DeviceView{interface};

        ui->deviceViewContainer->setWidget(m_deviceView);
        ui->descriptorView->setPlainText(QString::fromStdString(interface.getRawHidReportDesc()));
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

    void Window::clearModel() {
        delete m_deviceView;
        m_deviceView = nullptr;
    }

    libhidx::LibHidx* Window::getLibhidx() {
        using namespace std::chrono;
        if(!m_lib){
            m_lib = std::make_unique<libhidx::LibHidx>();
#ifdef __linux__
            m_lib->connectUnixSocket();
#else
            m_lib->connectLocal();
#endif

            WaitDialog waitDialog{500ms, [this]{return m_lib->doConnect();}, this};
            waitDialog.exec();

            if(waitDialog.result() == WaitDialog::Rejected){
                m_lib.reset(nullptr);
                return nullptr;
            }

            m_lib->init();
        }
        return m_lib.get();
    }

    void Window::updateSettings() {
        QSettings settings{Global::appName};
        settings.setValue(Global::Settings::hideInactiveUsages, ui->hideInactiveUsagesButton->isChecked());
        settings.setValue(Global::Settings::clampValues, ui->clampValues->isChecked());
        if(m_deviceView){
            m_deviceView->updateData();
        }
    }

    void Window::loadSettings() {
        QSettings settings{Global::appName};

        if(settings.contains(Global::Settings::hideInactiveUsages)){
            ui->hideInactiveUsagesButton->setChecked(settings.value(Global::Settings::hideInactiveUsages).toBool());
        }

        if(settings.contains(Global::Settings::clampValues)){
            ui->clampValues->setChecked(settings.value(Global::Settings::clampValues).toBool());
        }
    }
}
