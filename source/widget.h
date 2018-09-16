#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include "sudoku.h"
#include "ui_widget.h"
#include "mytable.h"

class widget : public QWidget
{
	Q_OBJECT

public:
	widget(QWidget *parent = 0);
	~widget();

private:
	Ui::widgetClass ui;

    // 预置数组
    static const char array[7][9][9];

    // 数据交换表
    char mydata[9][9];
    // 数独对象
	Sudoku shudu;
    // 当前解的序号
    int resultNo;

public slots:
    // 设置预置数组
    bool setDate(int i = 0);
    // 编辑表格
    void edit();
    // 解题
	void solve();
    // 计算第i个解
	unsigned long run(int i = 0);
    // 关于
	void about();
};

#endif // WIDGET_H
