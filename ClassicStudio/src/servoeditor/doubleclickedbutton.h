#ifndef DOUBLECLICKEDBUTTON_H
#define DOUBLECLICKEDBUTTON_H

#include <QPushButton>

class DoubleClickedButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DoubleClickedButton(QWidget *parent = nullptr);
    explicit DoubleClickedButton(const QString &text, QWidget *parent = nullptr);
    DoubleClickedButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
    ~DoubleClickedButton();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void doubleClicked();
};

#endif // DOUBLECLICKEDBUTTON_H
