#ifndef COMBOBUTTON_H
#define COMBOBUTTON_H

#include "commonui_global.h"

#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QDebug>

class COMMONUI_EXPORT ComboButton : public QWidget
{
    Q_OBJECT
public:
    explicit ComboButton(QWidget *parent = nullptr);

signals:
    void sigFunctionButtonClicked();

public:
    void setPromptMenu(QMenu* menu);
    void setFunctionButtonText(const QString& text);
    void hideSeparator();
    void setEnabled(bool enabled);

private slots:
    void showPromptMenu();

private:
    QMenu* promptMenu_;
    QPushButton* functionButton_;
    QPushButton* menuButton_;
    QLabel* separatorLabel_;
};

#endif // COMBOBUTTON_H
