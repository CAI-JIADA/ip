#include "mouseevent.h"

MouseEvent::MouseEvent(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(QStringLiteral("測試滑鼠事件"));
    statusLabel = new QLabel;
    statusLabel->setText (QStringLiteral("指標位置"));
    statusLabel->setFixedWidth (100);
    MousePosLabel = new QLabel;
    MousePosLabel->setText(tr(" "));
    MousePosLabel->setFixedWidth (100);
    statusBar()->addPermanentWidget (statusLabel);
    statusBar()->addPermanentWidget (MousePosLabel);
    setMouseTracking (true);
    resize (400,300);
}

MouseEvent::~MouseEvent()
{

}

void MouseEvent::mouseDoubleClickEvent (QMouseEvent * event)
{
     QString str="(" + QString::number(event->x()) + ", " + QString::number(event->y()) +")";
    statusBar ()->showMessage (QStringLiteral("雙擊:")+str,1000);
}

void MouseEvent::mouseMoveEvent (QMouseEvent * event)
{
    QString str="(" + QString::number(event->x()) + ", " + QString::number(event->y()) +")";
    MousePosLabel->setText(str);
}

void MouseEvent::mousePressEvent (QMouseEvent * event)
{
    QString str="(" + QString::number(event->x()) + ", " + QString::number(event->y()) +")";
    if (event->button() == Qt::LeftButton)
    {
        statusBar ()->showMessage (QStringLiteral("左鍵:")+str,1000);
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

void MouseEvent::mouseReleaseEvent (QMouseEvent * event)
{
    QString str="(" + QString::number(event->x()) + ", " + QString::number(event->y()) +")";
    statusBar()->showMessage(QStringLiteral("釋放:")+str);
}
