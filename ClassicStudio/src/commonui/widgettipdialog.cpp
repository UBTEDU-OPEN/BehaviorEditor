#include "widgettipdialog.h"
#include <QPainter>
#include <QApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>

static const int TRIANGLE_HEIGHT = 10;
static const int TRIANGLE_WIDTH = 20;

WidgetTipDialog::WidgetTipDialog(QWidget *parent)
    : QDialog(parent)
    , m_orientation(TriangleOrientation::NONE)
    , m_borderRadius(0)
{
    setWindowFlags(Qt::FramelessWindowHint|Qt::Dialog|Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setModal(false);
    qApp->installEventFilter(this);
    setAttribute(Qt::WA_DeleteOnClose);
    m_layout = new QVBoxLayout(this);
}

WidgetTipDialog::~WidgetTipDialog()
{
}

void WidgetTipDialog::setDisplayWidget(QWidget* widget, TriangleOrientation orientation, int borderRadius)
{
    m_layout->addWidget(widget);
    m_borderRadius = borderRadius;
    m_orientation = orientation;
    if(orientation == TriangleOrientation::UP || orientation == TriangleOrientation::DOWN) {
        setFixedSize(widget->width(),widget->height()+TRIANGLE_HEIGHT);
        if(orientation == TriangleOrientation::UP) {
            m_layout->setContentsMargins(0,TRIANGLE_HEIGHT,0,0);
        } else {
            m_layout->setContentsMargins(0,0,0,TRIANGLE_HEIGHT);
        }
    } else {
        setFixedSize(widget->width(),widget->height());
        m_layout->setContentsMargins(0,0,0,0);
    }
}

void WidgetTipDialog::setDisplayText(const QString& displayText, TriangleOrientation orientation, int borderRadius)
{
    QWidget* widget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(widget);
    QLabel* label = new QLabel(widget);
    label->setText(displayText);
    layout->addWidget(label);
    widget->adjustSize();
    setDisplayWidget(widget,orientation,borderRadius);
}

void WidgetTipDialog::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(30, 35, 45, 200));
    QPainterPath drawPath;
    int yStartPos = 0;
    int ySidePos = 0;
    int yVertexPos = 0;
    int yHeight = 0;
    switch (m_orientation) {
    case TriangleOrientation::UP:
        yStartPos = TRIANGLE_HEIGHT;
        ySidePos = yStartPos;
        yVertexPos = 0;
        yHeight = height()-TRIANGLE_HEIGHT;
        break;
    case TriangleOrientation::DOWN:
        yStartPos = 0;
        ySidePos = height()-TRIANGLE_HEIGHT;
        yVertexPos = height();
        yHeight = height()-TRIANGLE_HEIGHT;
        break;
    default:
        yStartPos = 0;
        yHeight = height();
        break;
    }
    drawPath.addRoundedRect(0,yStartPos,width(),yHeight,m_borderRadius,m_borderRadius);
    if(m_orientation == TriangleOrientation::UP || m_orientation == TriangleOrientation::DOWN) {
        int startXPos = (width()-TRIANGLE_WIDTH)/2;
        QPolygon trianglePolygon;
        trianglePolygon << QPoint(startXPos, ySidePos);
        trianglePolygon << QPoint(width()/2, yVertexPos);
        trianglePolygon << QPoint(startXPos + TRIANGLE_WIDTH, ySidePos);
        drawPath.addPolygon(trianglePolygon);
    }
    painter.drawPath(drawPath);
}

bool WidgetTipDialog::eventFilter(QObject *watched, QEvent *event)
{
//    if(watched != this) {
        switch (event->type()) {
        case QEvent::Leave:
//        case QEvent::WindowActivate:
//        case QEvent::WindowDeactivate:
//        case QEvent::FocusIn:
//        case QEvent::FocusOut:
        case QEvent::Close:
        case QEvent::MouseButtonPress:
        case QEvent::MouseButtonRelease:
        case QEvent::MouseButtonDblClick:
        case QEvent::Wheel:
            close();
            break;
        default:
            break;
        }
//    }
    return false;
}
