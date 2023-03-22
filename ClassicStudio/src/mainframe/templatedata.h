#ifndef TEMPLATEDATA_H
#define TEMPLATEDATA_H

#include <QList>
#include <QWidget>
#include <QToolButton>

class TemplateData;
class BaseData;
class TypeData;

class TemplateData {
public:
	TemplateData(){ _listTypeData.clear(); }
	~TemplateData(){ clear(); }
        
public:
	void add(TypeData *p){ _listTypeData.append(p); }
	void clear(){
		qDeleteAll(_listTypeData);
		_listTypeData.clear();
	}
	QList<TypeData*>& get(){ return _listTypeData; }
        
private:
	QList<TypeData*> _listTypeData;
};

/**
 * @brief 模块数据相关类
 */
class BaseData {
public:
	explicit BaseData(): _logo(""), _name(""), _btn(nullptr){ m_files.clear(); }
	~BaseData(){ m_files.clear(); _btn = nullptr; }
        
public:
	void add(const QString &path){ m_files.append(path); }
	void clear(){ m_files.clear(); }
	QList<QString>& get(){ return m_files; }
        
	void setLogo(const QString &logo){ _logo = logo; }
	QString getLogo() const { return _logo; }
        
	void setName(const QString &name){ _name = name; }
	QString getName() const { return _name; }
        
	void setToolButton(QToolButton *btn){ _btn = btn; }
	QToolButton* getToolButton() const { return _btn; }

    QList<QString> getFiles(){
        return m_files;
    }
        
private:
	QList<QString> m_files; /**< 保存xml/gif等文件路径 */
	QString _logo;       /**< logo路径 */
	QString _name;       /**< 文件夹名称 */
	QToolButton *_btn;
};

class ItemData {
public:
	explicit ItemData(): _pWidget(nullptr), _name(""), _logo_normal(""), _logo_hovering(""), _logo_selected(""){ _listBaseData.clear(); }
	~ItemData(){ clear(); _pWidget = nullptr; }
        
public:
	void add(BaseData *p){ _listBaseData.append(p); }
	void clear(){
		qDeleteAll(_listBaseData);
		_listBaseData.clear();
	}
	QList<BaseData*>& get(){ return _listBaseData; }
        
	void setName(const QString &name){ _name = name; }
	QString getName() const { return _name; }
        
	void setNormalIcon(const QString &logo_normal){ _logo_normal = logo_normal; }
	void setHoveringIcon(const QString &logo_hovering){ _logo_hovering = logo_hovering; }
	void setSelectedIcon(const QString &logo_selected){
		_logo_selected = logo_selected;
	}
	QString getNormalIcon() const { return _logo_normal; }
	QString getHoveringIcon() const { return _logo_hovering; }
	QString getSelectedIcon() const { return _logo_selected; }
        
	void setWidget(QWidget *p){ _pWidget = p; }
	QWidget* getWidget() const { return _pWidget; }
        
private:
	QList<BaseData*> _listBaseData;
	QWidget *_pWidget;
	QString _name;
	QString _logo_normal;
	QString _logo_hovering;
	QString _logo_selected;
};

class TypeData {
public:
	explicit TypeData(): _name(""){ _listItemData.clear(); }
	~TypeData(){ clear(); }
        
public:
	void add(ItemData *p){ _listItemData.append(p); }
	void clear(){
		qDeleteAll(_listItemData);
		_listItemData.clear();
	}
	QList<ItemData*>& get(){ return _listItemData; }
        
	void setName(const QString &name){ _name = name; }
	QString getName() const { return _name; }
        
private:
	QList<ItemData*> _listItemData;
	QString _name;
};

#endif // TEMPLATEDATA_H
