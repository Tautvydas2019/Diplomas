#include "naujaseka.h"
#include "ui_naujaseka.h"

NaujasEka::NaujasEka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NaujasEka)
{
    ui->setupUi(this);
}

NaujasEka::~NaujasEka()
{
    delete ui;
}
