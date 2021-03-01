#include "automode.h"

AutoMode::AutoMode(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutoMode) {
    ui->setupUi(this);
}

AutoMode::~AutoMode() { delete ui; }

void AutoMode::setInterp(MoiseevaAS_::Interpreter *value) { interpreter = value; }

void AutoMode::interpretation() {

    const int columnAmount = 8;
    ui->tableSources->setRowCount(interpreter->getProcessedReq().size());
    ui->tableSources->setColumnCount(columnAmount);

    ui->tableSources->setHorizontalHeaderLabels(QStringList() << "Successed"<< "Denied"<< "TIS"<< "TOW"<< "TOP"<< "Disp. TOW"<< "Disp. TOP"<< "Pr. Failure");

    for (int i=0; i < ui->tableSources->rowCount(); i++) {

        ui->tableSources->setVerticalHeaderItem(i, new QTableWidgetItem(("Source " + std::to_string(i+1)).c_str()));
        ui->tableSources->setItem(i, 0, new QTableWidgetItem((std::to_string(interpreter->getProcessedReq().at(i))).c_str()));
        ui->tableSources->setItem(i, 1, new QTableWidgetItem((std::to_string(interpreter->getDeniedReq().at(i))).c_str()));
        ui->tableSources->setItem(i, 2, new QTableWidgetItem((std::to_string(interpreter->getTimeInSystem().at(i))).c_str()));
        ui->tableSources->setItem(i, 3, new QTableWidgetItem((std::to_string(interpreter->getWaitTime().at(i))).c_str()));
        ui->tableSources->setItem(i, 4, new QTableWidgetItem((std::to_string(interpreter->getProcessTime().at(i))).c_str()));
        ui->tableSources->setItem(i, 5, new QTableWidgetItem((std::to_string(interpreter->getDispTOW().at(i))).c_str()));
        ui->tableSources->setItem(i, 6, new QTableWidgetItem((std::to_string(interpreter->getDispTOP().at(i))).c_str()));
        ui->tableSources->setItem(i, 7, new QTableWidgetItem((std::to_string(interpreter->getPFailure().at(i)) + "%").c_str()));
    }

    const int columnAmountDev = 1;
    ui->tableDevices->setRowCount(interpreter->getDeviceLoad().size());
    ui->tableDevices->setColumnCount(columnAmountDev);
    ui->tableDevices->setHorizontalHeaderLabels(QStringList() << "Workload");

    for (int i=0; i < ui->tableDevices->rowCount(); i++) {
        ui->tableDevices->setVerticalHeaderItem(i, new QTableWidgetItem(("Device " + std::to_string(i+1)).c_str()));
        ui->tableDevices->setItem(i, 0, new QTableWidgetItem((std::to_string(interpreter->getDeviceLoad().at(i)) + "%").c_str()));
    }
}

void AutoMode::on_buttonRefresh_clicked() { interpretation(); }

void AutoMode::on_AutoMode_customContextMenuRequested(const QPoint &pos)
{

}
