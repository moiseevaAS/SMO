#include "stepmode.h"

StepMode::StepMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepMode) {
    ui->setupUi(this);
}

StepMode::~StepMode() {
    delete ui;
}

void StepMode::setInterp(MoiseevaAS_::Interpreter *value) {
    interpreter = value;
}

void StepMode::refreshScene() {

    currentStep = 0;

    ui->textTime->setText("0");
    ui->textStep->setText("0");
    ui->textAllSteps->setText("0");
    ui->textBrowserMessages->clear();
    ui->tableBuffers->clear();
    ui->tableDevices->clear();
}

void StepMode::prepareScene() {

    if (interpreter->getSteps().size() <= 0)
        return;

    const int columnAmountSrc = 3;

    ui->textAllSteps->setText(std::to_string(interpreter->getSteps().size()).c_str());

    ui->tableBuffers->setRowCount(interpreter->getBuffersAmount());
    ui->tableBuffers->setColumnCount(columnAmountSrc);
    ui->tableBuffers->setHorizontalHeaderLabels(QStringList() << "State"<< "Request"<< "Point");
    for (int i=0; i < interpreter->getBuffersAmount(); i++) {
        ui->tableBuffers->setVerticalHeaderItem(i, new QTableWidgetItem(("Buffer " + std::to_string(i+1)).c_str()));
    }

    ui->tableDevices->setRowCount(interpreter->getDevicesAmount());
    ui->tableDevices->setColumnCount(columnAmountSrc);
    ui->tableDevices->setHorizontalHeaderLabels(QStringList() << "State"<< "Request"<< "Point");
    for (int i=0; i < interpreter->getDevicesAmount(); i++) {
        ui->tableDevices->setVerticalHeaderItem(i, new QTableWidgetItem(("Device " + std::to_string(i+1)).c_str()));
    }
}

void StepMode::on_buttonRefresh_clicked() {

    refreshScene();
    prepareScene();
}

void StepMode::on_buttonNext_clicked() {

    if (currentStep + 1 >= interpreter->getSteps().size())
        return;
    if (currentStep < 0){
        currentStep++;
        return;
    }

    currentStep++;

    const std::string free = "Free";
    const std::string busy = "Busy";
    const std::string point = "pointer";

    MoiseevaAS_::Interpreter::Archive stepModel = interpreter->getSteps().at(currentStep);

    ui->textBrowserMessages->setText(stepModel.message.c_str());
    ui->textTime->setText(std::to_string(stepModel.currentTime).c_str());
    ui->textStep->setText(std::to_string(currentStep).c_str());

    for (int i=0; i < ui->tableBuffers->rowCount(); i++) {

        if (stepModel.buffers.at(i) == stepModel.EMPTY) {

            ui->tableBuffers->setItem(i, 0, new QTableWidgetItem(free.c_str()));
            ui->tableBuffers->setItem(i, 1, new QTableWidgetItem("-"));
            ui->tableBuffers->setItem(i, 2, new QTableWidgetItem("-"));
        } else {
            ui->tableBuffers->setItem(i, 0, new QTableWidgetItem(busy.c_str()));
            ui->tableBuffers->setItem(i, 1, new QTableWidgetItem(stepModel.buffers.at(i).c_str()));
            ui->tableBuffers->setItem(i, 2, new QTableWidgetItem("-"));

        }

    }

     for (int i=0; i < ui->tableBuffers->rowCount(); i++) {
        if (lastBuf >= ui->tableBuffers->rowCount() || lastBuf < 0) {
            lastBuf = 0;
        }
        if (stepModel.buffers.at(lastBuf) == stepModel.EMPTY) {
            lastBuf++;} else {
            ui->tableBuffers->setItem(lastBuf, 2, new QTableWidgetItem(point.c_str()));

        break;}
        }



    for (int i=0; i < ui->tableDevices->rowCount(); i++) {
        if (stepModel.devices.at(i) == stepModel.EMPTY) {
            ui->tableDevices->setItem(i, 0, new QTableWidgetItem(free.c_str()));
            ui->tableDevices->setItem(i, 1, new QTableWidgetItem("-"));
            ui->tableDevices->setItem(i, 2, new QTableWidgetItem("-"));
        } else {
            ui->tableDevices->setItem(i, 0, new QTableWidgetItem(free.c_str()));
            ui->tableDevices->setItem(i, 1, new QTableWidgetItem("-"));
             ui->tableDevices->setItem(i, 2, new QTableWidgetItem("-"));
            QThread::msleep(10);
            ui->tableDevices->setItem(i, 0, new QTableWidgetItem(busy.c_str()));
            ui->tableDevices->setItem(i, 1, new QTableWidgetItem(stepModel.devices.at(i).c_str()));
        }
    }
int a=0;

    MoiseevaAS_::Interpreter::Archive laststepModel = interpreter->getSteps().at(currentStep-1);
    for (int i=0; i < ui->tableDevices->rowCount(); i++) {
     if (laststepModel.devices.at(i) != stepModel.devices.at(i)){
         if (stepModel.devices.at(i)!= stepModel.EMPTY) { lastDev = i;
         ui->tableDevices->setItem(lastDev, 2, new QTableWidgetItem(point.c_str()));
     a=1;}
    }
    }
    int aEmpty = 1;
    for (int i=0; i < ui->tableDevices->rowCount(); i++) {
         if (stepModel.devices.at(i) == stepModel.EMPTY) { aEmpty++ ;}

    }


    if (a==0){
        if ( aEmpty <= ui->tableDevices->rowCount()) {
    ui->tableDevices->setItem(lastDev, 2, new QTableWidgetItem(point.c_str()));}
        }

}

void StepMode::on_buttonBack_clicked() {

    if (currentStep <= 0)
        return;
    if (currentStep > interpreter->getSteps().size()) {
        currentStep--;
        return;
    }

    currentStep--;

    const std::string free = "Free";
    const std::string busy = "Busy";
    const std::string point = "pointer";


    MoiseevaAS_::Interpreter::Archive stepModel = interpreter->getSteps().at(currentStep);

    ui->textBrowserMessages->setText(stepModel.message.c_str());
    ui->textTime->setText(std::to_string(stepModel.currentTime).c_str());
    ui->textStep->setText(std::to_string(currentStep).c_str());

    for (int i = 0; i < ui->tableBuffers->rowCount(); i++) {
        if (stepModel.buffers.at(i) == stepModel.EMPTY) {
            ui->tableBuffers->setItem(i, 0, new QTableWidgetItem(free.c_str()));
            ui->tableBuffers->setItem(i, 1, new QTableWidgetItem("-"));
             ui->tableBuffers->setItem(i, 2, new QTableWidgetItem("-"));
        } else {
            ui->tableBuffers->setItem(i, 0, new QTableWidgetItem(busy.c_str()));
            ui->tableBuffers->setItem(i, 1, new QTableWidgetItem(stepModel.buffers.at(i).c_str()));
            ui->tableBuffers->setItem(i, 2, new QTableWidgetItem("-"));
        }
    }
    // lastBufb = 0;
    //
    int ab=0;

        MoiseevaAS_::Interpreter::Archive laststepModelbb = interpreter->getSteps().at(currentStep+1);
        for (int  i=0; i < ui->tableBuffers->rowCount();  i++) {
         if (laststepModelbb.buffers.at(i) != stepModel.buffers.at(i)){
             if (stepModel.buffers.at(i)!= stepModel.EMPTY) { lastBufb = i;
             ui->tableBuffers->setItem(lastBufb, 2, new QTableWidgetItem(point.c_str()));
         ab=1;}
        }
        }
        int abEmpty = 1;
        for (int i=0; i < ui->tableBuffers->rowCount(); i++) {
             if (stepModel.buffers.at(i) == stepModel.EMPTY) { abEmpty++ ;}

        }


      if (ab==0){
         if ( abEmpty <= ui->tableBuffers->rowCount()) {
      ui->tableBuffers->setItem(lastBufb, 2, new QTableWidgetItem(point.c_str()));}
       }
/*

    for (int  i = ui->tableBuffers->rowCount(); i >= 0; i--) {
       if (lastBufb >= ui->tableBuffers->rowCount() || lastBufb < 0) {
           lastBufb = ui->tableBuffers->rowCount();
       }
       if (stepModel.buffers.at(lastBufb) == stepModel.EMPTY) {
           lastBufb--;} else {
           ui->tableBuffers->setItem(lastBufb, 2, new QTableWidgetItem(point.c_str()));

       break;}
       }
       */
    for (int i = 0; i < ui->tableDevices->rowCount(); i++) {
        if (stepModel.devices.at(i) == stepModel.EMPTY) {
            ui->tableDevices->setItem(i, 0, new QTableWidgetItem(free.c_str()));
            ui->tableDevices->setItem(i, 1, new QTableWidgetItem("-"));
            ui->tableDevices->setItem(i, 2, new QTableWidgetItem("-"));
        } else {
            ui->tableDevices->setItem(i, 0, new QTableWidgetItem(busy.c_str()));
            ui->tableDevices->setItem(i, 1, new QTableWidgetItem(stepModel.devices.at(i).c_str()));
            ui->tableDevices->setItem(i, 2, new QTableWidgetItem("-"));
        }
    }
    int a=0;

        MoiseevaAS_::Interpreter::Archive laststepModelb = interpreter->getSteps().at(currentStep+1);
        for (int i=0; i < ui->tableDevices->rowCount(); i++) {
         if (laststepModelb.devices.at(i) != stepModel.devices.at(i)){
             if (stepModel.devices.at(i)!= stepModel.EMPTY) { lastDevb = i;
             ui->tableDevices->setItem(lastDevb, 2, new QTableWidgetItem(point.c_str()));
         a=1;}
        }
        }
        int aEmpty = 1;
        for (int i=0; i < ui->tableDevices->rowCount(); i++) {
             if (stepModel.devices.at(i) == stepModel.EMPTY) { aEmpty++ ;}

        }


        if (a==0){
            if ( aEmpty <= ui->tableDevices->rowCount()) {
        ui->tableDevices->setItem(lastDevb, 2, new QTableWidgetItem(point.c_str()));}
            }


}
