#include "ip.h"
//#include "mouseevent.h"
#include<QHBoxLayout>
#include<QMenuBar>
#include<QFileDialog>
#include<QDebug>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QPainter>
#include<QPen>
#include<QPushButton>

// 新增：可在預覽上手繪的標記元件
class DrawingLabel : public QLabel
{
public:
    explicit DrawingLabel(QWidget *parent = nullptr) : QLabel(parent) {
        setMouseTracking(true);
    }
    void setImage(const QImage &img) {
        // 新增：載入放大影像到可繪製的 ARGB 緩衝
        drawable = img.convertToFormat(QImage::Format_ARGB32);
        setFixedSize(drawable.size());
        refresh();
    }
    const QImage &image() const { return drawable; }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            drawing = true;
            lastPos = event->pos();
        }
        QLabel::mousePressEvent(event);
    }
    void mouseMoveEvent(QMouseEvent *event) override {
        if (drawing && (event->buttons() & Qt::LeftButton)) {
            QPainter painter(&drawable);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setPen(QPen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(lastPos, event->pos());
            lastPos = event->pos();
            refresh();
        }
        QLabel::mouseMoveEvent(event);
    }
    void mouseReleaseEvent(QMouseEvent *event) override {
        drawing = false;
        QLabel::mouseReleaseEvent(event);
    }
private:
    void refresh() {
        setPixmap(QPixmap::fromImage(drawable));
    }
    QImage drawable;
    bool drawing = false;
    QPoint lastPos;
};

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

// 新增：將 QLabel 座標換算成原圖座標，避免放大/縮小造成查詢錯位
QPoint ip::mapLabelToImage(const QPoint &pt) const
{
    if (img.isNull() || imgwin->width() == 0 || imgwin->height() == 0) return QPoint(0, 0);
    const double scaleX = static_cast<double>(img.width()) / imgwin->width();
    const double scaleY = static_cast<double>(img.height()) / imgwin->height();
    const int imgX = qBound(0, static_cast<int>(pt.x() * scaleX), img.width() - 1);
    const int imgY = qBound(0, static_cast<int>(pt.y() * scaleY), img.height() - 1);
    return QPoint(imgX, imgY);
}

// 新增：矩形選取換算成原圖矩形，並限制在原圖範圍內
QRect ip::mapLabelRectToImage(const QRect &rect) const
{
    if (img.isNull()) return QRect();
    QPoint topLeft = mapLabelToImage(rect.topLeft());
    QPoint bottomRight = mapLabelToImage(rect.bottomRight());
    QRect mapped(topLeft, bottomRight);
    return mapped.normalized().intersected(img.rect());
}



void ip::mouseMoveEvent (QMouseEvent * event)
{
    const QPoint labelPos = imgwin->mapFromGlobal(event->globalPos()); // 新增：將座標轉為影像區域
    int x = labelPos.x();
    int y = labelPos.y();

    QString str="(" + QString::number(x) + ", " + QString::number(y) +")";
    if(!img.isNull() && x >= 0 && x < imgwin->width() && y >= 0 && y < imgwin->height())
    {
        const QPoint imgPos = mapLabelToImage(labelPos); // 新增：共用換算函式
        if (img.rect().contains(imgPos)) { // 補強：安全界限檢查
            int gray = qGray(img.pixel(imgPos));
            str += ("=" + QString::number(gray));
        }
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
        if (!img.isNull() && imgwin->rect().contains(labelPos)) { // 新增：左鍵拖曳啟動矩形選取
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
            const QRect sourceRect = mapLabelRectToImage(selectedRect); // 新增：統一座標換算
            if (!sourceRect.isEmpty()) {
                QImage cropped = img.copy(sourceRect);
                QImage zoomed = cropped.scaled(cropped.width() * 2, cropped.height() * 2,
                                               Qt::KeepAspectRatio, Qt::SmoothTransformation);

                QWidget *preview = new QWidget; // 新增：帶有畫筆/存檔功能的預覽窗
                preview->setAttribute(Qt::WA_DeleteOnClose);
                preview->setWindowTitle(QStringLiteral("矩形放大結果（可標註/儲存）"));

                auto *layout = new QVBoxLayout(preview);
                auto *drawArea = new DrawingLabel(preview);
                drawArea->setImage(zoomed); // 新增：放大圖載入可繪製元件
                layout->addWidget(drawArea, 1);

                auto *btnRow = new QHBoxLayout();
                QPushButton *saveBtn = new QPushButton(QStringLiteral("儲存"), preview);
                QPushButton *closeBtn = new QPushButton(QStringLiteral("關閉"), preview);
                btnRow->addStretch();
                btnRow->addWidget(saveBtn);
                btnRow->addWidget(closeBtn);
                layout->addLayout(btnRow);

                connect(saveBtn, &QPushButton::clicked, preview, [this, drawArea]() {
                    // 新增：儲存繪製後結果
                    QString path = QFileDialog::getSaveFileName(
                        nullptr,
                        QStringLiteral("儲存標註影像"),
                        "",
                        QStringLiteral("PNG Files (*.png);;JPG Files (*.jpg)")
                    );
                    if (!path.isEmpty()) {
                        drawArea->image().save(path);
                    }
                });
                connect(closeBtn, &QPushButton::clicked, preview, &QWidget::close);

                preview->resize(zoomed.width() + 40, zoomed.height() + 80);
                preview->show();
            }
        }
    }
}
