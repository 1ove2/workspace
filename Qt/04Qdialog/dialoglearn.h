#ifndef DIALOGLEARN_H
#define DIALOGLEARN_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <QColorDialog>
#include <QPalette>
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

private:
    Ui::DialogLearn *ui;
};
#endif // DIALOGLEARN_H
