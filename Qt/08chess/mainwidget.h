#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <chess.h>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWidget *ui;
    chess chessmap;
};

#endif // MAINWIDGET_H
