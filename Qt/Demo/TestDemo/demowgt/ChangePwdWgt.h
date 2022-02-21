#ifndef CHANGEPWDWGT_H
#define CHANGEPWDWGT_H

#include <QDialog>

namespace Ui {
class ChangePwdWgt;
}

class ChangePwdWgt : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePwdWgt(QWidget *parent = 0);
    ~ChangePwdWgt();

private:
    Ui::ChangePwdWgt *ui;
};

#endif // CHANGEPWDWGT_H
