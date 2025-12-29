#ifndef IP_H
#define IP_H

#include<QMainWindow>
#include<QACtion>
#include<QMenu>
#include<QToolBar>
#include<QImage>
#include<QLabel>
#include <QMouseEvent>
#include <QStatusBar>
#include"gtransform.h"
//#include"mouseevent.h"

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

protected:

    void mouseMoveEvent (QMouseEvent * event);
    void mousePressEvent (QMouseEvent * event);
    void mouseReleaseEvent (QMouseEvent * event);

private slots:
    void showopenFile();
    void bigfile();
    void smallfile();
    void showGTranform();

private:

    GTransform *gwin;
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
    QAction *GAction;

    QLabel *statusLabel;
    QLabel *MousePosLabel;

    int x=300;
    int y=200;


};
#endif // IP_H
