#include "ImageWidget.h"
#include <QImage>
#include <QPainter>
#include <QtWidgets> 
#include <iostream>

using std::cout;
using std::endl;

ImageWidget::ImageWidget(void)
{
	ptr_image_ = new QImage();
	ptr_image_backup_ = new QImage();
	m_ptr_warping_ = nullptr;
	m_p_shape_ = nullptr;
	w_status_ = warp_status_::w_default;
}


ImageWidget::~ImageWidget(void)
{
}

void ImageWidget::paintEvent(QPaintEvent *paintevent)
{
	QPainter painter;
	
	painter.begin(this);


	// Draw background
	painter.setBrush(Qt::lightGray);
	QRect back_rect(0, 0, width(), height());
	painter.drawRect(back_rect);

	// Draw image
	QRect rect = QRect( (width()-ptr_image_->width())/2, (height()-ptr_image_->height())/2, ptr_image_->width(), ptr_image_->height());
	painter.drawImage(rect, *ptr_image_); 

	painter.end();


	QPainter painter2(this);
	if (m_p_shape_ != nullptr) {
		m_p_shape_->Draw(painter2);
	}
	for (auto& Ashape_ : m_v_shape_) {
		Ashape_->Draw(painter2);
	}
}

void ImageWidget::Open()
{
	// Open file
	QString fileName = QFileDialog::getOpenFileName(this, tr("Read Image"), ".", tr("Images(*.bmp *.png *.jpg)"));

	// Load file
	if (!fileName.isEmpty())
	{
		ptr_image_->load(fileName);
		*(ptr_image_backup_) = *(ptr_image_);
	}

	//ptr_image_->invertPixels(QImage::InvertRgb);
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	//*(ptr_image_) = ptr_image_->rgbSwapped();
	cout<<"image size: "<<ptr_image_->width()<<' '<<ptr_image_->height()<<endl;
	update();
}

void ImageWidget::Save()
{
	SaveAs();
}

void ImageWidget::SaveAs()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Image"), ".", tr("Images(*.bmp *.png *.jpg)"));
	if (filename.isNull())
	{
		return;
	}	

	ptr_image_->save(filename);
}

void ImageWidget::Invert()
{
	for (int i=0; i<ptr_image_->width(); i++)
	{
		for (int j=0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			ptr_image_->setPixel(i, j, qRgb(255-qRed(color), 255-qGreen(color), 255-qBlue(color)) );
		}
	}

	// equivalent member function of class QImage
	// ptr_image_->invertPixels(QImage::InvertRgb);
	update();
}

void ImageWidget::Mirror(bool ishorizontal, bool isvertical)
{
	QImage image_tmp(*(ptr_image_));
	int width = ptr_image_->width();
	int height = ptr_image_->height();

	if (ishorizontal)
	{
		if (isvertical)
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, height-1-j));
				}
			}
		} 
		else			//仅水平翻转			
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(width-1-i, j));
				}
			}
		}
		
	}
	else
	{
		if (isvertical)		//仅垂直翻转
		{
			for (int i=0; i<width; i++)
			{
				for (int j=0; j<height; j++)
				{
					ptr_image_->setPixel(i, j, image_tmp.pixel(i, height-1-j));
				}
			}
		}
	}

	// equivalent member function of class QImage
	//*(ptr_image_) = ptr_image_->mirrored(true, true);
	update();
}

void ImageWidget::TurnGray()
{
	for (int i=0; i<ptr_image_->width(); i++)
	{
		for (int j=0; j<ptr_image_->height(); j++)
		{
			QRgb color = ptr_image_->pixel(i, j);
			int gray_value = (qRed(color)+qGreen(color)+qBlue(color))/3;
			ptr_image_->setPixel(i, j, qRgb(gray_value, gray_value, gray_value) );
		}
	}

	update();
}

void ImageWidget::Restore()
{
	*(ptr_image_) = *(ptr_image_backup_);
	update();
}

void ImageWidget::mousePressEvent(QMouseEvent*event)
{
	//qDebug()<<event->pos()<<" "<<ptr_image_->width()<<" "<<width()<<" " << event->pos().rx() - (width() - ptr_image_->width()) / 2;
	//qDebug() << isInImage(event->pos());
	if (Qt::LeftButton == event->button()&& w_status_!=warp_status_::w_default) {
		qDebug()<<"here";
		m_p_shape_ = new Line;
		SPoint = EPoint = event->pos();
		m_p_shape_->set_start(SPoint);
		m_p_shape_->set_end(EPoint);
	}
	if (Qt::RightButton == event->button()) {
		switch (w_status_)
		{
		case ImageWidget::w_default:
			break;
		default:
			m_ptr_warping_->Warp();
			ptr_image_ = new QImage;
			*ptr_image_ = *(m_ptr_warping_->getQimage());
			if (m_p_shape_ != nullptr) {
				delete m_p_shape_;
			}
			m_v_shape_.clear();
		break;
		}
	}
	update();
}

void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (m_p_shape_ != nullptr && w_status_ != warp_status_::w_default) {
		EPoint = event->pos();
		m_p_shape_->set_end(EPoint);
		update();
	}
}

void ImageWidget::mouseReleaseEvent(QMouseEvent* event)
{
	if (Qt::LeftButton==event->button() && w_status_ != ImageWidget::w_default&&m_p_shape_!=nullptr) {
		EPoint = event->pos();
		m_p_shape_->set_end(EPoint);
		if (isInImage(SPoint)&& isInImage(EPoint) ) {
			m_v_shape_.push_back(m_p_shape_);
			m_ptr_warping_->push_back(picAix(SPoint), picAix(EPoint));
		}
		else {
			delete m_p_shape_;
		}
		m_p_shape_=nullptr;
	}
	update();
}

bool ImageWidget::isInImage(QPoint Qp)const 
{
	return ((Qp.rx() - (width() - ptr_image_->width()) / 2) > 0)
		&& ((Qp.rx() - (width() - ptr_image_->width()) / 2) < ptr_image_->width())
		&& ((Qp.ry() - (height() - ptr_image_->height()) / 2) > 0 )
		&& ((Qp.ry() - (height() - ptr_image_->height()) / 2) < ptr_image_->height());
}

QPoint ImageWidget::picAix(QPoint& Qp)
{
	QPoint P;
	P.setX(Qp.rx() - (width() - ptr_image_->width()) / 2);
	P.setY(Qp.ry() - (height() - ptr_image_->height()) / 2);
	return P;
}
void ImageWidget::IDW_warp() 
{
	if (w_status_ == warp_status_::w_default)
	{
		if (m_ptr_warping_ != nullptr)
		{
			delete m_ptr_warping_;
		}

		m_ptr_warping_ = new Image_Processing::IDW_Warping();
		m_ptr_warping_->setQimage(ptr_image_);
		w_status_ = warp_status_::w_IDW;
	}
	else if(w_status_== warp_status_::w_IDW)
	{
		if (m_ptr_warping_ != nullptr)
		{
			delete m_ptr_warping_;
			m_ptr_warping_ = nullptr;
		}
		w_status_ = warp_status_::w_default;
	}
}

void ImageWidget::RBF_warp() 
{
	if (w_status_ == warp_status_::w_default)
	{
		if (m_ptr_warping_ != nullptr)
		{
			delete m_ptr_warping_;
		}

		m_ptr_warping_ = new Image_Processing::RBF_Warping();
		m_ptr_warping_->setQimage(ptr_image_);
		w_status_ = warp_status_::w_RBF;
	}
	else if (w_status_ == warp_status_::w_RBF)
	{
		if (m_ptr_warping_ != nullptr)
		{
			delete m_ptr_warping_;
			m_ptr_warping_ = nullptr;
		}
		w_status_ = warp_status_::w_default;
	}
}