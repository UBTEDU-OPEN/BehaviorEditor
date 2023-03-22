#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QCoreApplication>
#include <QTabWidget>
#include <QLabel>
#include "updaterwindow.h"
#include "templatebar.h"
#include "robotactionlist.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class QRobotView;
class BehaviorEdit;
class BehaviorTable;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    void init ();
    void initWindows ();
    void initConnect ();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject* object, QEvent* event) override;

public slots:
    void onClose();
    void onMax();
    void onMin();
    void onNewFile();
    void onOpenFile();
    void onSaveFile();
    void onSaveAs();
    void onHelp();
    void onZoomTabWidget (int index);
    void onLayerChanged(int);
    void onUpdate();
    void onRobotBtnClicked();
    void onTemplateBarItemLongPress(QPoint point,QString filePath);
    void onTemplateBarItemLongPressRelease(QPoint point,QString filePath);
    void onTemplateBarItemLongPressMove(QPoint point);
    void onCtrlKeyEvent(int keyEvent);
    void onNormalKeyEvent(int keyEvent);

public:
    QString getFileName (const QString &title);
    void    autoCheckForUpdates();

private:
    void configStyle();
    void addPreviewTab(int index);

private:
    Ui::MainWindow *ui;
    bool m_bWndMax;
    bool m_bLeftButtonPressed;
    QPoint m_lastPos;
    BehaviorEdit *behaviorEdit_;
    QTabWidget* m_behaviorEditTab;
    QTabWidget* m_previewTab;
    TemplateBar* templateBar;
    RobotActionList* robotActionList;
    UpdaterWindow m_updater;
    bool m_dontShowUpdateWindow;
    QLabel *mTemplateBarLabel;
};
#endif // MAINWINDOW_H
