#pragma once

#include <ui_minidraw.h>
#include <viewwidget.h>

#include <QtWidgets/QMainWindow>
#include <qmessagebox.h>

class MiniDraw : public QMainWindow {
	Q_OBJECT

public:
	MiniDraw(QWidget* parent = 0);
	~MiniDraw();

	QMenu* pMenu;//�˵�ָ��
	QToolBar* pToolBar;//ToolBarָ��
	QAction* Action_About;//About����ָ��
	QAction* Action_Line;//��ֱ�߶���ָ��
	QAction* Action_Rect;//���ζ���ָ��
	QAction* Action_Ellipse;//��Բ����ָ��
	QAction* Action_Polygen;//����ζ���ָ��
	QAction* Action_Freehand;

	void Creat_Menu();//�����˵�����������ӵ��˵�
	void Creat_ToolBar();//����ToolBar����������ӵ�ToolBar
	void Creat_Action();//������ͼ����

	void AboutBox();//����About��Message

private:
	Ui::MiniDrawClass ui;
	ViewWidget* view_widget_;//����ViewWidgetָ��
};
