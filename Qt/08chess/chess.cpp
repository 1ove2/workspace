#include "chess.h"

chess::chess(QWidget *parent)
    : QWidget(parent)
{
    initChessState();
    setChessPath(":/new/image/board.jpg",
                 ":/new/image/black.png",
                 ":/new/image/white.png");
}

chess::~chess()
{
}

void chess::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QPen mypen;
    mypen.setWidth(4);
    mypen.setColor(Qt::black);
    mypen.setStyle(Qt::SolidLine);
    painter.setPen(mypen);
    //画背景图
    painter.drawPixmap(this->rect(),QPixmap(this->backgroundPath));
    //画棋盘
    for(int i = 0; i <= 8; i++){
        painter.drawLine(start_x, start_y+i*this->grid_h,start_x+8*this->grid_w, start_y+i*this->grid_h);

        painter.drawLine(start_x+i*this->grid_w,start_y,start_x+i*this->grid_w,start_y+8*this->grid_h);
    }
    //画棋子
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(this->chessState[i][j] == this->White){
                painter.drawPixmap(start_x+grid_w*i,start_y+grid_h*j,grid_w,grid_h,QPixmap(this->whitePath));
            }
            else if(this->chessState[i][j] == this->Black){
                painter.drawPixmap(start_x+grid_w*i,start_y+grid_h*j,grid_w,grid_h,QPixmap(this->blackPath));
            }
        }
    }
}

void chess::resizeEvent(QResizeEvent *event){

    this->grid_w = this->width()/10;
    this->grid_h = this->height()/10;
    this->start_x = this->grid_w;
    this->start_y = this->grid_h;
}

void chess::initChessState(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            this->chessState[i][j] = this->Empty;
        }
    }
    // 将棋盘中间的4个空间分别标志位2黑子2白子
    this->chessState[3][3] = this->Black;
    this->chessState[3][4] = this->White;
    this->chessState[4][3] = this->White;
    this->chessState[4][4] = this->Black;
}

void chess::mousePressEvent(QMouseEvent *event){
    int x = event->x();
    int y = event->y();
    if (x>start_x && x < (start_x + grid_w*8)&&(y > start_y && y <(start_y + 8*grid_h))){
        int i = (x-start_x)/grid_w;
        int j = (y-start_y)/grid_h;

        emit this->pressPoint(i,j);
    }
}

void chess::setChessManInfo(void *p){
    memcpy(this->chessState,p,sizeof(int)*64);
    this->update();
}

void chess::setChessPath(QString Path, QString bPath, QString wPath){
    this->backgroundPath = Path;
    this->blackPath = bPath;
    this->whitePath = wPath;
}
