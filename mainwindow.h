#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "stepmode.h"
#include "automode.h"
#include "configure.h"
#include "ui_mainwindow.h"

namespace Ui { class MainWindow; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    Ui::MainWindow *ui;
    Configure *configure;
    AutoMode *autoMode;
    StepMode *stepMode;

};

#endif
