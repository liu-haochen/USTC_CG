#pragma once

#include <QtGui>
//抽象类Shape

class Shape
{
public:
	Shape();
	virtual ~Shape();
	virtual void Draw(QPainter& paint) = 0;//所有继承的类要重写这个绘制函数
	void set_start(QPoint s);//设置绘制图形的起点
	void set_end(QPoint e);//设置绘制图形的终点
	virtual void AppendLine();

public:
	//图元类型
	enum Type 
	{
		kDefault = 0,//空
		kLine = 1,//直线
		kRect = 2,//矩形
		kEllipse = 3,//椭圆
		kPolygen = 4,//多边形
		kFreehand = 5, 
	};

protected:
	QPoint start;//绘制图形的起点
	QPoint end;//绘制图形的终点
};

