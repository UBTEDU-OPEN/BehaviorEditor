#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include "commonui_global.h"
#include <QDialog>
#include <QVBoxLayout>

class COMMONUI_EXPORT WidgetTipDialog : public QDialog
{
    Q_OBJECT

public:
    enum TriangleOrientation {
        NONE = 0,
        UP = 1,
        DOWN = 2
    };
    explicit WidgetTipDialog(QWidget *parent = nullptr);
    ~WidgetTipDialog();

    void setDisplayWidget(QWidget* widget,
                          TriangleOrientation orientation = TriangleOrientation::NONE,
                          int borderRadius = 0);

    void setDisplayText(const QString& displayText,
                        TriangleOrientation orientation = TriangleOrientation::NONE,
                        int borderRadius = 0);
    bool eventFilter(QObject *watched, QEvent *event) override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVBoxLayout* m_layout;
    TriangleOrientation m_orientation;
    int m_borderRadius;
};

#endif // TIPDIALOG_H
