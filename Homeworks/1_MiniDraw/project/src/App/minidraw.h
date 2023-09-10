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

	QMenu* pMenu;//菜单指针
	QToolBar* pToolBar;//ToolBar指针
	QAction* Action_About;//About动作指针
	QAction* Action_Line;//画直线动作指针
	QAction* Action_Rect;//矩形动作指针
	QAction* Action_Ellipse;//椭圆动作指针
	QAction* Action_Polygen;//多边形动作指针
	QAction* Action_Freehand;

	void Creat_Menu();//创建菜单并将动作添加到菜单
	void Creat_ToolBar();//创建ToolBar并将动作添加到ToolBar
	void Creat_Action();//创建画图动作

	void AboutBox();//发送About的Message

private:
	Ui::MiniDrawClass ui;
	ViewWidget* view_widget_;//声明ViewWidget指针
};
