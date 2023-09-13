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
		virtual void Warp() = 0;//变形
		void setPair(std::vector<QPoint>& vSpoint, std::vector<QPoint>& vEpoint);//设置变形(不动)的点
		void setQimage(QImage *ptr_image);//设置要变形的图片
		void push_back(QPoint&,QPoint&);//插入一个点对
		QImage* getQimage();//获得变形后的图片
	protected :
		struct QPoint_pair {
			QPoint Spoint;
			QPoint Epoint;
		};
		std::vector<QPoint_pair> m_Vpair;//变形(不变形)的点对
		QImage* m_ptr_image;//要变形的图片
		double m_mu;
	};
	bool isInPic(double x, double y, int width, int height);
	std::array<double, 2> Find(double x, double y, std::vector<std::vector<bool>>& isFill, int width, int height);
	double Factor_(double);
	double distant(double x1, double y1, double x2, double y2);

}