#include "viewwidget.h"

ViewWidget::ViewWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	draw_status_ = false;//初始状态,不绘制.
	shape_ = NULL;//初始状态.
	type_ = Shape::kDefault;//初始图元类型
}

ViewWidget::~ViewWidget()
{
	for (int i = 0; i < shape_list_.size(); i++) {//回收堆中空间
		if (shape_list_[i]) {
			delete shape_list_[i];
			shape_list_[i] = nullptr;
		}
	}
}

void ViewWidget::setLine()//设置当前绘制类型为Line
{
	type_ = Shape::kLine;
}

void ViewWidget::setRect()//设置当前绘制类型为Rect
{
	type_ = Shape::kRect;
}

void ViewWidget::setEllipse()//设置当前绘制类型为Ellipse
{
	type_ = Shape::kEllipse;
}

void ViewWidget::setPolygen() 
{
	type_ = Shape::kPolygen;
}

void ViewWidget::setFreehand()
{
	type_ = Shape::kFreehand;
}
//从QWidget中继承的一个虚函数,会在窗口运行期间的事件循环中自动检测是否在按压鼠标
void ViewWidget::mousePressEvent(QMouseEvent* event)
{
	//下面这个if判断是为了确定目前是否是刚点击鼠标
	if (Qt::LeftButton == event->button())//判断鼠标是否左击
	{
		switch (type_)//判断当前选择绘制的类型,并实例化那个图元,用shape_指向其实例化
		{
			case Shape::kLine:
				shape_ = new Line();
				break;

			case Shape::kDefault:
				break;

			case Shape::kRect:
				shape_ = new Rect();
				break;
			case Shape::kEllipse:
				shape_ = new my_class::Ellipse();
				break;
			
			case Shape::kPolygen:
				if (shape_ == nullptr) {
					shape_ = new my_class::Polygen();
				}
				break;
			case Shape::kFreehand:
				shape_ = new my_class::Freehand();
				break;

		}
		if (shape_ != NULL)
		{
			switch (type_) 
			{
				case Shape::kPolygen:
					setMouseTracking(true);
					if (!draw_status_) {
						draw_status_ = true;
						start_point_ = end_point_ = event->pos();//将图元初始点设置为鼠标点击位置
						shape_->set_start(start_point_);
						shape_->set_end(end_point_);
					}
					else {
						end_point_ = event->pos();
						shape_->set_end(end_point_);
						shape_->AppendLine();
						start_point_ = end_point_;
						shape_->set_start(start_point_);
						shape_->set_end(end_point_);

					}
					break;
				
				default:
					draw_status_ = true;//绘制
					start_point_ = end_point_ = event->pos();//将图元初始点设置为鼠标点击位置
					shape_->set_start(start_point_);
					shape_->set_end(end_point_);
					break;
			}
		}
	}
	if (Qt::RightButton == event->button()&&draw_status_&&type_== Shape::kPolygen) //右击退出多边形
	{
		draw_status_ = false;
		shape_list_.push_back(shape_);//将本次绘制的图形记录到表中
		shape_ = nullptr;
		setMouseTracking(false);
	}
	//单击鼠标之后,在按压的过程,即使是鼠标在移动,本函数仍然在调用,因而只需要在此处update.
	update();//更新窗口此时会调用paintEvent()
}
//同样是从QWidget中继承的一个虚函数
void ViewWidget::mouseMoveEvent(QMouseEvent* event)
{
	//这个if是需要的,因为只要鼠标运动就会触发本事件
	if (draw_status_ && shape_ != NULL)//判断当前是否正在绘制
	{
		end_point_ = event->pos();//将图元终点设置为鼠标当前位置
		shape_->set_end(end_point_);
	}
	if (draw_status_ && type_ == Shape::kPolygen) {
		end_point_ = event->pos();//将图元终点设置为鼠标当前位置
		shape_->set_end(end_point_);
		update();
	}
	
}

//同样是从QWidget中继承的一个虚函数
void ViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (shape_ != NULL && type_ != Shape::kPolygen)//判断是否在绘制
	{
		draw_status_ = false;//结束绘制
		shape_list_.push_back(shape_);//将本次绘制的图形记录到表中
		shape_ = NULL;//结束绘制
	}
}

//同样是从QWidget中继承的一个虚函数,当update()被调用时当前函数会被调用
void ViewWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);//在本控件上创建画布.

	for (int i = 0; i < shape_list_.size(); i++)//在画布上绘制之前记录的图形
	{
		shape_list_[i]->Draw(painter);
	}

	if (shape_ != NULL) {
		shape_->Draw(painter);//在画布上绘制正在绘制的图形
	}

	update();
}