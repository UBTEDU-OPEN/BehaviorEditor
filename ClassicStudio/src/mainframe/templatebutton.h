#ifndef TEMPLATEBUTTON_H
#define TEMPLATEBUTTON_H

#include <QWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QLabel>
#include <QIcon>
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

namespace Ui {
	class TemplateButton;
}

class TemplateButton: public QWidget {
Q_OBJECT
public:
	explicit TemplateButton(int number, QString name, const QIcon &qiNormalIcon, const QIcon &qiHoveringIcon, const QIcon &qiSelectedIcon, QWidget *parent = nullptr);
	~TemplateButton();
        
	void unselect();
	int getNumber() const;
	QString getName() const;
        
protected:
	void mousePressEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
        
private:
	bool isSelected = false;
	int number;
	QString name;
	QPoint qpMouseClickedPosition;
	QLabel *qlButtonIcon, *qlButtonText;
	QHBoxLayout *qhblHorizontalLayout;
	QVBoxLayout *qvblVertialLayout;
	QPixmap qpNormalIcon, qpHoveringIcon, qpSelectedIcon;
	static const QString arrBackgroundResources[6];
	void paintEvent(QPaintEvent *ev);
        
signals:
	void onClicked(TemplateButton &tb);
};

#endif // TEMPLATEBUTTON_H