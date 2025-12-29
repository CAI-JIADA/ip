#include "ip.h"
//#include "mouseevent.h"
#include<QHBoxLayout>
#include<QMenuBar>
#include<QFileDialog>
#include<QDebug>

ip::ip(QWidget *parent)
    : QMainWindow(parent)
{
    statusLabel = new QLabel;
    statusLabel->setText (QStringLiteral("指標位置"));
    statusLabel->setFixedWidth (100);
    MousePosLabel = new QLabel;
    MousePosLabel->setText(tr(" "));
    MousePosLabel->setFixedWidth (100);
    statusBar()->addPermanentWidget (statusLabel);
    statusBar()->addPermanentWidget (MousePosLabel);
    setMouseTracking (true);

    setWindowTitle (QStringLiteral("影像處理"));
    central =new QWidget();
    central->setMouseTracking(true);
    QHBoxLayout *mainLayout = new QHBoxLayout (central);
    imgwin = new QLabel();
    imgwin->setMouseTracking(true);
    QPixmap *initPixmap = new QPixmap(300,200);
    gwin=new GTransform();
    //mwin=new MouseEvent();
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

    GAction = new QAction (QStringLiteral("幾何轉換"),this);
    GAction->setShortcut (tr("Ctrl+G"));
    GAction->setStatusTip (QStringLiteral("影像幾何轉換"));
    connect (GAction, SIGNAL (triggered()), this, SLOT (showGTranform())); // 修正：連結到正確的槽函式
    connect (exitAction, SIGNAL (triggered()),gwin, SLOT (close()));


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
    fileMenu->addAction(GAction);

    fileMenu->addAction(exitAction);

    fileMenu = menuBar()->addMenu (QStringLiteral("工具&P"));
    fileMenu->addAction (big);
    fileMenu->addAction(small);
}
void ip::createToolBars()
{
    fileTool=addToolBar("file");
    fileTool->addAction(openFileAction);
    fileMenu->addAction(GAction);
  ;

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
void ip::showGTranform()
{
    if (!img.isNull())
    gwin->srcImg = img;
    gwin->inWin->setPixmap (QPixmap:: fromImage (gwin->srcImg));
    gwin->show();
}



void ip::mouseMoveEvent (QMouseEvent * event)
{
    const QPoint labelPos = imgwin->mapFromGlobal(event->globalPos()); // 新增：將座標轉為影像區域
    int x = labelPos.x();
    int y = labelPos.y();

    QString str="(" + QString::number(x) + ", " + QString::number(y) +")";
    if(!img.isNull() && x >= 0 && x < imgwin->width() && y >= 0 && y < imgwin->height())
    {
        const double scaleX = static_cast<double>(img.width()) / imgwin->width();
        const double scaleY = static_cast<double>(img.height()) / imgwin->height();
        const int imgX = qBound(0, static_cast<int>(x * scaleX), img.width() - 1);
        const int imgY = qBound(0, static_cast<int>(y * scaleY), img.height() - 1);
        int gray = qGray(img.pixel(imgX,imgY));
        str += ("=" + QString::number(gray));
    }
    MousePosLabel->setText(str);

    if (isSelecting && rubberBand) { // 新增：拖曳時更新選取框
        QRect rect(selectionOrigin, labelPos);
        rubberBand->setGeometry(rect.normalized());
    }
}

void ip::mousePressEvent (QMouseEvent * event)
{
    const QPoint labelPos = imgwin->mapFromGlobal(event->globalPos()); // 新增：以影像區座標為準
    QString str="(" + QString::number(labelPos.x()) + ", " + QString::number(labelPos.y()) +")";
    if (event->button() == Qt::LeftButton)
    {
        statusBar ()->showMessage (QStringLiteral("左鍵:")+str,1000);
        if (!img.isNull() && imgwin->rect().contains(labelPos)) { // 新增：開始矩形選取
            isSelecting = true;
            selectionOrigin = labelPos;
            if (!rubberBand) {
                rubberBand = new QRubberBand(QRubberBand::Rectangle, imgwin);
            }
            rubberBand->setGeometry(QRect(selectionOrigin, QSize()));
            rubberBand->show();
        }
    }
    else if (event->button() == Qt:: RightButton)
    {
        statusBar ()->showMessage (QStringLiteral("右鍵:")+str,1000);
    }
    else if (event->button()== Qt::MiddleButton)
    {
        statusBar()->showMessage (QStringLiteral("中鍵:")+str,1000);
    }
}

void ip::mouseReleaseEvent (QMouseEvent * event)
{
    const QPoint labelPos = imgwin->mapFromGlobal(event->globalPos()); // 新增：以影像區座標為準
    QString str="(" + QString::number(labelPos.x()) + ", " + QString::number(labelPos.y()) +")";
    statusBar()->showMessage(QStringLiteral("釋放:")+str);

    if (event->button() == Qt::LeftButton && isSelecting && rubberBand) { // 新增：完成選取並放大
        QRect selectedRect(selectionOrigin, labelPos);
        selectedRect = selectedRect.normalized().intersected(imgwin->rect());
        rubberBand->hide();
        isSelecting = false;

        if (!selectedRect.isEmpty() && !img.isNull()) {
            const double scaleX = static_cast<double>(img.width()) / imgwin->width();
            const double scaleY = static_cast<double>(img.height()) / imgwin->height();
            const int left   = qBound(0, static_cast<int>(selectedRect.left()   * scaleX), img.width()  - 1);
            const int top    = qBound(0, static_cast<int>(selectedRect.top()    * scaleY), img.height() - 1);
            const int right  = qBound(0, static_cast<int>(selectedRect.right()  * scaleX), img.width()  - 1);
            const int bottom = qBound(0, static_cast<int>(selectedRect.bottom() * scaleY), img.height() - 1);
            const int w = qMax(1, right - left + 1);
            const int h = qMax(1, bottom - top + 1);

            QImage cropped = img.copy(left, top, w, h);
            QImage zoomed = cropped.scaled(cropped.width() * 2, cropped.height() * 2,
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
            QLabel *preview = new QLabel; // 新增：顯示放大結果的新視窗
            preview->setAttribute(Qt::WA_DeleteOnClose);
            preview->setWindowTitle(QStringLiteral("矩形放大結果"));
            preview->setPixmap(QPixmap::fromImage(zoomed));
            preview->show();
        }
    }
}

