#ifndef SPINBOX_H
#define SPINBOX_H

#include <QSpinBox>
#include "commonui_global.h"

class COMMONUI_EXPORT SpinBox: public QSpinBox {
Q_OBJECT
public:
	explicit SpinBox(QWidget *parent = nullptr);
        
protected:
	void enterEvent(QEvent *event) override;
	void leaveEvent(QEvent *event) override;
    void stepBy(int steps) override;

signals:
    void stepEventTrigger(int);
};

#endif // SPINBOX_H
