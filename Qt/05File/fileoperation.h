#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
QT_BEGIN_NAMESPACE
namespace Ui { class FileOperation; }
QT_END_NAMESPACE

class FileOperation : public QWidget
{
    Q_OBJECT

public:
    FileOperation(QWidget *parent = nullptr);
    ~FileOperation();
private slots:
    void doProcessOpenFile();
    void doProcessSaveFile();
private:
    Ui::FileOperation *ui;
    QFile *myFile;
};
#endif // FILEOPERATION_H
