#pragma once

#include <ui_viewwidget.h>

#include "Shape.h"
#include "Line.h"
#include "Rect.h"
#include "Ellipse.h"
#include "Polygen.h"
#include "Freehand.h"

#include <qevent.h>
#include <qpainter.h>
#include <QWidget>

#include <vector>

class ViewWidget : public QWidget
{
	Q_OBJECT

public:
	ViewWidget(QWidget* parent = 0);
	~ViewWidget();

private:
	Ui::ViewWidget ui;

private:
	bool draw_status_;//当前绘制状态,true表示鼠标正在拖动图元,false表示不绘制
	QPoint start_point_;//图元起始点
	QPoint end_point_;//图元终点
	Shape::Type type_;//当前绘制图元类型
	Shape* shape_;//shape父类的指针
	std::vector<Shape*> shape_list_;//shape父类指针链表,指向子类


public:
	void mousePressEvent(QMouseEvent* event);//鼠标激发响应函数
	void mouseMoveEvent(QMouseEvent* event);//鼠标移动响应函数
	void mouseReleaseEvent(QMouseEvent* event);//鼠标释放响应函数

public:
	void paintEvent(QPaintEvent*);//绘制函数
signals:
public slots:
	void setLine();//设置当前绘制类型为直线
	void setRect();//设置当前绘制类型为矩形
	void setEllipse();//设置当前绘制类型为椭圆
	void setPolygen();//设置当前类型为多边形
	void setFreehand();
};
