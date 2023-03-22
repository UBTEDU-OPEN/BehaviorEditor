#include "templatelistviewmodel.h"
#include"templatedata.h"
#include<QModelIndex>
#include<QDebug>

TemplateListViewModel::TemplateListViewModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int TemplateListViewModel::rowCount(const QModelIndex &parent) const
{

    return m_baseDataList.size();;

}

QVariant TemplateListViewModel::data(const QModelIndex &index, int role) const
{
    BaseData *baseData=m_baseDataList.at(index.row());
    if(baseData==NULL)
    {
        return QVariant();
    }
    else
    {
        qDebug() << "role=====" << role;
        switch (role) {

        case Qt::DisplayRole:
            return baseData->getName();
        case Qt::SizeHintRole:
             return QSize(340, 40);


        default:
            return QVariant();
        }
    }
    return QVariant();
}

int TemplateListViewModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

void TemplateListViewModel::setBaseData(QList<BaseData *> baseDataList)
{
    this->m_baseDataList = baseDataList;
    emit layoutChanged();
}

void TemplateListViewModel::add(BaseData *baseData)
{
    this->m_baseDataList.push_back(baseData);
    emit layoutChanged();
}

BaseData *TemplateListViewModel::at(int index)
{
    return m_baseDataList.at(index);
}
