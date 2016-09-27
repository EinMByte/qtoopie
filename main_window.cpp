#include "main_window.h"
#include "ui_main_window.h"

#include <iostream>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    itoopie("http://localhost:7650"),
    timer(this)
{
    ui->setupUi(this);
    itoopie.start([this](bool result) {
        if(!result) {
            statusBar()->showMessage("Failed to authenticate.");
            return;
        }
        statusBar()->showMessage("Authentication succesful.");
        ui->sidebar->startListening(this);
        ui->main->startListening(this);
        // Periodically request updates
        timer.setInterval(DEFAULT_REFRESH_INTERVAL);
        connect(&timer, SIGNAL(timeout()), this, SLOT(updateRI()));
        timer.start();
        // Update information immediately the first time
        updateRI();
    });
}

void MainWindow::registerRIListener(const std::string& param, RIListener listener)
{
    auto pos = std::find(infoParameters.begin(), infoParameters.end(), param);
    if(pos == infoParameters.end())
        infoParameters.push_back(param);
    infoListeners.insert({param, listener});
}

void MainWindow::sendAction(const std::string& action, std::function<void(bool)> callback) {
    I2PControlRequest request;
    request.setMethod("RouterManager");
    request.setID("0");
    request.setParam("Token", itoopie.getToken());
    request.setParam(action, "");

    // TODO: do not always pass true
    itoopie.sendRequest(request, [callback] (const QJsonDocument&) { callback(true); });
}

void MainWindow::setRefreshSpeed(int miliseconds) {
    timer.setInterval(miliseconds);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateRI()
{
    I2PControlRequest request;
    request.setMethod("RouterInfo");
    request.setID("0");
    request.setParam("Token", itoopie.getToken());
    for(const std::string& p : infoParameters)
        request.setParam(p, "");

    std::cout << "Sending RouterInfo request." << std::endl;

    itoopie.sendRequest(request, [this](const QJsonDocument& response) {
        QJsonObject responseObj = response.object()["result"].toObject();
        std::cout << "RouterInfo request received." << std::endl;
        std::cout << response.toJson().toStdString() << std::endl;
        for(const std::string& p : infoParameters) {
            const QJsonValue result = responseObj[QString::fromStdString(p)];
            const auto range = infoListeners.equal_range(p);
            for(auto it = range.first; it != range.second; ++it)
                it->second(result);
        }
    });
}
