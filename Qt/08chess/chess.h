#ifndef CHESS_H
#define CHESS_H

#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QPaintEvent>
#include <QDebug>
class chess : public QWidget
{
    Q_OBJECT

public:
    chess(QWidget *parent = nullptr);
    ~chess();

    enum GridState{Empty, Black, White};
    void initChessState();
    void setChessManInfo(void *p);
    void setChessPath(QString Path = "",QString bPath = "",QString wPath = "");

protected:
    //绘图事件,画棋盘棋子
    void paintEvent(QPaintEvent *event);
    //resize事件,自动调整界面棋子棋盘
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
signals:
    void pressPoint(int x, int y);

private:
    //棋子宽度高度
    int grid_w ,grid_h;
    //棋盘起点坐标
    int start_x, start_y;
    int chessState[8][8];
    QString backgroundPath,blackPath,whitePath;
};

#endif // CHESS_H
