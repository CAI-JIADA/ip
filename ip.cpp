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
}
void ip::createMenus()
{
    fileMenu = menuBar()->addMenu (QStringLiteral("檔案&F"));
    fileMenu->addAction (openFileAction);
    fileMenu->addAction(exitAction);
}
void ip::createToolBars()
{
    fileTool=addToolBar("file");
    fileTool->addAction(openFileAction);
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
