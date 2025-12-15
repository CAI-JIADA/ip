#include "ip.h"
#include<QHBoxLayout>
#include<QMenuBar>
#include<QFileDialog>
#include<QDebug>

ip::ip(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle (QStringLiteral("影像處理"));
    central =new QWidget();
    QHBoxLayout *mainLayout = new QHBoxLayout (central);
    imgwin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300,200);
    initPixmap->fill(QColor(255,255,255));
    imgwin->resize (300,200);
    imgwin->setScaledContents (true);
    imgwin->setPixmap (*initPixmap);
    mainLayout->addWidget(imgwin);
    setCentralWidget (central);
    createActions();
    createMenus();
    createToolBars();
}

ip::~ip()
{

}
void ip::createActions()
{
    openFileAction = new QAction (QStringLiteral("開啟檔案&O"),this);
    openFileAction->setShortcut (tr("Ctrl+O"));
    openFileAction->setStatusTip (QStringLiteral("開智影像檔案"));
    connect (openFileAction, SIGNAL (triggered()), this, SLOT (showopenFile()));
    exitAction = new QAction (QStringLiteral("結束&Q"),this);
    exitAction->setShortcut (tr("Ctrl+Q"));
    exitAction->setStatusTip (QStringLiteral("退出程式"));
    connect (exitAction, SIGNAL (triggered()),this, SLOT (close()));

    big = new QAction (QStringLiteral("放大&B"),this);
    big->setShortcut (tr("Ctrl+B"));
    big->setStatusTip (QStringLiteral("放大"));
    connect (big, SIGNAL (triggered()), this, SLOT (bigfile()));
    small = new QAction (QStringLiteral("縮小&S"),this);
    small->setShortcut (tr("Ctrl+S"));
    small->setStatusTip (QStringLiteral("退出程式"));
    connect (small, SIGNAL (triggered()),this, SLOT (smallfile()));
}
void ip::createMenus()
{
    fileMenu = menuBar()->addMenu (QStringLiteral("檔案&F"));
    fileMenu->addAction (openFileAction);
    fileMenu->addAction(exitAction);

    fileMenu = menuBar()->addMenu (QStringLiteral("工具&P"));
    fileMenu->addAction (big);
    fileMenu->addAction(small);
}
void ip::createToolBars()
{
    fileTool=addToolBar("file");
    fileTool->addAction(openFileAction);

    filebig=addToolBar("bigg");
    filebig->addAction(big);

    filesmall=addToolBar("smalll");
    filesmall->addAction(small);
}
void ip::loadFile(QString filename)
{
    qDebug() <<QString("file name: %1").arg (filename);
    QByteArray ba=filename.toLatin1();
    printf("FN:%s\n", (char *) ba.data());
    img.load(filename);
    imgwin->setPixmap (QPixmap:: fromImage(img));
}
void ip::showopenFile()
{
    filename=QFileDialog::getOpenFileName(this,QStringLiteral("開啟影像"),tr("."),"bmp(*.bmp);;png(*.png)"";;Jpeg(*.jpg)");
    if(!filename.isEmpty())
    {
        if(img.isNull())
        {
            loadFile(filename);
        }
        else
        {
            ip *newIPWin = new ip();
            newIPWin->show();
            newIPWin->loadFile(filename);
        }
    }
}
void ip::bigfile()
{
    QImage bigfile;
    bigfile=img.scaled(img.width()*2,img.width()*2);
    QLabel *ret=new QLabel();
    ret->setPixmap(QPixmap::fromImage(bigfile));
    ret->setWindowTitle(tr("放大結果"));
    ret->show();
}
void ip::smallfile()
{
    QImage smallfile;
    smallfile=img.scaled(img.width()/2,img.width()/2);
    QLabel *ret=new QLabel();
    ret->setPixmap(QPixmap::fromImage(smallfile));
    ret->setWindowTitle(tr("縮小結果"));
    ret->show();
}

