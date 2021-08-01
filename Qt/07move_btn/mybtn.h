#ifndef MYBTN_H
#define MYBTN_H

#include <QWidget>
#include <QPushButton>
#include <QTime>
class mybtn : public QPushButton
{
    Q_OBJECT
public:
    explicit mybtn(QWidget *parent = nullptr);
protected:
    void mouseMoveEvent(QMouseEvent *);

signals:

};

#endif // MYBTN_H
