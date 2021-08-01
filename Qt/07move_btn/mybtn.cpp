#include "mybtn.h"
#include <QMouseEvent>

mybtn::mybtn(QWidget *parent) : QPushButton(parent)
{
    setMouseTracking(true);
    //产生随机种子
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
}

void mybtn::mouseMoveEvent(QMouseEvent *e){
    //QWidget *parentW = this->parentWidget();
    int mouse_x = e->windowPos().x();
    int mouse_y = e->windowPos().y();

    int cur_wid_x = this->x();
    int cur_wid_y = this->y();

    if(mouse_x >= cur_wid_x && (mouse_x <= cur_wid_x + this->width())){
        if(mouse_y >= cur_wid_y && (mouse_y <= cur_wid_y + this->height())){
            QWidget *parentW = this->parentWidget();
            int d_x = qrand()%(parentW->width() - this->width());
            int d_y = qrand()%(parentW->height() - this->height());
            this->move(d_x,d_y);

        }
    }
}
