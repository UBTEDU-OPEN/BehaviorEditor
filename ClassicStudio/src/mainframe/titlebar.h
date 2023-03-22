#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QWidget>
#include <QMenu>

namespace Ui {
	class titlebar;
}

class TitleBar: public QWidget {
Q_OBJECT

public:
	explicit TitleBar(QWidget *parent = nullptr);
	~TitleBar();

public slots:
    void onEdited();
    void onSaved(const QString& fileName);
    void onOpend(const QString& fileName);
        
protected:
	void mouseDoubleClickEvent(QMouseEvent *event) override;
        
signals:
	void sigMin();
	void sigMax();
	void sigClose();
	void sigPlay();
    void sigLoad();
        
private slots:
	void onFileClicked();
        
private:
	void configStyle();
    void setEditedIconVisible(bool edited);
    void setFileName(const QString& fileName);
        
private:
	Ui::titlebar *ui;
//	QMenu *m_mainMenu;
};

#endif // TITLEBAR_H
