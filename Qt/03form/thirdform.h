#ifndef THIRDFORM_H
#define THIRDFORM_H

#include <QWidget>
#include <QTimer>
namespace Ui {
class ThirdForm;
}

class ThirdForm : public QWidget
{
    Q_OBJECT

public:
    explicit ThirdForm(QWidget *parent = nullptr);
    ~ThirdForm();

private slots:
    void doProcessTimerOut();

private:
    Ui::ThirdForm *ui;
    int count;
    QTimer *myTimer;

};

#endif // THIRDFORM_H
