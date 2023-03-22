#ifndef COMMONDIALOG_H
#define COMMONDIALOG_H

#include <QDialog>
#include "commonui_global.h"

namespace Ui {
class CommonDialog;
}

class COMMONUI_EXPORT CommonDialog : public QDialog
{
    Q_OBJECT

public:
    enum ButtonType {
        NoButton = 0x1,
        OnlyOkButton = 0x2,
        OkCancelButton = 0x3,
        SaveButton = 0x4
    };
    explicit CommonDialog(const QString& title,
                          ButtonType type = OkCancelButton,
                          QWidget *parent = nullptr);
    ~CommonDialog();

    void setDisplayWidget(QWidget* widget); //注意，widget和底下的text是互斥的，只能调用其中一个
    void setDisplayText(const QString& text);
    void setOkBtnText(const QString& text);
    void setCancelBtnText(const QString& text);
    void setSaveBtnText(const QString& text);
    void setAbandonBtnText(const QString& text);
    void setCancel2BtnText(const QString& text);
    void setCloseOnOk(bool ok);

public slots:
    void onOkClicked();
    void onCancelClicked();
    void onSaveClicked();
    void onAbandonClicked();
    void onCancel2Clicked();
    void onCloseClicked();
    void onSetOkBtnEnabled(bool);
    void onSetFocusToOkBtn();

signals:
    void sigAccepted(bool);
    void sigSave(bool);
    void sigClose();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void hideBtn(QPushButton* btn);

private:
    Ui::CommonDialog *ui;
    bool leftButtonPressed_;
    QPoint lastPos_;
    ButtonType btnType_;
    bool closeOnOk_;
};

#endif // COMMONDIALOG_H
