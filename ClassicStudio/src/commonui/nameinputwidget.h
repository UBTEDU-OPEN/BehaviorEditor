#ifndef NAMEINPUTWIDGET_H
#define NAMEINPUTWIDGET_H

#include <QWidget>

#include "commonui_global.h"

namespace Ui {
class NameInputWidget;
}

class COMMONUI_EXPORT NameInputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NameInputWidget(const QString& label, const QString& originName, QWidget *parent = nullptr);
    ~NameInputWidget();
    QString newName();
    void setWarningText(QString warningText);

signals:
    void nameIsEmpty(bool);

private:
    Ui::NameInputWidget *ui;
};

#endif // NAMEINPUTWIDGET_H
