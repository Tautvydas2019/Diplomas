#include "naujasklientas.h"
#include "ui_naujasklientas.h"
#include "settings.h"

#include <QString>

NaujasKlientas::NaujasKlientas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NaujasKlientas)
{
    ui->setupUi(this);
    QWidget::setWindowTitle(Settings::TEXT_NAUJASKLIENTAS_NAME);
}

NaujasKlientas::~NaujasKlientas()
{
    delete ui;
}
