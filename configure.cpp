#include "configure.h"

Configure::Configure(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Configure) { ui->setupUi(this); }

Configure::~Configure() { delete ui; }

void Configure::init(MoiseevaAS_::Interpreter *value) { interpreter = value; }

void Configure::on_buttonConfigure_clicked() {

    if (ui->editAlpha->text().isEmpty() || ui->editBeta->text().isEmpty() || ui->editLambda->text().isEmpty()
             || ui->editNumSourc->text().isEmpty() || ui->editNumBuff->text().isEmpty() || ui->editNumDev->text().isEmpty()
             || ui->editNumReq->text().isEmpty()) {

        (new QErrorMessage(this))->showMessage("Fill in all the fields!");

    } else {

        float alpha = ui->editAlpha->text().toFloat();
        float betta = ui->editBeta->text().toFloat();
        float lambda = ui->editLambda->text().toFloat();
        int sources = ui->editNumSourc->text().toInt();
        int devices = ui->editNumDev->text().toInt();
        int buffers = ui->editNumBuff->text().toInt();
        int requests = ui->editNumReq->text().toInt();

        controller.setAlpha(alpha);
        controller.setBetta(betta);
        controller.setLambda(lambda);
        controller.setSourcesAmount(sources);
        controller.setBuffersAmount(buffers);
        controller.setDevicesAmount(devices);
        controller.setRequestsNumber(requests);
        controller.work(*interpreter);
    }
}

