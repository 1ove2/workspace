#ifndef MYTEST_H
#define MYTEST_H

#include <QWidget>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class mytest; }
QT_END_NAMESPACE

class mytest : public QWidget
{
    Q_OBJECT

public:
    mytest(QWidget *parent = nullptr);
    ~mytest();

private slots:
    void on_bth_Cancel_clicked();

    void on_btn_Login_clicked();

private:
    Ui::mytest *ui;
};
#endif // MYTEST_H
