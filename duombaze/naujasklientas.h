#ifndef NAUJASKLIENTAS_H
#define NAUJASKLIENTAS_H

#include <QDialog>

namespace Ui {
class NaujasKlientas;
}

class NaujasKlientas : public QDialog
{
    Q_OBJECT

public:
    explicit NaujasKlientas(QWidget *parent = 0);
    ~NaujasKlientas();

private:
    Ui::NaujasKlientas *ui;
};

#endif // NAUJASKLIENTAS_H
