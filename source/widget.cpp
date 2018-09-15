#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "widget.h"

widget::widget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QObject::connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(reset()));
	QObject::connect(ui.solveButton, SIGNAL(clicked()), this, SLOT(solve()));
	QObject::connect(ui.mBox, SIGNAL(valueChanged(int)), this, SLOT(run(int)));
	QObject::connect(ui.aboutButton, SIGNAL(clicked()), this, SLOT(about()));
	//实验用数组
    char g_little[9][9]=
    {
		1,2,3,4,5,6,7,8,9,
		4,5,6,7,8,9,1,2,3,
		7,8,9,1,2,3,4,5,6,
		2,3,4,5,6,7,8,9,1,
		5,6,7,8,9,1,2,3,4,
		8,9
	};

	char g_simple[9][9]=
	{
		0,0,4,0,0,7,0,2,8,
		6,3,8,1,2,4,0,0,7,
		9,7,2,0,5,0,4,0,0,
		5,0,3,9,0,0,0,7,0,
		4,9,0,5,7,0,2,0,0,
		2,0,0,0,0,0,5,0,6,
		0,0,0,8,0,0,7,0,0,
		8,4,9,7,1,5,0,6,2,
		0,1,5,0,6,0,8,0,9
	};

	char g_multi[9][9]=
	{
		0,5,0,0,3,6,9,0,0,
		3,4,0,7,0,0,2,5,0,
		8,0,0,9,0,5,0,0,3,
		0,2,4,0,9,0,1,0,0,
		6,0,0,2,0,1,0,9,7,
		0,0,5,0,0,4,0,0,0,
		0,9,0,1,8,2,6,0,4,
		0,8,6,0,4,0,0,0,0,
		4,7,0,5,0,0,8,0,0
	};


	char g_hard[9][9]=
	{
		4,7,9,2,6,0,1,0,0,
		0,2,5,0,1,0,0,3,0,
		1,0,0,0,4,0,0,9,0,
		0,0,4,0,5,7,6,0,0,
		0,0,0,8,0,0,7,0,0,
		0,0,6,0,0,1,0,0,0,
		3,5,2,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,5,
		0,0,0,0,0,0,0,0,0
	};

	char g_hardest[9][9]=
	{
		8,0,0,0,0,0,0,0,0,
		0,0,3,6,0,0,0,0,0,
		0,7,0,0,9,0,2,0,0,
		0,5,0,0,0,7,0,0,0,
		0,0,0,0,4,5,7,0,0,
		0,0,0,1,0,0,0,3,0,
		0,0,1,0,0,0,0,6,8,
		0,0,8,5,0,0,0,1,0,
		0,9,0,0,0,0,4,0,0
	};
	ui.tableWidget->setData(g_hard, true);
}

widget::~widget()
{
}

void widget::reset()
{
	ui.tableWidget->clearData();
	ui.tableWidget->setEditable(true);
	ui.numEdit->clear();
	ui.timeEdit->clear();
	ui.mBox->setReadOnly(true);
	ui.mBox->setRange(0,0);
}

void widget::solve()
{
	//从表格中获取初值
	ui.tableWidget->getData(Data);
	shudu.Initialize(Data);
	switch(shudu.Check())
	{
	case 0:
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("初始数少于17个！"));
		break;
	case -1:
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("表格中只能为1～9的数字！"));
		break;
	case -2:
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("数组未初始化！"));
		break;
	case -3:
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("行、列、宫中有重复的数！"));
		break;
	case 1:
		ui.tableWidget->setEditable(false);
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
			QMessageBox::information(this, QStringLiteral("结果"), QStringLiteral("此题无解！"));
		}
		break;
	}
}

unsigned long widget::run(int i/* = 0*/)
{
	if(ui.tableWidget->isEditable())
		return 0;
	QTime timeTake;
	timeTake.start();
	unsigned long result =  shudu.Solve(i);
	int time = timeTake.elapsed();
	shudu.Output(Data);
	ui.tableWidget->setData(Data);
	ui.timeEdit->setText(QString::number(time)+"ms");
	return result;
}

void widget::about()
{
    QMessageBox::information(this,tr("说明"), tr("by liuweilhy at 2013.02.05"));
}
