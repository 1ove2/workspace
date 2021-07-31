#ifndef FIRSTFORM_H
#define FIRSTFORM_H

#include <QWidget>
#include <secondform.h>
#include <QDebug>
QT_BEGIN_NAMESPACE
namespace Ui { class FirstForm; }
QT_END_NAMESPACE

class FirstForm : public QWidget
{
    Q_OBJECT

public:
    FirstForm(QWidget *parent = nullptr);
    ~FirstForm();

    void Init();

private slots:
    void doProcessClickNext(bool);
    void doProcessSecondReq(QString);

private:
    Ui::FirstForm *ui;
    SecondForm second;

};
#endif // FIRSTFORM_H
