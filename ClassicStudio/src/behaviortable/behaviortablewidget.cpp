#include "behaviortablewidget.h"

#include <QScrollBar>
#include <QHeaderView>
#include <QSpinBox>
#include <QTableWidgetItem>
#include <QEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>

#include "behaviortabledelegate.h"
#include "robotcfg.h"
#include "horizontalheaderview.h"
#include "spinbox.h"
#include "robotcfg.h"
#include "commondefine.h"
#include "predefinedposture.h"
#include "datacenter.h"

const int INDEX_COLUMN_INDEX = 0;
const int CURVE_COLUMN_INDEX = 1;
const int FIRST_SERVO_COLUMN_INDEX = 2;

BehaviorTableWidget::BehaviorTableWidget(QWidget * parent) : QTableView(parent)
  , frozenTableView(nullptr)
  , m_hHeader(nullptr)
  , m_tableDelegate(nullptr)
  , m_isBehaviorSelect(false)
  , m_firstVisibleColumn(1)
  , m_frozenVisibleWidth(0)
  , m_pressItemCheckState(Qt::Unchecked)
  , m_headerPressIndex(0)
{    
    initContextMenu();

    m_model = DataCenter::getInstance()->frameModel();
    setModel(m_model);  
    m_hHeader  = new HorizontalHeaderView(Qt::Horizontal,this);
    m_hHeader->setSectionResizeMode(QHeaderView::Fixed);
    setHorizontalHeader(m_hHeader);
    horizontalHeader()->setSectionsClickable(true);
    m_tableDelegate = new BehaviorTableDelegate(this);
    setItemDelegate(m_tableDelegate);
    m_tableDelegate->setTableViewPtr(this);
    verticalHeader()->hide();
    frozenTableView = new QTableView(this);
    setSelectionMode(QAbstractItemView::NoSelection);
    initFrozenTable();

    connect(frozenTableView->verticalScrollBar(), &QAbstractSlider::valueChanged,
            verticalScrollBar(), &QAbstractSlider::setValue);
    connect(verticalScrollBar(), &QAbstractSlider::valueChanged,
            frozenTableView->verticalScrollBar(), &QAbstractSlider::setValue);
    connect(selectionModel(),&QItemSelectionModel::selectionChanged,this,&BehaviorTableWidget::onSelectionChanged);
    connect(m_model,&QStandardItemModel::itemChanged,[this](QStandardItem *item){
        if(item->index().column() >= FIRST_SERVO_COLUMN_INDEX) {
            QMap<int, QMap<int,double>> frame;
            QString itemText = item->text();
            if(itemText != "-") {
                int frameIndex = m_model->item(item->index().row(),0)->text().toInt();
                frame[frameIndex].insert(item->index().column()-1,itemText.toDouble());
                DataCenter::getInstance()->changeServos(frame);
            }
        }
    });
    connect(horizontalScrollBar(),&QScrollBar::valueChanged,[this](int value){
        m_firstVisibleColumn = 2+value/columnWidth(0);
    });
    connect(m_hHeader,&HorizontalHeaderView::sigLButtonPressEvent,[this](int logicalIndex){
        m_headerPressIndex = logicalIndex;
        selectColumns(m_headerPressIndex,m_headerPressIndex);
    });
    connect(m_hHeader,&HorizontalHeaderView::sigLButtonMoveEvent,[this](int logicalIndex){
        selectColumns(m_headerPressIndex,logicalIndex);
    });

    const auto& servos = RobotConfig->getServos();

    for(auto cit = servos.cbegin(); cit != servos.cend(); cit++) {
        auto pServo = cit.value();
        if(pServo){
            m_servosRange.insert(cit.key(),qMakePair(pServo->getMinAngle(),pServo->getMaxAngle()));
        }
    }

    configStyle();
}

BehaviorTableWidget::~BehaviorTableWidget()
{
    delete frozenTableView;
}

void BehaviorTableWidget::configStyle()
{
    horizontalScrollBar()->setFixedHeight(14);
}

void BehaviorTableWidget::initFrozenTable()
{
    frozenTableView->setModel(model());
    frozenTableView->setFocusPolicy(Qt::NoFocus);
    frozenTableView->verticalHeader()->hide();
    auto hHeader  = new HorizontalHeaderView(Qt::Horizontal,this);
    frozenTableView->setHorizontalHeader(hHeader);
    frozenTableView->horizontalHeader()->setSectionsClickable(true);
    frozenTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    auto delegate = new BehaviorTableDelegate(frozenTableView);
    frozenTableView->setItemDelegate(delegate);
    delegate->setTableViewPtr(frozenTableView);
    viewport()->stackUnder(frozenTableView);
    for (int col = 1; col < model()->columnCount(); ++col) {
        frozenTableView->setColumnHidden(col, true);
    }
    horizontalHeader()->setFixedHeight(45);
    frozenTableView->horizontalHeader()->setFixedHeight(horizontalHeader()->height());
    frozenTableView->setColumnWidth(0, columnWidth(0) );
    frozenTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    frozenTableView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    frozenTableView->show();
    updateFrozenTableGeometry();
    setHorizontalScrollMode(ScrollPerPixel);
    setVerticalScrollMode(ScrollPerPixel);
    frozenTableView->setVerticalScrollMode(ScrollPerPixel);
    frozenTableView->horizontalHeader()->viewport()->installEventFilter(this);
    frozenTableView->viewport()->installEventFilter(this);
    setStyleSheet("selection-background-color: #B2DFEE;");
    frozenTableView->setStyleSheet("selection-background-color: #B2DFEE;");
    setShowGrid(false);
    frozenTableView->setShowGrid(false);
    frozenTableView->setSelectionMode(QAbstractItemView::NoSelection);

    connect(hHeader,&HorizontalHeaderView::sigSelectAll,this,&BehaviorTableWidget::onSelectAllStateChanged);
    connect(this,&BehaviorTableWidget::sigChangeSelectAllState,hHeader,&HorizontalHeaderView::onSelectChanged);
    connect(frozenTableView,&QTableView::clicked,[this](const QModelIndex &index){
        if(index.column() == 0) {
            if(m_pressItemCheckState == Qt::Checked) {
                clearAllSelection();
            } else if(m_pressItemCheckState == Qt::Unchecked || m_pressItemCheckState == Qt::PartiallyChecked) {
                selectRows(index,index);
            }
        }
    });
    connect(frozenTableView,&QTableView::doubleClicked,[this](const QModelIndex &index){
        if(index.column() == 0) {
            qDebug() << "frozenTableView doubleClicked" << index;
            DataCenter::getInstance()->changeRobotServoAnglesByRow(index.row());
        }
    });
}

void BehaviorTableWidget::resizeEvent(QResizeEvent * event)
{
    QTableView::resizeEvent(event);
    updateFrozenTableGeometry();
}

void BehaviorTableWidget::updateFrozenTableGeometry()
{
    setFrameShape(QFrame::NoFrame);
    frozenTableView->setFrameShape(QFrame::NoFrame);
    frozenTableView->setGeometry(verticalHeader()->width() /*+ frameWidth()*/,
                                 /*frameWidth()*/0, columnWidth(0),
                                 viewport()->height()+horizontalHeader()->height());
    m_frozenVisibleWidth = columnWidth(0);
}

void BehaviorTableWidget::updateCheckState(QMap<int,QSet<int>> selectedItems)
{
    clearCheckState();
    QSet<int> checkedRows;
    QSet<int> partiallyCheckedRows;
    for(auto cit = selectedItems.cbegin();cit != selectedItems.cend(); cit++) {
        auto item0 = m_model->item(cit.key());
        if(cit.value().size() >= 25 && cit.value().contains(1) && cit.value().contains(25)){
            item0->setCheckState(Qt::Checked);
            checkedRows.insert(cit.key());
        } else if(cit.value().size() > 0 && cit.value().size() < 25) {
            item0->setCheckState(Qt::PartiallyChecked);
            partiallyCheckedRows.insert(cit.key());
        } else {
            item0->setCheckState(Qt::Unchecked);
        }
    }
    int rowCount = m_model->rowCount();
    if(checkedRows.size() == rowCount){
        emit sigChangeSelectAllState(Qt::Checked);
    } else if(checkedRows.size() > 0 || partiallyCheckedRows.size() > 0) {
        emit sigChangeSelectAllState(Qt::PartiallyChecked);
    } else if(checkedRows.size() == 0 && partiallyCheckedRows.size() == 0) {
        emit sigChangeSelectAllState(Qt::Unchecked);
    }
}

void BehaviorTableWidget::clearCheckState()
{
    int rowCount = model()->rowCount();
    for(int i = 0; i < rowCount; i++) {
        auto item0 = m_model->item(i,0);
        item0->setCheckState(Qt::Unchecked);
    }
}

bool BehaviorTableWidget::eventFilter(QObject *object, QEvent *event)
{
    if(frozenTableView && object == frozenTableView->horizontalHeader()->viewport())
    {
        if(event->type() == QEvent::MouseButtonPress
                || event->type() == QEvent::MouseButtonRelease
                || event->type() == QEvent::MouseMove) {
            QApplication::sendEvent(horizontalHeader()->viewport(),event);
        }
    } else if(frozenTableView && object == frozenTableView->viewport()) {
        if(event->type() == QEvent::MouseButtonPress
                || event->type() == QEvent::MouseButtonRelease
                || event->type() == QEvent::MouseMove) {
            QApplication::sendEvent(viewport(),event);
        }
    }
    return false;
}

QString BehaviorTableWidget::getCurveModeString(int runMode)
{
    switch (runMode) {
    case 1: {
        return tr("Line");
    }
        break;
    case 2: {
        return tr("Bezier");
    }
        break;
    default:
        return tr("Line");
        break;
    }
}

int BehaviorTableWidget::getCurveMode(QString displayString)
{
    if(displayString == "Line"){
        return 1;
    } else if(displayString == "Bezier") {
        return 2;
    }
    return 1;
}

//批量修改会触发item修改，item修改统一发送信号
void BehaviorTableWidget::onSetCurve(int curveType)
{
    QString curveStr = getCurveModeString(curveType);
    auto list = selectionModel()->selectedIndexes();
    for(const auto& index : list){
        if(index.column() == 1){
            m_model->item(index.row(),1)->setText(curveStr);
        }
    }
}

void BehaviorTableWidget::onSetServosValue(double value)
{
    auto list = selectionModel()->selectedIndexes();
    for(const auto& index : list){
        if(index.column() >= 2){
            int servoId = index.column() -1;
            if(value >= m_servosRange[servoId].first && value <= m_servosRange[servoId].second) {
                m_model->item(index.row(),index.column())->setText(QString::number(value,'f',1));
            }
        }
    }
}

void BehaviorTableWidget::onSelectAllStateChanged(bool isChecked)
{
    if(isChecked) {
        selectAllItems();
    } else {
        clearAllSelection();
    }
}

void BehaviorTableWidget::onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    auto indexList = selectionModel()->selectedIndexes();
    QSet<QString> selectedCurvesValue;
    QSet<QString> selectedServosValue;
    QMap<int, QMap<int,double>> selectedFrames;
    QMap<int,QSet<int>> selectedRows;
    QModelIndex firstIndex;
    for(auto index : indexList) {
        if(!firstIndex.isValid() || index < firstIndex) {
            firstIndex = index;
        }
        selectedRows[index.row()].insert(index.column());
        /*if(index.column() == CURVE_COLUMN_INDEX) {
            selectedCurvesValue.insert(index.data().toString());
        } else */if(index.column() >= FIRST_SERVO_COLUMN_INDEX) {
            selectedServosValue.insert(index.data().toString());
            int frameIndex = dynamic_cast<QStandardItemModel*>(model())->item(index.row(),0)->text().toInt();
            selectedFrames[frameIndex].insert(index.column()-FIRST_SERVO_COLUMN_INDEX+1,index.data().toDouble());
        }

    }
    //update check state
    updateCheckState(selectedRows);

    //update buttons
    //reversal and mirror
    if(selectedCurvesValue.size() > 0 || selectedServosValue.size() > 0) {
        emit sigEnableReverseAndMirror(true);
    } else {
        emit sigEnableReverseAndMirror(false);
    }
    //delete button
    if(selectedServosValue.size() > 0 && !(selectedServosValue.size() == 1 && selectedServosValue.contains("-"))) {
        emit sigEnableDelete(true);
    } else {
        emit sigEnableDelete(false);
    }
    //curve
    if(selectedCurvesValue.size() > 0) {
        emit sigEnableCurve(true);
    } else {
        emit sigEnableCurve(false);
    }
    //line edit
    if(selectedServosValue.size() == 1 && !selectedServosValue.contains("-")) {
        emit sigSameServoValue(true,selectedServosValue.values()[0].toDouble());
    } else {
        emit sigSameServoValue(false,0);
    }

    //sync with behavioredit
    if(m_isBehaviorSelect) {
        m_isBehaviorSelect = false;
        int scollX = 0;
        int scollY = 0;
        for(int i = FIRST_SERVO_COLUMN_INDEX; i < firstIndex.column(); i++) {
            scollX += columnWidth(i);
        }
        horizontalScrollBar()->setValue(scollX);
        for(int i = 0; i < firstIndex.row(); i++) {
            scollY += rowHeight(i);
        }
        verticalScrollBar()->setValue(scollY);
    } else {
        emit sigSelcectedChanged(selectedFrames);
    }
}

void BehaviorTableWidget::selectItems(const QModelIndex& firstIndex, const QModelIndex& secondIndex, bool selectFrozenTable)
{
    clearAllSelection();
    if(firstIndex.isValid() && secondIndex.isValid()) {
        QItemSelection selection(firstIndex,secondIndex);
        selectionModel()->select(selection,QItemSelectionModel::Select);
    }
    if(selectFrozenTable) {
        QModelIndex frozenFirstIndex = m_model->index(firstIndex.row(),0);
        QModelIndex frozenSecondIndex = m_model->index(secondIndex.row(),0);
        QItemSelection selection(frozenFirstIndex,frozenSecondIndex);
        frozenTableView->selectionModel()->select(selection,QItemSelectionModel::Select);
    }
}

void BehaviorTableWidget::selectRows(const QModelIndex& firstIndex, const QModelIndex& secondIndex)
{
    int columnCount = m_model->columnCount();
    QModelIndex tempFirstIndex = m_model->index(firstIndex.row(),0);
    QModelIndex tempSecondIndex = m_model->index(secondIndex.row(),columnCount-1);
    selectItems(tempFirstIndex,tempSecondIndex,true);
}

void BehaviorTableWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton) {
        m_pressPosIndex = indexAt(event->pos());
        m_frozenPressPosIndex = frozenTableView->indexAt(event->pos());
        if(m_frozenPressPosIndex.column() == 0){
            m_pressItemCheckState = m_model->item(m_frozenPressPosIndex.row(),m_frozenPressPosIndex.column())->checkState();
            selectRows(m_pressPosIndex,m_pressPosIndex);
        } else {
            selectItems(m_pressPosIndex,m_pressPosIndex,false);
        }
    } else if (event->button() == Qt::RightButton) {
        m_rightClickIndex = indexAt(event->pos());
        if(!selectedIndexes().contains(m_rightClickIndex)) {
            if(m_rightClickIndex.column() == 0) {
                selectRows(m_rightClickIndex,m_rightClickIndex);
            } else {
                selectItems(m_rightClickIndex,m_rightClickIndex,false);
            }
        }
        m_pContexMenu->popup(event->globalPos());
    }
    QTableView::mousePressEvent(event);
}

void BehaviorTableWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons().testFlag(Qt::LeftButton) == true) {
        auto movePosIndex = indexAt(event->pos());
        if(m_frozenPressPosIndex.column() == 0){
            selectRows(m_pressPosIndex,movePosIndex);
        } else {
            selectItems(m_pressPosIndex,movePosIndex,false);
        }
    }
    QTableView::mouseMoveEvent(event);
}

void BehaviorTableWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        auto frozenReleaseIndex = frozenTableView->indexAt(event->pos());
        if(frozenReleaseIndex.column() == 0 && frozenReleaseIndex == m_frozenPressPosIndex) {
//            clearAllSelection(); //转由点击事件处理
        }
    }
    QTableView::mouseReleaseEvent(event);
}


void BehaviorTableWidget::onBehaviorEditSelected(const QMap<int, QMap<int,double>> &selectedServos)
{
    qDebug() << "BehaviorTableWidget::onBehaviorEditSelected" << selectedServos;
    if(selectedServos.empty()) {
        if(m_isBehaviorSelect) {
            m_isBehaviorSelect = false;
        }
        clearAllSelection();
        clearCheckState();
        return;
    }
    QModelIndex topLeft;
    QModelIndex bottomRight;
    int firstFrameIndex = -1;
    int lastFrameIndex = -1;
    int topServoColumn = -1;
    int bottomServoColumn = -1;
    for(auto cit = selectedServos.cbegin(); cit != selectedServos.cend(); cit++){
        if(firstFrameIndex == -1){
            firstFrameIndex = lastFrameIndex = cit.key();
        } else {
            lastFrameIndex = cit.key();
        }
        auto servos = cit.value();
        for(auto cit2 = servos.cbegin(); cit2 != servos.cend(); cit2++){
            int servoIdColumn = cit2.key()+1;
            if(topServoColumn == -1){
                topServoColumn = bottomServoColumn = servoIdColumn;
            } else {
                if(servoIdColumn < topServoColumn){
                    topServoColumn = servoIdColumn;
                } else if(servoIdColumn > bottomServoColumn){
                    bottomServoColumn = servoIdColumn;
                }
            }
        }
    }
    int firstRow = DataCenter::getInstance()->getRowIndexOfFrameIndex(firstFrameIndex);
    int lastRow = DataCenter::getInstance()->getRowIndexOfFrameIndex(lastFrameIndex);
    if(firstRow != -1 && lastRow != -1 && topServoColumn != -1 && bottomServoColumn != -1){
        topLeft = m_model->index(firstRow,topServoColumn);
        bottomRight = m_model->index(lastRow,bottomServoColumn);
        if(topLeft.isValid() && bottomRight.isValid()){
            m_isBehaviorSelect = true;
            QItemSelection selection(topLeft, bottomRight);
            clearSelection();
            selectionModel()->select(selection,QItemSelectionModel::Select);
        }
    }
}

void BehaviorTableWidget::onClearSelection()
{
    clearAllSelection(false);
}



void BehaviorTableWidget::selectColumns(int firstColumn, int secondColumn)
{
    int rowCount = m_model->rowCount();
    QModelIndex firstIndex = m_model->index(0,firstColumn);
    QModelIndex secondIndex = m_model->index(rowCount-1,secondColumn);
    selectItems(firstIndex,secondIndex,false);
}

void BehaviorTableWidget::clearAllSelection(bool notify)
{
    clearSelection();
    frozenTableView->clearSelection();
    if(notify) {
        emit sigSelcectedChanged(QMap<int, QMap<int,double>>());
    }
}

void BehaviorTableWidget::selectAllItems()
{
    QModelIndex firstIndex = m_model->index(0,0);
    QModelIndex secondIndex = m_model->index(m_model->rowCount()-1,m_model->columnCount()-1);
    QItemSelection selection(firstIndex,secondIndex);
    selectionModel()->select(selection,QItemSelectionModel::Select);
    firstIndex = m_model->index(0,0);
    secondIndex = m_model->index(m_model->rowCount()-1,0);
    QItemSelection selection2(firstIndex,secondIndex);
    frozenTableView->selectionModel()->select(selection2,QItemSelectionModel::Select);
}

void BehaviorTableWidget::mirrorLeftArmToRight()
{
    emit sigMirror(UBTYanshee::eMirrorTypeLeftArmToRight);
}
void BehaviorTableWidget::mirrorRightArmToLeft()
{
    emit sigMirror(UBTYanshee::eMirrorTypeRightArmToLeft);
}
void BehaviorTableWidget::mirrorLeftLegToRight()
{
    emit sigMirror(UBTYanshee::eMirrorTypeLeftLegToRight);
}
void BehaviorTableWidget::mirrorRightLegToLeft()
{
    emit sigMirror(UBTYanshee::eMirrorTypeRightLegToLeft);
}

void BehaviorTableWidget::reverseFull()
{
    emit sigReverse(UBTYanshee::eReversalTypeFull);
}

void BehaviorTableWidget::reverseHead()
{
    emit sigReverse(UBTYanshee::eReversalTypeHead);
}

void BehaviorTableWidget::reverseArm()
{
    emit sigReverse(UBTYanshee::eReversalTypeArm);
}
void BehaviorTableWidget::reverseLeg()
{
    emit sigReverse(UBTYanshee::eReversalTypeLeg);
}

void BehaviorTableWidget::setStandPos()
{
}

void BehaviorTableWidget::setSquatPos()
{
}

void BehaviorTableWidget::setSpreadArmsPos()
{
}

void BehaviorTableWidget::copySelection()
{
    auto indexList = selectionModel()->selectedIndexes();
    if(!indexList.empty()) {
        copyData_.clear();
        for(auto index : indexList) {
            if(index.column() >= FIRST_SERVO_COLUMN_INDEX) {
                int frameIndex = m_model->index(index.row(),0).data().toInt();
                int servoId = index.column() - FIRST_SERVO_COLUMN_INDEX + 1;
                int servoAngle = index.data().toDouble();
                copyData_[frameIndex].insert(servoId,servoAngle);
            }
        }
        qDebug() << "BehaviorTableWidget::copySelection" << copyData_;
    }
}

void BehaviorTableWidget::pasteSelection()
{
}

void BehaviorTableWidget::onPasteToCurrentRow()
{

}
void BehaviorTableWidget::onPasteToPreviousRow() //insert
{

}
void BehaviorTableWidget::onPasteToNextRow() //insert
{

}

void BehaviorTableWidget::deleteSelection()
{
    auto list = selectionModel()->selectedIndexes();
    QMap<int,QMap<int,double>> frameServos;
    for(const auto& index : list){
        if(index.column() >= 2){
            QString text = m_model->item(index.row(),index.column())->text();
            if(text != "") {
                int frameIndex = m_model->item(index.row(),0)->text().toInt();
                frameServos[frameIndex].insert(index.column()-1,text.toDouble());
            }
        }
    }
    if(!frameServos.empty()) {
        DataCenter::getInstance()->deleteServos(frameServos);
    }
}

void BehaviorTableWidget::initContextMenu()
{
    m_pMirrorMenu = new QMenu(tr("Mirror"));
    m_pRightArmToLeft = new QAction(tr("Arm:RightToLeft"),m_pMirrorMenu);
    m_pMirrorMenu->addAction(m_pRightArmToLeft);
    m_pLeftArmToRight = new QAction(tr("Arm:LeftToRight"),m_pMirrorMenu);
    m_pMirrorMenu->addAction(m_pLeftArmToRight);
    m_pRightLegToLeft = new QAction(tr("Leg:RightToLeft"),m_pMirrorMenu);
    m_pMirrorMenu->addAction(m_pRightLegToLeft);
    m_pLeftLegToRight = new QAction(tr("Leg:LeftToRight"),m_pMirrorMenu);
    m_pMirrorMenu->addAction(m_pLeftLegToRight);
    connect(m_pLeftArmToRight,&QAction::triggered,this,&BehaviorTableWidget::mirrorLeftArmToRight);
    connect(m_pRightArmToLeft,&QAction::triggered,this,&BehaviorTableWidget::mirrorRightArmToLeft);
    connect(m_pLeftLegToRight,&QAction::triggered,this,&BehaviorTableWidget::mirrorLeftLegToRight);
    connect(m_pRightLegToLeft,&QAction::triggered,this,&BehaviorTableWidget::mirrorRightLegToLeft);

    m_pReverseMenu = new QMenu(tr("Reversal"));
    m_pReverseFull = new QAction(tr("Full"),m_pReverseMenu);
    m_pReverseMenu->addAction(m_pReverseFull);
    m_pReverseHead = new QAction(tr("Head"),m_pReverseMenu);
    m_pReverseMenu->addAction(m_pReverseHead);
    m_pReverseArm = new QAction(tr("Arm"),m_pReverseMenu);
    m_pReverseMenu->addAction(m_pReverseArm);
    m_pReverseLeg = new QAction(tr("Leg"),m_pReverseMenu);
    m_pReverseMenu->addAction(m_pReverseLeg);
    connect(m_pReverseFull,&QAction::triggered,this,&BehaviorTableWidget::reverseFull);
    connect(m_pReverseHead,&QAction::triggered,this,&BehaviorTableWidget::reverseHead);
    connect(m_pReverseArm,&QAction::triggered,this,&BehaviorTableWidget::reverseArm);
    connect(m_pReverseLeg,&QAction::triggered,this,&BehaviorTableWidget::reverseLeg);

    m_pPostureMenu = new QMenu(tr("Posture"));
    m_pStandPos = new QAction(tr("Stand"),m_pPostureMenu);
    m_pPostureMenu->addAction(m_pStandPos);
    m_pSquatPos = new QAction(tr("Squat"),m_pPostureMenu);
    m_pPostureMenu->addAction(m_pSquatPos);
    m_pSpreadArmsPos = new QAction(tr("Spread Arms"),m_pPostureMenu);
    m_pPostureMenu->addAction(m_pSpreadArmsPos);
    connect(m_pStandPos,&QAction::triggered,this,&BehaviorTableWidget::setStandPos);
    connect(m_pSquatPos,&QAction::triggered,this,&BehaviorTableWidget::setSquatPos);
    connect(m_pSpreadArmsPos,&QAction::triggered,this,&BehaviorTableWidget::setSpreadArmsPos);
    m_pContexMenu = new QMenu();
    m_pDeleteAction = new QAction(tr("Delete"),m_pContexMenu);
    m_pCopyAction = new QAction(tr("Copy"),m_pContexMenu);
//    m_pPasteAction = new QAction(tr("Paste"),m_pContexMenu);
    connect(m_pCopyAction,&QAction::triggered,this,&BehaviorTableWidget::copySelection);
//    connect(m_pPasteAction,&QAction::triggered,this,&BehaviorTableWidget::pasteSelection);
    connect(m_pDeleteAction,&QAction::triggered,this,&BehaviorTableWidget::deleteSelection);

    auto pasteMenu = new QMenu(tr("Past"));
    auto currentRowAction = new QAction(tr("Current Row"),pasteMenu);
    connect(currentRowAction,&QAction::triggered,[this]{
        if(!copyData_.empty()) {
            int frameIndex = m_model->index(m_rightClickIndex.row(),0).data().toInt();
            int indexDelta = frameIndex - copyData_.firstKey();
            QMap<int, QMap<int,double>> targetFrames;
            for(auto cit = copyData_.cbegin(); cit != copyData_.cend(); cit++) {
                targetFrames.insert(cit.key()+indexDelta,cit.value());
            }
            qDebug() << "past current row" << targetFrames;
            DataCenter::getInstance()->changeServos(targetFrames);
        }
    });
    pasteMenu->addAction(currentRowAction);
    auto previousRowAction = new QAction(tr("Previous Row"),pasteMenu);
    connect(previousRowAction,&QAction::triggered,[this]{
        if(!copyData_.empty()) {
            int frameIndex = m_model->index(m_rightClickIndex.row(),0).data().toInt() - 1;
            if(frameIndex < 1) {
                frameIndex = 1;
            }
            DataCenter::getInstance()->insertCopyedFrames(copyData_,frameIndex);
        }
    });
    pasteMenu->addAction(previousRowAction);
    auto nextRowAction = new QAction(tr("Next Row"),pasteMenu);
    connect(nextRowAction,&QAction::triggered,[this]{
        if(!copyData_.empty()) {
            int frameIndex = m_model->index(m_rightClickIndex.row(),0).data().toInt() + 1;
            DataCenter::getInstance()->insertCopyedFrames(copyData_,frameIndex);
        }
    });
    pasteMenu->addAction(nextRowAction);

    m_pContexMenu->addMenu(m_pReverseMenu);
    m_pContexMenu->addMenu(m_pMirrorMenu);
    m_pContexMenu->addMenu(m_pPostureMenu);
    m_pContexMenu->addAction(m_pCopyAction);
//    m_pContexMenu->addAction(m_pPasteAction);
    m_pContexMenu->addMenu(pasteMenu);
    m_pContexMenu->addAction(m_pDeleteAction);



}

void BehaviorTableWidget::onSelectRow(int row)
{
    auto firstModelIndex = m_model->index(row,1);
    auto lastModelIndex = m_model->index(row,m_model->columnCount() - 1);
    qDebug() << "BehaviorTableWidget::onSelectRow" << firstModelIndex << lastModelIndex;
    selectRows(firstModelIndex,lastModelIndex);
}

