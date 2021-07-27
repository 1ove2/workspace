#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    showFrom();
    //showText();
}

Widget::~Widget()
{
}
//显示控件
void Widget::showFrom(){
    if(this->objectName().isEmpty()){
        this->setObjectName(QString::fromUtf8("Login View"));
    }
    this->resize(384,309);
    girdLayout = new QGridLayout(this);
    girdLayout->setObjectName(QString::fromUtf8("gridLayout"));
    verticalSpacer  = new QSpacerItem(20,96,QSizePolicy::Minimum,QSizePolicy::Expanding);
    girdLayout->addItem(verticalSpacer,0,1,1,1);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    label = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label"));
    horizontalLayout->addWidget(label);
    line_name = new QLineEdit(this);
    line_name->setObjectName(QString::fromUtf8("line_name"));
    line_name->setFocusPolicy(Qt::StrongFocus);
    horizontalLayout->addWidget(line_name);
    girdLayout->addLayout(horizontalLayout,1,1,1,1);
    horizontalSpacer = new QSpacerItem(90,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    girdLayout->addItem(horizontalSpacer,2,0,1,1);
    HorizontalLayout_2 = new QHBoxLayout();
    HorizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
    label_2 = new QLabel(this);
    label->setObjectName(QString::fromUtf8("label_2"));
    HorizontalLayout_2->addWidget(label_2);
    line_Pwd = new QLineEdit(this);
    line_Pwd->setObjectName(QString::fromUtf8("line_Pwd"));
    line_Pwd->setFocusPolicy(Qt::StrongFocus);
    line_Pwd->setEchoMode(QLineEdit::Password);
    HorizontalLayout_2->addWidget(line_Pwd);
    girdLayout->addLayout(HorizontalLayout_2,2,1,1,1);
    horizontalSpacer_2 = new QSpacerItem(90,20,QSizePolicy::Expanding,QSizePolicy::Minimum);
    girdLayout->addItem(horizontalSpacer_2,2,2,1,1);
    horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));

    btn_Login = new QPushButton(this);
    btn_Login->setObjectName(QString::fromUtf8("btn_Login"));
    btn_Login->setFocusPolicy(Qt::TabFocus);
    horizontalLayout_3->addWidget(btn_Login);

    btn_Cancel = new QPushButton(this);
    btn_Cancel->setObjectName(QString::fromUtf8("btn_Cancel"));
    btn_Cancel->setFocusPolicy(Qt::TabFocus);
    horizontalLayout_3->addWidget(btn_Cancel);

    girdLayout->addLayout(horizontalLayout_3,3,1,1,1);
    verticalSpacer_2 = new QSpacerItem(20,95,QSizePolicy::Minimum, QSizePolicy::Expanding);
    girdLayout->addItem(verticalSpacer_2,4,1,1,1);

    QWidget::setTabOrder(line_name,line_Pwd);
    QWidget::setTabOrder(line_Pwd,btn_Login);
    QWidget::setTabOrder(btn_Login,btn_Cancel);
    showText();
    QMetaObject::connectSlotsByName(this);
}
//显示字体
void Widget::showText(){
    this->setWindowTitle(QCoreApplication::translate("Login VIEW","Login VIEW",nullptr));
    label->setText(QCoreApplication::translate("Login VIEW", "\350\264\246\345\217\267\357\274\232", nullptr));
    label_2->setText(QCoreApplication::translate("Login VIEW", "\345\257\206\347\240\201\357\274\232", nullptr));
    btn_Login->setText(QCoreApplication::translate("Login VIEW", "\347\231\273\345\275\225", nullptr));
    btn_Cancel->setText(QCoreApplication::translate("Login VIEW", "\345\217\226\346\266\210", nullptr));
}
