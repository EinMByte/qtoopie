#include "sidebar.h"
#include "ui_sidebar.h"
#include "main_window.h"

#include <iostream>
#include <sstream>
#include <QJsonObject>

SideBar::SideBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SideBar)
{
    ui->setupUi(this);
}

void SideBar::startListening(MainWindow* main)
{
    // Allow slider to change the refresh speed
    connect(ui->refreshSpeedSlider,&QSlider::valueChanged, [this, main] (int value) {
        const int ms = ui->refreshSpeedSlider->maximum() - value;
        main->setRefreshSpeed(ms > 250 ? ms : 250);
    });

    // Enable shutdown and restart buttons
    ui->shutdownButton->setEnabled(true);
    connect(ui->shutdownButton, &QPushButton::clicked, [this, main]() {
        ui->shutdownButton->setEnabled(false);
        main->sendAction("Shutdown", [] (bool) {});
    });
    ui->restartButton->setEnabled(true);
    connect(ui->restartButton, &QPushButton::clicked, [this, main]() {
        ui->restartButton->setEnabled(false);
        main->sendAction("Restart", [] (bool) {});
    });
    ui->reseedButton->setEnabled(true);
    connect(ui->reseedButton, &QPushButton::clicked, [this, main]() {
        ui->reseedButton->setEnabled(false);
        main->sendAction("Reseed", [this] (bool) {
            ui->reseedButton->setEnabled(true);
        });
    });

    // Register listeners
    main->registerRIListener("i2p.router.status", [this](const QJsonValue& result) {
        ui->statusText->setText(result.toString());
    });
    main->registerRIListener("i2p.router.uptime", [this](const QJsonValue& result) {
        const std::size_t seconds = result.toInt() / 1000;
        const std::size_t minutes = seconds / 60;
        const std::size_t hours = minutes / 60;
        std::stringstream ss;
        ss << hours << 'h' << (minutes % 60) << 'm' << (seconds % 60) << 's';
        ui->uptimeText->setText(QString::fromStdString(ss.str()));
    });
    main->registerRIListener("i2p.router.version", [this](const QJsonValue& result) {
        ui->versionText->setText(result.toString());
    });
    main->registerRIListener("i2p.router.net.status", [this](const QJsonValue& result) {
        switch(result.toInt()) {
        case 0:
            ui->netStatusText->setText("OK");
            break;
        case 1:
            ui->netStatusText->setText("Testing");
            break;
        case 2:
            ui->netStatusText->setText("Firewalled");
            break;
        case 3:
            ui->netStatusText->setText("Hidden");
            break;
        case 4:
            ui->netStatusText->setText("Warning: Firewalled & Fast");
            break;
        case 5:
            ui->netStatusText->setText("Warning: Firewalled & Floodfill");
            break;
        case 6:
            ui->netStatusText->setText("Warning: Firewalled & Inbound TCP");
            break;
        case 7:
            ui->netStatusText->setText("Warning: Firewalled & UDP disabled");
            break;
        case 8:
            ui->netStatusText->setText("Error: I2CP");
            break;
        case 9:
            ui->netStatusText->setText("Error: Clock skew");
            break;
        case 10:
            ui->netStatusText->setText("Error: Private TCP address");
            break;
        case 11:
            ui->netStatusText->setText("Error: Symmetric NAT");
            break;
        case 12:
            ui->netStatusText->setText("Error: UDP port in use");
            break;
        case 13:
            ui->netStatusText->setText("Error: No active peers");
            break;
        case 14:
            ui->netStatusText->setText("Error: UDP disabled, TCP unset");
            break;
        default:
            ui->netStatusText->setText("Unknown");
            break;
        }
    });
    main->registerRIListener("i2p.router.net.bw.inbound.1s", [this](const QJsonValue& result) {
        ui->bwInText->setText(QString::number(result.toInt()) + " B/s");
    });
    main->registerRIListener("i2p.router.net.bw.outbound.1s", [this](const QJsonValue& result) {
        ui->bwOutText->setText(QString::number(result.toInt()) + " B/s");
    });
    main->registerRIListener("i2p.router.net.tunnels.participating", [this](const QJsonValue& result) {
        ui->participatingText->setText(QString::number(result.toInt()));
    });
    main->registerRIListener("i2p.router.netdb.activepeers", [this](const QJsonValue& result) {
        ui->activePeersText->setText(QString::number(result.toInt()));
    });
    main->registerRIListener("i2p.router.netdb.fastpeers", [this](const QJsonValue& result) {
        ui->fastPeersText->setText(QString::number(result.toInt()));
    });
    main->registerRIListener("i2p.router.netdb.highcapacitypeers", [this](const QJsonValue& result) {
        ui->highCapPeersText->setText(QString::number(result.toInt()));
    });
    main->registerRIListener("i2p.router.netdb.isreseeding", [this](const QJsonValue& result) {
        ui->reseedingText->setText(result.toBool() ? "Yes" : "No");
    });
    main->registerRIListener("i2p.router.netdb.knownpeers", [this](const QJsonValue& result) {
        ui->knownPeersText->setText(QString::number(result.toInt()));
    });
}

SideBar::~SideBar()
{
    delete ui;
}
