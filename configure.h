#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QWidget>
#include <QErrorMessage>

#include "controller.h"
#include "ui_configure.h"

namespace Ui { class Configure; }

class Configure : public QWidget {
    Q_OBJECT

public:

    explicit Configure(QWidget *parent = nullptr);
    ~Configure();
    void init(MoiseevaAS_::Interpreter *value);

private slots:

    void on_buttonConfigure_clicked();

private:

    Ui::Configure *ui;
    MoiseevaAS_::Controller controller;
    MoiseevaAS_::Interpreter *interpreter;
};

#endif
