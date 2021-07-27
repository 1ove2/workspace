#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVariant>
#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void showFrom();
    void showText();

public:
    QGridLayout *girdLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *line_name;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *HorizontalLayout_2;
    QLabel *label_2;
    QLineEdit *line_Pwd;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_Login;
    QPushButton *btn_Cancel;
    QSpacerItem *verticalSpacer_2;
};
#endif // WIDGET_H
