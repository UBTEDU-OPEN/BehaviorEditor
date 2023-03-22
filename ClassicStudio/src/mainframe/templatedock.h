#ifndef TEMPLATEDOCK_H
#define TEMPLATEDOCK_H

#include <QDockWidget>
#include <QPoint>
#include <QToolButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QApplication>
#include <QMenu>
#include <QDrag>
#include <QMimeData>
#include <QDebug>

#include <thread>
#include <list>

#include "templatedata.h"

using namespace std;

/**
 * @brief The DockWidget class
 */

namespace Ui {
	class TemplateDock;
}

class TemplateDock: public QDockWidget {
Q_OBJECT
public:
	explicit TemplateDock(TemplateData *pTemplateData, QWidget *parent = nullptr);
        
signals:
	void sigSetVisible(bool bVisible);
	void sigAddCurFrame(QString fileName);
        
public:
	void init();
        
public:
	virtual bool eventFilter(QObject *watched, QEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	void setVisibility(bool visible);
	bool getVisibility() const;
	void showPage(const QString &name);
        
protected:
	QToolButton* queryToolButton(const QPoint &pt, BaseData* &pUserData);
	void startThreadToHidden();
        
private:
	QPoint m_startPos;
	TemplateData *m_pTemplateData;
	bool m_bCancelThread;
	bool isNowVisible;
	list<QWidget> vecTemplateDockPages;
	list<QString> vecNames;
};

#endif // TEMPLATEDOCK_H