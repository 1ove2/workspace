#ifndef DATAQUERYWGT_H
#define DATAQUERYWGT_H

#include <QWidget>

namespace Ui {
class DataQueryWgt;
}

class DataQueryWgt : public QWidget
{
    Q_OBJECT

public:
    explicit DataQueryWgt(QWidget *parent = 0);
    ~DataQueryWgt();

private:
    Ui::DataQueryWgt *ui;
};

#endif // DATAQUERYWGT_H
