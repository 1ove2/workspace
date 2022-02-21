#ifndef CONSCFGWGT_H
#define CONSCFGWGT_H

#include <QWidget>

namespace Ui {
class ConsCfgWgt;
}

class ConsCfgWgt : public QWidget
{
    Q_OBJECT

public:
    explicit ConsCfgWgt(QWidget *parent = 0);
    ~ConsCfgWgt();

private:
    Ui::ConsCfgWgt *ui;
};

#endif // CONSCFGWGT_H
