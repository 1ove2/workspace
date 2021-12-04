#ifndef CHANWGT_H
#define CHANWGT_H

#include <QWidget>

namespace Ui {
class ChanWgt;
}

class ChanWgt : public QWidget
{
    Q_OBJECT

public:
    explicit ChanWgt(QWidget *parent = 0);
    ~ChanWgt();

private:
    Ui::ChanWgt *ui;
};

#endif // CHANWGT_H
