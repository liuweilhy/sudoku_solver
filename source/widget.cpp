#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "widget.h"
#include <QMessageBox>
#include <QElapsedTimer>
#include <QDebug>

widget::widget(QWidget *parent)
    : QWidget(parent),
resultNo(-1)
{
	ui.setupUi(this);
    ui.comboBox->addItem(tr("很多解"), 0);
    ui.comboBox->addItem(tr("两个解"), 1);
    ui.comboBox->addItem(tr("简单"), 2);
    ui.comboBox->addItem(tr("普通"), 3);
    ui.comboBox->addItem(tr("困难"), 4);
    ui.comboBox->addItem(tr("最难"), 5);
    ui.comboBox->addItem(tr("自定义"), 6);

    connect(ui.editButton, SIGNAL(clicked()), this, SLOT(edit()));
    connect(ui.solveButton, SIGNAL(clicked()), this, SLOT(solve()));
    connect(ui.mBox, SIGNAL(valueChanged(int)), this, SLOT(run(int)));
    connect(ui.aboutButton, SIGNAL(clicked()), this, SLOT(about()));
    connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setDate(int)));
    ui.comboBox->setCurrentIndex(2);
}

widget::~widget()
{
}

bool widget::setDate(int i /*= 0*/)
{
    resultNo = -1;
    if (i >= 0 && i < 7)
    {
        ui.tableWidget->clearSelection();
        ui.tableWidget->setData(array[i], true);
        return true;
    }
    else
    {
        ui.tableWidget->setData(array[6], true);
        return false;
    }
}

void widget::edit()
{
    ui.comboBox->setCurrentIndex(ui.comboBox->count() - 1);
	ui.tableWidget->setEditable(true);
	ui.numEdit->clear();
	ui.timeEdit->clear();
	ui.mBox->setReadOnly(true);
	ui.mBox->setRange(0,0);
}

void widget::solve()
{
	//从表格中获取初值
    ui.tableWidget->getData(mydata);
    shudu.Initialize(mydata);
	switch(shudu.Check())
	{
	case 0:
		QMessageBox::warning(this, tr("警告"), tr("初始数少于17个！"));
		break;
	case -1:
		QMessageBox::warning(this, tr("警告"), tr("表格中只能为1～9的数字！"));
		break;
	case -2:
		QMessageBox::warning(this, tr("警告"), tr("数组未初始化！"));
		break;
	case -3:
		QMessageBox::warning(this, tr("警告"), tr("行、列、宫中有重复的数！"));
		break;
	case 1:
        ui.tableWidget->setEditable(false);
        resultNo = -1;
		unsigned long n = run();
		if(n > 0)
		{
			ui.numEdit->setText(QString::number(n));
			ui.mBox->setReadOnly(false);
			ui.mBox->setRange(1,n);
			ui.mBox->setValue(n);
		}
		else
		{
			ui.tableWidget->setEditable(true);
			QMessageBox::information(this, tr("结果"), tr("此题无解！"));
		}
		break;
	}
}

unsigned long widget::run(int i/* = 0*/)
{
	if(ui.tableWidget->isEditable())
		return 0;
    // 添加一步判断，避免重复计算产生额外时间
    if (i !=0 && i == resultNo + 1)
        return i;

    setCursor(Qt::WaitCursor);
    QElapsedTimer timer;
    timer.start();
    unsigned long result = shudu.Solve(i);
    qint64 tm = timer.elapsed();
    qDebug() << tm;
    if (result >= 0)
    {
        resultNo = result - 1;
        shudu.Output(mydata);
        ui.tableWidget->setData(mydata);
        ui.timeEdit->setText(QString::number(tm)+"ms");
    }
    setCursor(Qt::ArrowCursor);

    return result;
}

void widget::about()
{
    QMessageBox::information(this,tr("说明"), tr("by liuweilhy at 2013.02.05"));
}
