#ifndef MYTABLE_H
#define MYTABLE_H

#include <QTableWidget>
#include <QKeyEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QPen>

class MyTable : public QTableWidget
{
	Q_OBJECT

public:
	MyTable(QWidget *parent);
	~MyTable();

private slots:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyboardSearch(const QString &search);

private:
	//空白色与填充色
	QColor blankColor, filledColor;
	//表格的可编辑状态
	bool m_isEditable;

public:
	//设置表格数据
	void setData(const char Data[9][9], bool init = false);
	//从表格中获取数据
	void getData(char Data[9][9]);
	//清空表格数据
	void clearData();
	//设置可编辑状态
	void setEditable(bool key);
	//获取可编辑状态
	bool isEditable();
};
#endif // MYTABLE_H
