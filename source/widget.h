#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include <QMessageBox>
#include <QTime>
#include "ui_widget.h"
#include "mytable.h"
#include "sudoku.h"

class widget : public QWidget
{
	Q_OBJECT

public:
	widget(QWidget *parent = 0);
	~widget();

private:
	Ui::widgetClass ui;

private:
	//数据交换表
	char Data[9][9];
	//数独对象
	Sudoku shudu;

public slots:
	//重新设置表格
	void reset();
	//解题
	void solve();
	//计算
	unsigned long run(int i = 0);
	//关于
	void about();
};

#endif // WIDGET_H
