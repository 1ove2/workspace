#ifndef USERMANAGEWGT_H
#define USERMANAGEWGT_H

#include <QDialog>

namespace Ui {
class UserManageWgt;
}

class UserManageWgt : public QDialog
{
    Q_OBJECT

public:
    explicit UserManageWgt(QWidget *parent = 0);
    ~UserManageWgt();

private:
    Ui::UserManageWgt *ui;
};

#endif // USERMANAGEWGT_H
