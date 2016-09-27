#include "mainpage.h"
#include "ui_mainpage.h"
#include "main_window.h"

#include <QJsonValue>

MainPage::MainPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainPage),
    startTime()
{
    ui->setupUi(this);
    ui->bwPlot->addGraph();
    ui->bwPlot->addGraph();
    ui->bwPlot->yAxis->setRange(0, 5000);
    ui->bwPlot->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    ui->bwPlot->graph(1)->setPen(QPen(QColor(100, 255, 0)));
}

void MainPage::startListening(MainWindow* main)
{
    startTime = QDateTime::currentDateTime().toTime_t();
    main->registerRIListener("i2p.router.net.bw.inbound.1s", [this](const QJsonValue& result) {
        const int passedTime = QDateTime::currentDateTime().toTime_t() - startTime;
        ui->bwPlot->xAxis->setRange(std::max(passedTime - 60, 0), std::max(passedTime, 60));
        ui->bwPlot->graph(0)->addData(passedTime, result.toInt());
        // Don't replot here, should be handled when updating outbound data
    });
    main->registerRIListener("i2p.router.net.bw.outbound.1s", [this](const QJsonValue& result) {
        const int passedTime = QDateTime::currentDateTime().toTime_t() - startTime;
        ui->bwPlot->graph(1)->addData(passedTime, result.toInt());
        ui->bwPlot->replot();
    });
}

MainPage::~MainPage()
{
    delete ui;
}
