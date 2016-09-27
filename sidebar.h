#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>

namespace Ui {
class SideBar;
}

class MainWindow;

class SideBar : public QWidget
{
    Q_OBJECT

public:
    explicit SideBar(QWidget *parent = 0);

    void startListening(MainWindow* main);

    ~SideBar();

private:
    Ui::SideBar *ui;
};

#endif // SIDEBAR_H
