#ifndef CONNECTDLG_H
#define CONNECTDLG_H

#include <QDialog>
#include <QListWidget>

namespace Ui {
class ConnectDlg;
}

class ConnectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectDlg(QWidget *parent = 0);
    ~ConnectDlg();

private:
    Ui::ConnectDlg *ui;
};

#endif // CONNECTDLG_H
