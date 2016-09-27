#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "i2pcontrol_client.h"

#include <unordered_map>
#include <vector>

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

typedef std::function<void(const QJsonValue&)> RIListener;
constexpr int DEFAULT_REFRESH_INTERVAL = 5000;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void registerRIListener(const std::string& param, RIListener listener);

    void sendAction(const std::string& action, std::function<void(bool)> callback);

    void setRefreshSpeed(int miliseconds);

    ~MainWindow();

private slots:

    void updateRI();

private:
    Ui::MainWindow *ui;
    I2PControlClient itoopie;
    std::vector<std::string> infoParameters;
    std::unordered_multimap<std::string, RIListener> infoListeners;
    QTimer timer;
};

#endif // MAIN_WINDOW_H
