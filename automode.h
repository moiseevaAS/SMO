#ifndef AUTOMODE_H
#define AUTOMODE_H

#include <QWidget>

#include "interpreter.h"
#include "ui_automode.h"

namespace Ui { class AutoMode; }

class AutoMode : public QWidget {
    Q_OBJECT

public:

    explicit AutoMode(QWidget *parent = nullptr);
    ~AutoMode();
    void setInterp(MoiseevaAS_::Interpreter *value);
    void interpretation();

private slots:

    void on_buttonRefresh_clicked();

    void on_AutoMode_customContextMenuRequested(const QPoint &pos);

private:

    Ui::AutoMode *ui;
    MoiseevaAS_::Interpreter *interpreter;

};

#endif
