#include "minidraw.h"

#include <QToolBar>

MiniDraw::MiniDraw(QWidget* parent)
	: QMainWindow(parent)
{
	//��ͼ�ؼ�ʵ����
	ui.setupUi(this);
	view_widget_ = new ViewWidget();//ʵ����ViewWidget����.
	Creat_Action();
	Creat_ToolBar();
	Creat_Menu();

	setCentralWidget(view_widget_);//��ViewWidget��������Ϊ����������λ��.
}

void MiniDraw::Creat_Action() {//�������󶨶���
	Action_About = new QAction(tr("&About"), this);
	connect(Action_About, &QAction::triggered, this, &MiniDraw::AboutBox);//��About������Action_About

	Action_Line = new QAction(tr("&Line"), this);
	connect(Action_Line, SIGNAL(triggered()), view_widget_, SLOT(setLine()));//�󶨻�ֱ�߶�����Action_About

	Action_Rect = new QAction(tr("&Rect"), this);
	connect(Action_Rect, &QAction::triggered, view_widget_, &ViewWidget::setRect);//�󶨻����ζ�����Action_Rect
	
	Action_Ellipse = new QAction(tr("&Ellipse"), this);
	connect(Action_Ellipse, &QAction::triggered, view_widget_, &ViewWidget::setEllipse);//�󶨻���Բ������Action_Ellipse

	Action_Polygen = new QAction(tr("&Polygen"), this);
	connect(Action_Polygen, &QAction::triggered, view_widget_, &ViewWidget::setPolygen);

	Action_Freehand = new QAction(tr("&Freehand"), this);
	connect(Action_Freehand, &QAction::triggered, view_widget_, &ViewWidget::setFreehand);
}

void MiniDraw::Creat_ToolBar() {
	pToolBar = addToolBar(tr("&Main"));//����ToolBar
	pToolBar->addAction(Action_About);//��About������ӵ�ToolBar��
	pToolBar->addAction(Action_Line);//����ֱ�߶�����ӵ�ToolBar��
	pToolBar->addAction(Action_Rect);//�������ζ�����ӵ�ToolBar��
	pToolBar->addAction(Action_Ellipse);//������Բ������ӵ�ToolBar��
	pToolBar->addAction(Action_Polygen);
	pToolBar->addAction(Action_Freehand);
}

void MiniDraw::Creat_Menu() {
	pMenu = menuBar()->addMenu(tr("&Figure Tool"));//�����˵�
	pMenu->addAction(Action_About);//��About������ӵ��˵���
	pMenu->addAction(Action_Line);//����ֱ�߶�����ӵ��˵���
	pMenu->addAction(Action_Rect);//�������ζ�����ӵ��˵���
	pMenu->addAction(Action_Ellipse);//������Բ������ӵ��˵���
	pMenu->addAction(Action_Polygen);
	pMenu->addAction(Action_Freehand);
}

void MiniDraw::AboutBox() {
	QMessageBox::about(this, tr("About"), tr("MiniDraw"));//����Message����
}

MiniDraw::~MiniDraw() {}
