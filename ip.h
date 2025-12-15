#ifndef IP_H
#define IP_H

#include<QMainWindow>
#include<QACtion>
#include<QMenu>
#include<QToolBar>
#include<QImage>
#include<QLabel>

#include <QMainWindow>

class ip : public QMainWindow
{
    Q_OBJECT

public:
    ip(QWidget *parent = nullptr);
    ~ip();
    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile(QString filename);
private slots:
    void showopenFile();
    void bigfile();
    void smallfile();
private:
    QWidget *central;
    QMenu   *fileMenu;
    QToolBar *fileTool;
    QToolBar *filebig;
    QToolBar *filesmall;
    QImage  img;
    QString filename;
    QLabel  *imgwin;
    QAction *openFileAction;
    QAction *exitAction;
    QAction *big;
    QAction *small;
    int x=300;
    int y=200;


};
#endif // IP_H
