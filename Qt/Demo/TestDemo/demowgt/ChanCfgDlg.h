#ifndef CHANCFGDLG_H
#define CHANCFGDLG_H

#include <QDialog>

namespace Ui {
class ChanCfgDlg;
}

class ChanCfgDlg : public QDialog
{
    Q_OBJECT

public:
    explicit ChanCfgDlg(QWidget *parent = 0);
    ~ChanCfgDlg();

private:
    Ui::ChanCfgDlg *ui;
};

#endif // CHANCFGDLG_H
