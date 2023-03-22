#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include<QList>
#include<QFileInfo>
#include"improtitemwidget.h"

namespace Ui {
class ImportDialog;
}

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ImportDialog(QWidget *parent = nullptr);
    ~ImportDialog();

private slots:
    void on_closeBtn_clicked();

    void on_addBtn_clicked();

    void on_deleteBtn_clicked();

    void on_checkBox_toggled(bool checked);

    void on_exportBtn_clicked();

    void on_cancelBtn_clicked();

    void onCancelImprot();

private:
    Ui::ImportDialog *ui;
    QList<ImprotItemWidget *> *mItemWidgets;
    bool isStartImport;
    QList<ImprotItemWidget *> *mCheckedItems = NULL;
    QString stopStyle = "QPushButton{color: rgb(255, 255, 255);border-radius:14px 14px 14px 14px;border: 1px solid #FFFFFF;}";
    QString startStyle = "QPushButton{"
            "color: rgb(255, 255, 255);"
            "border-radius:14px 14px 14px 14px;"
            "background-color: rgb(0, 165, 255);"
            "}"
            "QPushButton:hover{"
            "background-color: rgb(85, 255, 255);"
            "}"
            "QPushButton:pressed{"
            "background-color: rgb(144, 144, 144);}";

    void startImport();
    void setButtonStartStyle(bool isStart);
};

#endif // EXPORTDIALOG_H
