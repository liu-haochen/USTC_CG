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
	bool draw_status_;//��ǰ����״̬,true��ʾ��������϶�ͼԪ,false��ʾ������
	QPoint start_point_;//ͼԪ��ʼ��
	QPoint end_point_;//ͼԪ�յ�
	Shape::Type type_;//��ǰ����ͼԪ����
	Shape* shape_;//shape�����ָ��
	std::vector<Shape*> shape_list_;//shape����ָ������,ָ������


public:
	void mousePressEvent(QMouseEvent* event);//��꼤����Ӧ����
	void mouseMoveEvent(QMouseEvent* event);//����ƶ���Ӧ����
	void mouseReleaseEvent(QMouseEvent* event);//����ͷ���Ӧ����

public:
	void paintEvent(QPaintEvent*);//���ƺ���
signals:
public slots:
	void setLine();//���õ�ǰ��������Ϊֱ��
	void setRect();//���õ�ǰ��������Ϊ����
	void setEllipse();//���õ�ǰ��������Ϊ��Բ
	void setPolygen();//���õ�ǰ����Ϊ�����
	void setFreehand();
};
