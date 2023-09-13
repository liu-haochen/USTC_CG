#include "IDW_Warping.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <Eigen\Dense>
#include<array>
namespace Image_Processing {


	

	void IDW_Warping::Warp() {
		QImage* ptr_image = new QImage;
		*ptr_image = *m_ptr_image;
		ptr_image->fill(qRgba(255, 255, 255,0));
		auto len = m_Vpair.size();
		const auto width = ptr_image->width();
		const auto height = ptr_image->height();
		Eigen::MatrixXd MSp(len, 2);
		Eigen::MatrixXd MEp(len, 2);
		Eigen::MatrixXd p(len, 2);
		Eigen::MatrixXd w(1, len);
		Eigen::MatrixXd wTemp(1, len);
		Eigen::MatrixXd res(1, 2); 
		std::vector<std::vector<bool>>isFill (width,std::vector<bool>(height));
		std::vector<std::vector<std::array<double,2>>> Transform(width, std::vector<std::array<double, 2>>(height) );

		QPoint QP;
		for (int i = 0; i < len; i++) {
			MSp(i, 0) = m_Vpair[i].Spoint.rx();
			MSp(i, 1) = m_Vpair[i].Spoint.ry();
			MEp(i, 0) = m_Vpair[i].Epoint.rx();
			MEp(i, 1) = m_Vpair[i].Epoint.ry();
		}

		for (double x = 0; x < width; x++) {
			for (double y = 0; y < height; y++) {
				p.col(0) = Eigen::VectorXd::Constant(p.rows(), x);
				p.col(1) = Eigen::VectorXd::Constant(p.rows(), y);
				for (int i = 0; i < len; i++) {
					wTemp(0, i) = Factor_(pow( (p.row(i) - MSp.row(i)).norm(),m_mu));
				}
				w = wTemp / wTemp.sum();
				res = w* (MEp + p - MSp);
				QP.setX(x);
				QP.setY(y);
				if (isInPic(res(0, 0), res(0, 1), width, height)) {
					Transform[res(0, 0)][res(0, 1)][0] = res(0, 0) - x;
					Transform[res(0, 0)][res(0, 1)][1] = res(0, 1) - y;
					isFill[res(0, 0)][res(0, 1)] = true;
					ptr_image->setPixel(res(0,0),res(0,1), m_ptr_image->pixel(QP));
				}
				//std::cout <<"p" << p << std::endl;
				//std::cout <<"MSp" << MSp << std::endl;
				//std::cout <<"MEp" << MEp << std::endl;
				//std::cout << "res" << res << std::endl;
				//std::cout << "w" << w << std::endl;
			}
		}
		//将未填充部分搜索附近点并填充
		std::array<double, 2>arrTemp;
		double Tempx, Tempy;
		for (double x = 0; x < width; x++) {
			for (double y = 0; y < height; y++) {
				if (!isFill[x][y]) {
					arrTemp = Find(x, y, isFill, width, height);
					if (isInPic (arrTemp[0],arrTemp[1],width,height) ) {
						Tempx = x - Transform[arrTemp[0]][arrTemp[1]][0];
						Tempy = y - Transform[arrTemp[0]][arrTemp[1]][1];
						if (isInPic(Tempx, Tempy, width, height)) {
							QP.setX(Tempx);
							QP.setY(Tempy);
							ptr_image->setPixel(x,y, ptr_image->pixel(arrTemp[0],arrTemp[1]));
							//ptr_image->setPixel(x, y, m_ptr_image->pixel(Tempx,Tempy));
						}
					}
				}
			}
		}

		*m_ptr_image = *ptr_image;
	}
}