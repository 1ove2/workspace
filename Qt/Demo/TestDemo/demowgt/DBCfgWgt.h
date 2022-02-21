#ifndef DBCFGWGT_H
#define DBCFGWGT_H

#include <QDialog>

namespace Ui {
class DBCfgWgt;
}

class DBCfgWgt : public QDialog
{
    Q_OBJECT

public:
    explicit DBCfgWgt(QWidget *parent = 0);
    ~DBCfgWgt();

private:
    Ui::DBCfgWgt *ui;
};

#endif // DBCFGWGT_H
