#include "templateitemdelegate.h"
#include<QPainter>
#include<QStyleOptionViewItem>
#include<QModelIndex>
#include<templatedata.h>
#include<QDebug>
#include<QString>

TemplateItemDelegate::TemplateItemDelegate(TemplateListViewModel *templateListViewModel, QObject *parent)
    : QStyledItemDelegate(parent)


{
    this->m_templateListViewModel=templateListViewModel;


}

TemplateItemDelegate::~TemplateItemDelegate()
{
}


