#ifndef NAUJASEKA_H
#define NAUJASEKA_H

#include <QDialog>

namespace Ui {
class NaujasEka;
}

class NaujasEka : public QDialog
{
    Q_OBJECT

public:
    explicit NaujasEka(QWidget *parent = 0);
    ~NaujasEka();

private:
    Ui::NaujasEka *ui;
};

#endif // NAUJASEKA_H
