#ifndef DIALOGLEARN_H
#define DIALOGLEARN_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QColorDialog>
#include <QPalette>
#include <QInputDialog>
#include <QProgressDialog>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class DialogLearn; }
QT_END_NAMESPACE

class DialogLearn : public QWidget
{
    Q_OBJECT

public:
    DialogLearn(QWidget *parent = nullptr);
    ~DialogLearn();
private slots:
    void doProcessbtn_msgClicked();
    void doProcessbtn_ClorBoxClicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void doProcessCancel();
    void doProcessTimeOut();

private:
    Ui::DialogLearn *ui;
    QProgressDialog *P_dialog;
    int step;
    QTimer *myTimer;
};
#endif // DIALOGLEARN_H
