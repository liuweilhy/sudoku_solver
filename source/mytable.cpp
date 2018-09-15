#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "mytable.h"
#include <QDebug>

MyTable::MyTable(QWidget *parent)
	: QTableWidget(parent)
{
	m_isEditable = true;
	blankColor = Qt::white;
	filledColor = Qt::gray;
	setRowCount(9);
	setColumnCount(9);
    for(int i=0; i<9; i++)
	{
		for(int j=0; j<9; j++)
		{
			QTableWidgetItem * newItem = new QTableWidgetItem();
			newItem->setTextAlignment(Qt::AlignCenter);
			newItem->setBackgroundColor(blankColor);
			newItem->setText(QString(' '));
			setItem(i,j,newItem);
		}
	}
}

MyTable::~MyTable()
{

}

void MyTable::setData(const char Data[9][9], bool init/* = false*/)
{
	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
			if(Data[i][j] >= 1 && Data[i][j] <= 9)
			{
				item(i,j)->setText(QString::number(Data[i][j]));
				if(init)
					item(i,j)->setBackgroundColor(filledColor);
			}
			else
			{
				item(i,j)->setText(QString(' '));
				item(i,j)->setBackgroundColor(blankColor);
			}
}

void MyTable::getData(char Data[9][9])
{
	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
		{
            // 仅提取初设值
            if (item(i,j)->backgroundColor() == filledColor)
            {
                //QString a = item(i,j)->text();
                Data[i][j] = (char)(item(i,j)->text().toInt());
            }
            else
            {
                // 其余置0
                Data[i][j] = (char)0;
            }
		}
}

void MyTable::clearData()
{
	for(int i=0; i<9; i++)
		for(int j=0; j<9; j++)
		{
			item(i,j)->setText(QString(' '));
			item(i,j)->setBackgroundColor(blankColor);
		}
}

void MyTable::setEditable(bool key)
{
	m_isEditable = key;
}

bool MyTable::isEditable()
{
	return m_isEditable;
}

void MyTable::resizeEvent(QResizeEvent *event)
{
	int l = width() < height() ? width() : height();
	int x = l/9;
	for(int i = 0; i < 9; i++)
	{
		setRowHeight(i,x);
		setColumnWidth(i,x);
	}
}

void MyTable::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_1:
	case Qt::Key_2:
	case Qt::Key_3:
	case Qt::Key_4:
	case Qt::Key_5:
	case Qt::Key_6:
	case Qt::Key_7:
	case Qt::Key_8:
	case Qt::Key_9:
		if(m_isEditable)
		{
			currentItem()->setText(event->text());
			//QString a = event->text();
			//QString b = currentItem()->text();
			currentItem()->setBackgroundColor(filledColor);
		}
        qDebug() << event->key();
		break;
	case Qt::Key_0:
	case Qt::Key_Space:
	case Qt::Key_Delete:
        // 如果输入0或空格或删除，则清空单元格里的值
		if(m_isEditable)
		{
			currentItem()->setText(QString(' '));
			currentItem()->setBackgroundColor(blankColor);
		}
        qDebug() << event->key();
        break;
	default:
        QTableWidget::keyPressEvent(event);
	}
}

// 必须屏蔽掉原keyboardSearch函数
void MyTable::keyboardSearch(const QString &search)
{
    qDebug() << search;
    if(!m_isEditable)
        return;

    // 在shift状态下不能接收keyPressEvent，所以在此手动赋值
    if (search >= "1" && search <= "9")
    {
        currentItem()->setText(search);
        currentItem()->setBackgroundColor(filledColor);
    }
    else if (search == " " || search == "0")
    {
        currentItem()->setText(" ");
        currentItem()->setBackgroundColor(blankColor);
    }
}

