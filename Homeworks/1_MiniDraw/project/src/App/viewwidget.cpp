#include "viewwidget.h"

ViewWidget::ViewWidget(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	draw_status_ = false;//��ʼ״̬,������.
	shape_ = NULL;//��ʼ״̬.
	type_ = Shape::kDefault;//��ʼͼԪ����
}

ViewWidget::~ViewWidget()
{
	for (int i = 0; i < shape_list_.size(); i++) {//���ն��пռ�
		if (shape_list_[i]) {
			delete shape_list_[i];
			shape_list_[i] = nullptr;
		}
	}
}

void ViewWidget::setLine()//���õ�ǰ��������ΪLine
{
	type_ = Shape::kLine;
}

void ViewWidget::setRect()//���õ�ǰ��������ΪRect
{
	type_ = Shape::kRect;
}

void ViewWidget::setEllipse()//���õ�ǰ��������ΪEllipse
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
//��QWidget�м̳е�һ���麯��,���ڴ��������ڼ���¼�ѭ�����Զ�����Ƿ��ڰ�ѹ���
void ViewWidget::mousePressEvent(QMouseEvent* event)
{
	//�������if�ж���Ϊ��ȷ��Ŀǰ�Ƿ��Ǹյ�����
	if (Qt::LeftButton == event->button())//�ж�����Ƿ����
	{
		switch (type_)//�жϵ�ǰѡ����Ƶ�����,��ʵ�����Ǹ�ͼԪ,��shape_ָ����ʵ����
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
						start_point_ = end_point_ = event->pos();//��ͼԪ��ʼ������Ϊ�����λ��
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
					draw_status_ = true;//����
					start_point_ = end_point_ = event->pos();//��ͼԪ��ʼ������Ϊ�����λ��
					shape_->set_start(start_point_);
					shape_->set_end(end_point_);
					break;
			}
		}
	}
	if (Qt::RightButton == event->button()&&draw_status_&&type_== Shape::kPolygen) //�һ��˳������
	{
		draw_status_ = false;
		shape_list_.push_back(shape_);//�����λ��Ƶ�ͼ�μ�¼������
		shape_ = nullptr;
		setMouseTracking(false);
	}
	//�������֮��,�ڰ�ѹ�Ĺ���,��ʹ��������ƶ�,��������Ȼ�ڵ���,���ֻ��Ҫ�ڴ˴�update.
	update();//���´��ڴ�ʱ�����paintEvent()
}
//ͬ���Ǵ�QWidget�м̳е�һ���麯��
void ViewWidget::mouseMoveEvent(QMouseEvent* event)
{
	//���if����Ҫ��,��ΪֻҪ����˶��ͻᴥ�����¼�
	if (draw_status_ && shape_ != NULL)//�жϵ�ǰ�Ƿ����ڻ���
	{
		end_point_ = event->pos();//��ͼԪ�յ�����Ϊ��굱ǰλ��
		shape_->set_end(end_point_);
	}
	if (draw_status_ && type_ == Shape::kPolygen) {
		end_point_ = event->pos();//��ͼԪ�յ�����Ϊ��굱ǰλ��
		shape_->set_end(end_point_);
		update();
	}
	
}

//ͬ���Ǵ�QWidget�м̳е�һ���麯��
void ViewWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (shape_ != NULL && type_ != Shape::kPolygen)//�ж��Ƿ��ڻ���
	{
		draw_status_ = false;//��������
		shape_list_.push_back(shape_);//�����λ��Ƶ�ͼ�μ�¼������
		shape_ = NULL;//��������
	}
}

//ͬ���Ǵ�QWidget�м̳е�һ���麯��,��update()������ʱ��ǰ�����ᱻ����
void ViewWidget::paintEvent(QPaintEvent*)
{
	QPainter painter(this);//�ڱ��ؼ��ϴ�������.

	for (int i = 0; i < shape_list_.size(); i++)//�ڻ����ϻ���֮ǰ��¼��ͼ��
	{
		shape_list_[i]->Draw(painter);
	}

	if (shape_ != NULL) {
		shape_->Draw(painter);//�ڻ����ϻ������ڻ��Ƶ�ͼ��
	}

	update();
}