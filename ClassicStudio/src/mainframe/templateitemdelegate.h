#ifndef TEMPLATEITEMDELEGATE_H
#define TEMPLATEITEMDELEGATE_H

#include <QObject>
#include<QStyledItemDelegate>
#include"templatelistviewmodel.h"
#include"templateitem.h"

class TemplateItemDelegate: public QStyledItemDelegate

{
    Q_OBJECT
public:
    explicit TemplateItemDelegate(TemplateListViewModel *templateListViewModel, QObject *parent = 0);
    ~TemplateItemDelegate();

private:
    TemplateListViewModel *m_templateListViewModel;




};

#endif // TEMPLATEITEMDELEGATE_H
