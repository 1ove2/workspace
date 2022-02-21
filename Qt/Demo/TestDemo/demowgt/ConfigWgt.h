#ifndef CONFIGWGT_H
#define CONFIGWGT_H

#include <QDialog>

namespace Ui {
class ConfigWgt;
}

class ConfigWgt : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigWgt(QWidget *parent = 0);
    ~ConfigWgt();

private:
    Ui::ConfigWgt *ui;
};

#endif // CONFIGWGT_H
