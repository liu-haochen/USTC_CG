#include "minidraw.h"

#include <QToolBar>

MiniDraw::MiniDraw(QWidget* parent)
	: QMainWindow(parent)
{
	//绘图控件实例化
	ui.setupUi(this);
	view_widget_ = new ViewWidget();//实例化ViewWidget窗口.
	Creat_Action();
	Creat_ToolBar();
	Creat_Menu();

	setCentralWidget(view_widget_);//将ViewWidget窗口设置为主窗口中心位置.
}

void MiniDraw::Creat_Action() {//创建并绑定动作
	Action_About = new QAction(tr("&About"), this);
	connect(Action_About, &QAction::triggered, this, &MiniDraw::AboutBox);//绑定About动作到Action_About

	Action_Line = new QAction(tr("&Line"), this);
	connect(Action_Line, SIGNAL(triggered()), view_widget_, SLOT(setLine()));//绑定画直线动作到Action_About

	Action_Rect = new QAction(tr("&Rect"), this);
	connect(Action_Rect, &QAction::triggered, view_widget_, &ViewWidget::setRect);//绑定画矩形动作到Action_Rect
	
	Action_Ellipse = new QAction(tr("&Ellipse"), this);
	connect(Action_Ellipse, &QAction::triggered, view_widget_, &ViewWidget::setEllipse);//绑定画椭圆动作到Action_Ellipse

	Action_Polygen = new QAction(tr("&Polygen"), this);
	connect(Action_Polygen, &QAction::triggered, view_widget_, &ViewWidget::setPolygen);

	Action_Freehand = new QAction(tr("&Freehand"), this);
	connect(Action_Freehand, &QAction::triggered, view_widget_, &ViewWidget::setFreehand);
}

void MiniDraw::Creat_ToolBar() {
	pToolBar = addToolBar(tr("&Main"));//创建ToolBar
	pToolBar->addAction(Action_About);//将About动作添加到ToolBar上
	pToolBar->addAction(Action_Line);//将画直线动作添加到ToolBar上
	pToolBar->addAction(Action_Rect);//将画矩形动作添加到ToolBar上
	pToolBar->addAction(Action_Ellipse);//将画椭圆动作添加到ToolBar上
	pToolBar->addAction(Action_Polygen);
	pToolBar->addAction(Action_Freehand);
}

void MiniDraw::Creat_Menu() {
	pMenu = menuBar()->addMenu(tr("&Figure Tool"));//创建菜单
	pMenu->addAction(Action_About);//将About动作添加到菜单上
	pMenu->addAction(Action_Line);//将画直线动作添加到菜单上
	pMenu->addAction(Action_Rect);//将画矩形动作添加到菜单上
	pMenu->addAction(Action_Ellipse);//将画椭圆动作添加到菜单上
	pMenu->addAction(Action_Polygen);
	pMenu->addAction(Action_Freehand);
}

void MiniDraw::AboutBox() {
	QMessageBox::about(this, tr("About"), tr("MiniDraw"));//弹出Message窗口
}

MiniDraw::~MiniDraw() {}
