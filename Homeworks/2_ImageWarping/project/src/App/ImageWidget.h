#pragma once
#include <QWidget>
#include "IDW_Warping.h"
#include "shape.h"
#include <vector>
#include "Line.h"
#include "RBF_Warping.h"
QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
QT_END_NAMESPACE

class ImageWidget :
	public QWidget
{
	Q_OBJECT

public:
	ImageWidget(void);
	~ImageWidget(void);

protected:
	void paintEvent(QPaintEvent *paintevent);

public slots:
	// File IO
	void Open();												// Open an image file, support ".bmp, .png, .jpg" format
	void Save();												// Save image to current file
	void SaveAs();												// Save image to another file

	// Image processing
	void Invert();												// Invert pixel value in image
	void Mirror(bool horizontal=false, bool vertical=true);		// Mirror image vertically or horizontally
	void TurnGray();											// Turn image to gray-scale map
	void Restore();												// Restore image to origin
	void IDW_warp();
	void RBF_warp();

	//event
	void mousePressEvent(QMouseEvent* event);//��갴ѹ
	void mouseMoveEvent(QMouseEvent* event);//����ƶ�
	void mouseReleaseEvent(QMouseEvent* event);//����ͷ�
	bool isInImage(QPoint)const;//�жϵ�ǰ���λ���Ƿ��ڴ�ͼƬ��
	QPoint picAix(QPoint&);

private:
	Image_Processing::Warping* m_ptr_warping_;//warping�����һ��ָ��
	Shape* m_p_shape_;//��ֱ��
	std::vector<Shape*> m_v_shape_;
	QImage		*ptr_image_;				// image 
	QImage		*ptr_image_backup_;
	enum warp_status_//�Ƿ�����ʹ��warp
	{
		w_default = 0,
		w_IDW = 1,
		w_RBF = 2,
	} w_status_;
	QPoint SPoint;
	QPoint EPoint;
};

