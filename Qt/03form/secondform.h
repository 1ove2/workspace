#ifndef SECONDFORM_H
#define SECONDFORM_H

#include <QWidget>
#include <thirdform.h>
namespace Ui {
class SecondForm;
}

class SecondForm : public QWidget
{
    Q_OBJECT

public:
    explicit SecondForm(QWidget *parent = nullptr);
    ~SecondForm();

private slots:
    void doProcessClickgoOne();
    void doProcessClickgoThird();
signals:
    void SignalShowOneFrom(QString);

private:
    Ui::SecondForm *ui;
    int clickCount;
    ThirdForm *third;
};

#endif // SECONDFORM_H
