#ifndef REALTIMEDATAWGT_H
#define REALTIMEDATAWGT_H

#include <QWidget>

namespace Ui {
class RealTimeDataWgt;
}

class RealTimeDataWgt : public QWidget
{
    Q_OBJECT

public:
    explicit RealTimeDataWgt(QWidget *parent = 0);
    ~RealTimeDataWgt();

private:
    Ui::RealTimeDataWgt *ui;
};

#endif // REALTIMEDATAWGT_H
