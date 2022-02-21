#ifndef DEVCFGWGT_H
#define DEVCFGWGT_H

#include <QDialog>

namespace Ui {
class DevCfgWgt;
}

class DevCfgWgt : public QDialog
{
    Q_OBJECT

public:
    explicit DevCfgWgt(QWidget *parent = 0);
    ~DevCfgWgt();

private:
    Ui::DevCfgWgt *ui;
};

#endif // DEVCFGWGT_H
