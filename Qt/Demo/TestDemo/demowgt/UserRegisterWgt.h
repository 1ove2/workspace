#ifndef USERREGISTERWGT_H
#define USERREGISTERWGT_H
#include <QDialog>
#include "DBSQLite.h"

namespace Ui {
class UserRegisterWgt;
}

class UserRegisterWgt : public QDialog
{
    Q_OBJECT

public:
    explicit UserRegisterWgt(QWidget *parent = 0);
    ~UserRegisterWgt();

private slots:
    void on_btn_Add_clicked();

    void on_btn_Cancel_clicked();

private:
    Ui::UserRegisterWgt *ui;
    DBSQLite *m_db;
};

#endif // USERREGISTERWGT_H
