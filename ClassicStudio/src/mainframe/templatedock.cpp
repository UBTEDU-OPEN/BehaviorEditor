#include "templatedock.h"

TemplateDock::TemplateDock(TemplateData *pTemplateData, QWidget *parent):
	isNowVisible(true), m_pTemplateData(pTemplateData), m_bCancelThread(false), QDockWidget(parent){
	init();
}

void TemplateDock::init(){
	connect(this, &TemplateDock::sigSetVisible, this, &TemplateDock::setVisible);
	if(!m_pTemplateData){
		return;
	}
        
	// 样式
	this->setStyleSheet("background-color: #373C49");
        
	// 滚动条
	QScrollArea *pScrollArea = new QScrollArea(this);

	pScrollArea->setWidgetResizable(true);
	pScrollArea->setBackgroundRole(QPalette::Light);
	pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setWidget(pScrollArea);
        
	QWidget *widgetScrollArea = new QWidget(pScrollArea);
	pScrollArea->setWidget(widgetScrollArea);
        
	QVBoxLayout *pVlayout = new QVBoxLayout;
	widgetScrollArea->setLayout(pVlayout);
        
	// 动作内容
	const int nMaxCols = 4;
	foreach(TypeData * pTypeData, m_pTemplateData->get()){
		foreach(ItemData * pItemData, pTypeData->get()){
			int nRow = 0;
			int nCol = 0;
                        
			// 创建并放入unordered_map
			this->vecTemplateDockPages.emplace_back(this);
			this->vecNames.emplace_back(pItemData->getName());
			QWidget &page = *(--this->vecTemplateDockPages.end());
			pItemData->setWidget(&page);
			pVlayout->addWidget(&page);
			page.setVisible(false);
			page.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
                        
			// 标题
			QLabel *qlTitle = new QLabel(pItemData->getName(), &page);
			qlTitle->setStyleSheet(
				"background: transparent;"
				"background-color: #373C49;"
				"border: none;"
				"font-family: Microsoft Yahei; font-size: 9pt; color: rgba(255,255,255,0.8);"
				);
			qlTitle->setFixedHeight(30);
                        
			// 动作列表
			QGridLayout *pGridLayout = new QGridLayout(this);
			pGridLayout->setContentsMargins(QMargins(0, 0, 0, 0));
			pGridLayout->setSpacing(0);
			pGridLayout->setMargin(0);
			for(BaseData *pBaseData: pItemData->get()){
				QToolButton *qtbButton = new QToolButton();
				qtbButton->setIcon(QIcon(pBaseData->getLogo()));
				qtbButton->setText(pBaseData->getName());
				qtbButton->setToolTip(pBaseData->getName());
				qtbButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
				qtbButton->setIconSize(QSize(48, 48));
				qtbButton->setFixedSize(50, 70);
				qtbButton->setCursor(Qt::PointingHandCursor);
				qtbButton->setStyleSheet(
					"QToolButton {"
					"background: transparent;"
					"background-color: #373C49;"
					"border: none;"
					"font-family: Microsoft Yahei; font-size: 9pt; color: rgba(255,255,255,0.8);"
					"}"
					"QToolButton: hover{"
					"background-color: #373C49;"
					"}");
				qtbButton->installEventFilter(this);
				pBaseData->setToolButton(qtbButton);
                                
				pGridLayout->addWidget(qtbButton, nRow, nCol, Qt::AlignTop);
				nCol++;
				if(nCol % nMaxCols == 0){
					nCol = 0;
					nRow++;
				}
			}
                        
			QVBoxLayout *pPageVlayout = new QVBoxLayout(this);
			pPageVlayout->addWidget(qlTitle);
			pPageVlayout->addLayout(pGridLayout);
			page.setLayout(pPageVlayout);
		}
	}
}

bool TemplateDock::eventFilter(QObject *watched, QEvent *event){
	if(event->type() == QEvent::MouseButtonPress){
		return QApplication::sendEvent(this, event);
	}
	return QWidget::eventFilter(watched, event);
}

void TemplateDock::mousePressEvent(QMouseEvent *event){
	if(event->buttons() == Qt::LeftButton){
		m_startPos = mapFromGlobal(event->globalPos());
	} else if(event->buttons() == Qt::RightButton){
		QPoint pt = mapFromGlobal(event->globalPos());
		BaseData *pUserData{ nullptr };
		QToolButton *pBtn = queryToolButton(pt, pUserData);
		if(pBtn && pUserData->get().count() > 0){
			QString fileName = pUserData->get().at(0);
			QMenu menu(this);
			menu.addAction(
				tr("add current frame"), this, [ = ](){
				emit sigAddCurFrame(fileName);
			});
			menu.exec(QCursor::pos());
		}
	}
}

void TemplateDock::mouseReleaseEvent(QMouseEvent *event){
	m_startPos = QPoint(0, 0);
	qDebug() << "拖曳结束" << endl;
	QDockWidget::mouseReleaseEvent(event);
}

void TemplateDock::mouseMoveEvent(QMouseEvent *event){
	if(event->buttons() == Qt::LeftButton){
		QPoint pt = mapFromGlobal(event->globalPos());
		int distance = (pt - m_startPos).manhattanLength();
		if(distance >= QApplication::startDragDistance()){
			BaseData *pUserData{ nullptr };
			QToolButton *pBtn = queryToolButton(m_startPos, pUserData);
			if(pBtn == nullptr) return;
                        
			if(pUserData->get().count() > 0){
				QDrag *drag = new QDrag(this);
				QMimeData *data = new QMimeData;
				QList<QUrl> urls;
				urls.append(QUrl::fromLocalFile(pUserData->get().at(0)));
				data->setUrls(urls);
				drag->setMimeData(data);
				drag->setPixmap(QPixmap(pUserData->getLogo()));
				startThreadToHidden();
				Qt::DropAction actions = drag->exec(Qt::CopyAction | Qt::MoveAction);
				if(actions == Qt::IgnoreAction){
					m_bCancelThread = true;
				}
			}
		}
	}
}

void TemplateDock::setVisibility(bool visible){
	this->isNowVisible = visible;
	if(this->isNowVisible) this->show();
	else this->hide();
}

bool TemplateDock::getVisibility() const {
	return this->isNowVisible;
}

void TemplateDock::showPage(const QString &name){
	auto it = this->vecNames.begin();
	for(QWidget &page: this->vecTemplateDockPages){
		QString &currentName = *it;
		if(currentName == name) page.setVisible(true);
		else page.setVisible(false); // 其余都隐藏
		it++;
	}
}

QToolButton* TemplateDock::queryToolButton(const QPoint &pt, BaseData* &pUserData){
	QScrollArea *pScrollArea = qobject_cast<QScrollArea*>(widget());
	if(pScrollArea == nullptr){
		return nullptr;
	}
	QPoint point = pScrollArea->mapFromParent(pt);
	point = pScrollArea->widget()->mapFromParent(point);
	foreach(TypeData * pTypeData, m_pTemplateData->get()){
		foreach(ItemData * pItemData, pTypeData->get()){
			QPoint pt = pItemData->getWidget()->mapFromParent(point);
			foreach(BaseData * pBaseData, pItemData->get()){
				if(pBaseData->getToolButton()->geometry().contains(pt)){
					pUserData = pBaseData;
					return pBaseData->getToolButton();
				}
			}
		}
	}
	return nullptr;
}

void TemplateDock::startThreadToHidden(){
	m_bCancelThread = false;
	if(isVisible()){
		std::thread thread([ = ](){
				   while(!m_bCancelThread){
					   QPoint pt = QCursor::pos();
					   if(!geometry().contains(pt)){
						   emit sigSetVisible(false);
						   m_bCancelThread = true;
						   break;
					   }
					   std::this_thread::sleep_for(std::chrono::milliseconds(200));
				   }
			});
		thread.detach();
	}
}
