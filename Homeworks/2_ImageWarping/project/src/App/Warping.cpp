#include "Warping.h"
#include<array>

#define Distant_Coef 1
namespace Image_Processing {
	Warping::Warping() {
		m_mu = Distant_Coef;
		m_ptr_image = nullptr;
	}
	Warping::Warping(std::vector<QPoint>& vSpoint, std::vector<QPoint>& vEpoint) {
		m_mu = Distant_Coef;
		m_ptr_image = nullptr;
		setPair(vSpoint, vEpoint);
	}
	Warping::Warping(std::vector<QPoint>& vSpoint, std::vector<QPoint>& vEpoint, QImage* ptr_image) {
		m_mu = Distant_Coef;
		m_ptr_image = nullptr;
		setPair(vSpoint, vEpoint);
		setQimage(ptr_image);
	}
	Warping::~Warping(){
	}
	void Warping::setPair(std::vector<QPoint>& vSpoint, std::vector<QPoint>& vEpoint) {
		assert(vSpoint.size() == vEpoint.size());
		QPoint_pair qpTemp ;
		for (int i = 0; i < vSpoint.size(); i++) {
			qpTemp.Spoint = vSpoint[i];
			qpTemp.Epoint = vEpoint[i];
			m_Vpair.push_back(qpTemp);
		}
	}
	void Warping::setQimage(QImage* ptr_image) {
		if (m_ptr_image != nullptr) {
			delete m_ptr_image;
			m_ptr_image = nullptr;
		}
		m_ptr_image = new QImage;
		*m_ptr_image = *ptr_image;
	}
	void Warping::push_back(QPoint&Spoint,QPoint&Epoint){
		QPoint_pair QTemp_pair;
		QTemp_pair.Spoint = Spoint;
		QTemp_pair.Epoint = Epoint;
		m_Vpair.push_back(QTemp_pair);
	}
	QImage* Warping::getQimage(){
		return m_ptr_image;
	}

	double Factor_(double f) {
		if (f < 1e-20) {
			return 1e20;
		}
		else {
			return 1 / f;
		}
	}
	double distant(double x1, double y1, double x2, double y2) {
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	}
	bool isInPic(double x, double y, int width, int height) {
		return x >= 0
			&& y >= 0
			&& x < width
			&& y < height;
	}
	std::array<double, 2> Find(double x, double y, std::vector<std::vector<bool>>& isFill, int width, int height)
	{
		int Maxi = (width > height ? width : height) / 10;
		std::array<double, 2> arr;
		arr[0] = -1;
		arr[1] = -1;
		for (int i = 0; i < Maxi; i++) {
			if (isInPic(x + i, y, width, height)) {
				if (isFill[x + i][y]) {
					arr[0] = x + i; arr[1] = y;
					return arr;
				}
			}
			if (isInPic(x - i, y, width, height)) {
				if (isFill[x - i][y]) {
					arr[0] = x - i; arr[1] = y;
					return arr;
				}
			}
			if (isInPic(x, y + i, width, height)) {
				if (isFill[x][y + i]) {
					arr[0] = x; arr[1] = y + i;
					return arr;
				}
			}
			if (isInPic(x, y - i, width, height)) {
				if (isFill[x][y - i]) {
					arr[0] = x; arr[1] = y - i;
					return arr;
				}
			}
		}
		return arr;

	}
}