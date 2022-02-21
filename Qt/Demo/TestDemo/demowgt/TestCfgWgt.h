#ifndef TESTCFGWGT_H
#define TESTCFGWGT_H

#include <QDialog>

namespace Ui {
class TestCfgWgt;
}

class TestCfgWgt : public QDialog
{
    Q_OBJECT

public:
    explicit TestCfgWgt(QWidget *parent = 0);
    ~TestCfgWgt();

private:
    Ui::TestCfgWgt *ui;
};

#endif // TESTCFGWGT_H
