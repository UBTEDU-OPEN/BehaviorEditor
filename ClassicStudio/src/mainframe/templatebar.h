#ifndef TEMPLATEBAR_H
#define TEMPLATEBAR_H

#include <QScrollArea>
#include <QDir>
#include <QSpacerItem>
#include <QSizePolicy>
#include"templateitem.h"
#include <list>

#include "templatebutton.h"
#include "TemplateDock.h"

namespace Ui {
	class TemplateBar;
}

QT_BEGIN_NAMESPACE
class QListWidgetItem;
class TemplateData;
class TypeData;
class ItemData;
class BaseData;
class QToolButton;
class DockWidget;
class QDrag;
QT_END_NAMESPACE

using namespace std;

/**
 * @brief The TemplateBar class 模板库界面类
 */
class TemplateBar: public QWidget {
Q_OBJECT

public:
	explicit TemplateBar(QWidget *parent = nullptr);
	~TemplateBar();
        
signals:
	void sigAddCurFrame(QString fileName);
    void sigItemLongPress(QPoint point,QString filePath);
    void sigItemLongPressRelease(QPoint point,QString filePath);
    void sigItemLongPressMove(QPoint point);
        
public:
	void init();
	void initVariable();
	void initWindow();
	void initConnect();
	void adjustDockWidget();
    void addItem(const QString& filePath);

protected:
	void resizeEvent(QResizeEvent *event) override;
	void initDockWidget();
	void initTemplateList();
	bool loadLocalTemplateData();
        
private:
	Ui::TemplateBar *ui;
	TemplateDock *tdTemplatePanel;
	TemplateData *m_pTemplateData;
	int lastSelectedItemNumber;
	list<TemplateButton> listTemplateButtons;
    QList<TemplateItem *> *mTemplateItems;
    QLabel *mLabel;
	void selectTemplateType(TemplateButton &tb);
};

#endif // TEMPLATEBAR_H
