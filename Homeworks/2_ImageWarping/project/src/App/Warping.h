#pragma once
#include <QWidget>
#include <vector>

namespace Image_Processing {
	class Warping {
	public:
		Warping();
		Warping(std::vector<QPoint>& vSpoint, std::vector<QPoint>& vEpoint);
		Warping(std::vector<QPoint>& vSpoint, std::vector<QPoint>& vEpoint, QImage* ptr_image);
		~Warping();
		virtual void Warp() = 0;//����
		void setPair(std::vector<QPoint>& vSpoint, std::vector<QPoint>& vEpoint);//���ñ���(����)�ĵ�
		void setQimage(QImage *ptr_image);//����Ҫ���ε�ͼƬ
		void push_back(QPoint&,QPoint&);//����һ�����
		QImage* getQimage();//��ñ��κ��ͼƬ
	protected :
		struct QPoint_pair {
			QPoint Spoint;
			QPoint Epoint;
		};
		std::vector<QPoint_pair> m_Vpair;//����(������)�ĵ��
		QImage* m_ptr_image;//Ҫ���ε�ͼƬ
		double m_mu;
	};
	bool isInPic(double x, double y, int width, int height);
	std::array<double, 2> Find(double x, double y, std::vector<std::vector<bool>>& isFill, int width, int height);
	double Factor_(double);
	double distant(double x1, double y1, double x2, double y2);

}