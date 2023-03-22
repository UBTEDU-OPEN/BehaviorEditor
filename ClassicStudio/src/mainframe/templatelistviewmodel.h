#ifndef TEMPLATELISTVIEWMODEL_H
#define TEMPLATELISTVIEWMODEL_H

#include <QAbstractListModel>

class BaseData;

class TemplateListViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit TemplateListViewModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setBaseData(QList<BaseData*> baseDataList);

    void add(BaseData *basedate);

    BaseData *at(int index);



private:
    QList<BaseData*> m_baseDataList;
};

#endif // TEMPLATELISTVIEWMODEL_H
