#ifndef STEPMODE_H
#define STEPMODE_H

#include <QWidget>
#include <QThread>
#include <interpreter.h>

#include "ui_stepmode.h"

namespace Ui { class StepMode; }

class StepMode : public QWidget {
    Q_OBJECT

public:
        int lastBuf=0;
        int lastBufb=0;
        int lastDev=0;
        int lastDevb=0;
    explicit StepMode(QWidget *parent = nullptr);
    ~StepMode();
    void setInterp(MoiseevaAS_::Interpreter *value);

private slots:

    void on_buttonRefresh_clicked();
    void on_buttonNext_clicked();
    void on_buttonBack_clicked();

private:

    int currentStep;


    void refreshScene();
    void prepareScene();
    Ui::StepMode *ui;
    MoiseevaAS_::Interpreter *interpreter;

};

#endif
