#include "behaviortabledelegate.h"
#include <QDebug>
#include <QCheckBox>
#include <QApplication>
#include <QStyleOptionButton>
#include <QPainter>
#include <QPen>
#include "behaviorcurvewidget.h"
#include "spinbox.h"
#include "robotcfg.h"

BehaviorTableDelegate::BehaviorTableDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
    m_checkedPixmap = QPixmap(":/icon/res/radio_normal_select.png");
    m_partiallyCheckedPixmap = QPixmap(":/icon/res/radio_half_select.png");
    m_uncheckedPixmap = QPixmap(":/icon/res/radio_normal.png");
}

void BehaviorTableDelegate::paint(QPainter *painter,
           const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    if(index.column() >= 0){
        if (option.state & QStyle::State_Selected){
            QPen pen(Qt::blue);
            painter->setPen(pen);
            painter->fillRect(option.rect, option.palette.highlight());
        }
        if(index.column() == 0) {
            int checkState = index.data(Qt::CheckStateRole).toInt();
            QPixmap pixmap;
            if(checkState == Qt::Checked) {
                pixmap = m_checkedPixmap;
            } else if(checkState == Qt::PartiallyChecked) {
                pixmap = m_partiallyCheckedPixmap;
            } else {
                pixmap = m_uncheckedPixmap;
            }
            painter->drawPixmap(5,option.rect.y()+5,pixmap.width(),pixmap.height(),pixmap);
            painter->drawText(pixmap.width()+2,option.rect.y(),option.rect.width()-pixmap.width()-2,option.rect.height(),Qt::AlignCenter,index.data().toString());
        }
        else if(index.column() >= 1 && !(m_pTableView && m_pTableView->isPersistentEditorOpen(index))) {
            QRect rect;
            if(index.column() == 1){
                rect.setX(option.rect.x()+10);
                rect.setY(option.rect.y()+5);
                rect.setWidth(option.rect.width()-20);
                rect.setHeight(option.rect.height()-10);
            } else {
                rect.setX(option.rect.x()+30);
                rect.setY(option.rect.y()+5);
                rect.setWidth(option.rect.width()-60);
                rect.setHeight(option.rect.height()-10);
            }
            QStyleOptionButton button;
            button.rect = rect;
            button.text = index.data().toString();
            button.features = QStyleOptionButton::ButtonFeature::DefaultButton;
            QApplication::style()->drawControl(QStyle::CE_PushButton,&button,painter);
            painter->drawRect(rect);
        }
    } else {
        QStyledItemDelegate::paint(painter,option,index);
    }
    painter->restore();
    painter->drawLine(option.rect.bottomLeft(),option.rect.bottomRight());
}

QWidget *BehaviorTableDelegate::createEditor(QWidget *parent,
                                       const QStyleOptionViewItem & option,
                                       const QModelIndex & index ) const
{
    Q_UNUSED(option);
    if(index.column() == 1) {
        BehaviorCurveWidget *editor = new BehaviorCurveWidget(parent);
        editor->setFlat(true);
        editor->setStyleSheet("border:1px solid black;");
        return editor;
    } else if (index.column() >= 2) {
        SpinBox *editor = new SpinBox(parent);
        editor->setFrame(true);
        int servoId = index.column()-1;
        auto pServo = RobotConfig->queryServo(servoId);
        if(pServo){
            editor->setMaximum(pServo->getMaxAngle());
            editor->setMinimum(pServo->getMinAngle());
        }
        editor->setStyleSheet("border:1px solid black;");
        return editor;
    } else {
        return nullptr;
    }
}

void BehaviorTableDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    if(index.column() == 1) {
        BehaviorCurveWidget *button = dynamic_cast<BehaviorCurveWidget*>(editor);
        button->setText(index.data(Qt::EditRole).toString());
    } else if (index.column() >= 2) {
        SpinBox *spinBox = dynamic_cast<SpinBox*>(editor);        
        spinBox->setValue(index.data(Qt::EditRole).toDouble());
    }
}

void BehaviorTableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if(index.column() == 1) {
        BehaviorCurveWidget *button = dynamic_cast<BehaviorCurveWidget*>(editor);
        QString value = button->text();
        model->setData(index, value, Qt::EditRole);
    }  else if(index.column() >= 1) {
        SpinBox *spinBox = dynamic_cast<SpinBox*>(editor);
        QString value = QString::number(spinBox->value(),'f',1);
        if(!(index.data(Qt::EditRole).toString() == "-" && value == "0.0")){
            model->setData(index, value, Qt::EditRole);
        }
    }
}

void BehaviorTableDelegate::updateEditorGeometry(QWidget *editor,
                                           const QStyleOptionViewItem &option,
                                           const QModelIndex & index ) const
{
    auto rect = option.rect;
    if(index.column() == 0) {
        rect = option.rect;
        rect.setX(rect.x()+3);
    } else if(index.column() == 1){
        rect.setX(option.rect.x()+10);
        rect.setY(option.rect.y()+5);
        rect.setWidth(option.rect.width()-20);
        rect.setHeight(option.rect.height()-10);
    } else if(index.column() >= 1){
        rect.setX(option.rect.x()+30);
        rect.setY(option.rect.y()+5);
        rect.setWidth(option.rect.width()-60);
        rect.setHeight(option.rect.height()-10);
    }
    editor->setGeometry(rect);
}
