#include "gtransform.h"
#include<QPixmap>
#include<Qpainter>
#include <QFileDialog>
GTransform::GTransform(QWidget *parent)
    : QWidget(parent)
{
    mainLayout = new QHBoxLayout (this);
    leftLayout = new QVBoxLayout (this);
    mirrorGroup = new QGroupBox (QStringLiteral("鏡射"),this);
    groupLayout = new QVBoxLayout (mirrorGroup);

    hCheckBox = new QCheckBox (QStringLiteral ("水平"), mirrorGroup);
    vCheckBox = new QCheckBox (QStringLiteral("垂直"),mirrorGroup);
    mirrorButton = new QPushButton (QStringLiteral ("執行"), mirrorGroup);
    mirrorButton2 = new QPushButton (QStringLiteral ("存檔"), mirrorGroup);
    hCheckBox->setGeometry (QRect (13, 28, 87, 19));
    vCheckBox->setGeometry (QRect (13, 54, 87, 19));
    mirrorButton->setGeometry (QRect(13, 80, 93, 28));
    mirrorButton2->setGeometry (QRect(13, 90, 93, 28));
    groupLayout->addWidget (hCheckBox);
    groupLayout->addWidget (vCheckBox);
    groupLayout->addWidget (mirrorButton);
    groupLayout->addWidget (mirrorButton2);
    leftLayout->addWidget (mirrorGroup);
    rotateDial = new QDial (this);
    rotateDial->setNotchesVisible(true);
    vSpacer = new QSpacerItem (20, 58, QSizePolicy:: Minimum,
                              QSizePolicy:: Expanding);
    leftLayout->addWidget (rotateDial);
    leftLayout->addItem(vSpacer);
    mainLayout->addLayout (leftLayout);

    inWin = new QLabel(this);
    inWin->setScaledContents(true);
    QPixmap *initPixmap= new QPixmap (300,200);
    initPixmap->fill(QColor(255,255,255));
    //------------------------------------
    QPainter *paint=new QPainter(initPixmap);
    paint->setPen(*(new QColor(0,0,0)));
    paint->begin(initPixmap);
    paint->drawRect(15,15,60,40);
    paint->end();
    if(srcImg.isNull())srcImg=initPixmap->toImage();
    //------------------------------------
    inWin->setPixmap (*initPixmap);
    inWin->setSizePolicy (QSizePolicy:: Expanding, QSizePolicy:: Expanding);
    if (srcImg.isNull())
    {
        QPixmap *initPixmap= new QPixmap (300,200);
        initPixmap->fill(QColor (255, 255, 255));
        inWin->setPixmap(*initPixmap);
    }
    mainLayout->addWidget(inWin);
    connect (mirrorButton, SIGNAL(clicked()), this, SLOT (mirroredImage()));
    connect (mirrorButton2, SIGNAL(clicked()), this, SLOT (QImagesave()));
    connect (rotateDial, SIGNAL (valueChanged(int)), this, SLOT (rotatedImage()));
}

GTransform::~GTransform()
{

}

void GTransform::QImagesave()
{
    QImage imgToSave = dstImg.isNull() ? srcImg : dstImg;
    if (imgToSave.isNull()) return;

    QString filePath = QFileDialog::getSaveFileName(
        this,
        QStringLiteral("存成 PNG"),
        "",
        QStringLiteral("PNG Files (*.png)")
        );
    if (filePath.isEmpty()) return;

    if (!filePath.endsWith(".png", Qt::CaseInsensitive))
        filePath += ".png";

    bool ok = imgToSave.save(filePath, "PNG");
    if (!ok){
    }
}
void GTransform::mirroredImage()
{
    bool H,V;
    if (srcImg.isNull())return;
    H=hCheckBox->isChecked();
    V=vCheckBox->isChecked();
    dstImg = srcImg.mirrored(H,V);
    inWin->setPixmap (QPixmap:: fromImage (dstImg));
    srcImg = dstImg;
}

void GTransform::rotatedImage()
{
    QTransform tran;
    int angle;
    if (srcImg.isNull())return;
    angle = rotateDial->value();
    tran.rotate (angle);
    dstImg = srcImg.transformed (tran);
    inWin->setPixmap (QPixmap:: fromImage (dstImg));
    //srcImg = dstImg;
}
