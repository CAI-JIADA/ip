#ifndef IP_H
#define IP_H

#include<QMainWindow>
#include<QAction>
#include<QMenu>
#include<QToolBar>
#include<QImage>
#include<QLabel>
#include <QMouseEvent>
#include <QStatusBar>
#include <QRubberBand> // 新增：用於矩形選取的橡皮筋框
#include <QPoint>
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
    QRubberBand *rubberBand = nullptr; // 新增：顯示選取範圍的框
    QPoint selectionOrigin;            // 新增：記錄選取起點
    bool isSelecting = false;          // 新增：標記是否正在拖曳選取


};
#endif // IP_H
