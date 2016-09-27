#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QWidget>

namespace Ui {
class MainPage;
}

class MainWindow;

class MainPage : public QWidget
{
    Q_OBJECT

public:
    explicit MainPage(QWidget *parent = 0);

    void startListening(MainWindow* main);

    ~MainPage();

private:
    Ui::MainPage *ui;
    std::size_t startTime;
};

#endif // MAINPAGE_H
