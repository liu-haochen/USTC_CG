#include "RBF_Warping.h"
#include "Eigen/Dense"
#include <cmath>
#include<iostream>
#include <array>

namespace Image_Processing {

	double RBF_Warping::g(double x,double y,double X,double Y, double sigma) {
		return exp	( -(((x - X)* (x - X) + (y - Y) * (y - Y)) * Factor_(sigma * sigma)) );
		
	}
	double dis(Eigen::MatrixXd a, Eigen::MatrixXd b) {
		return (a - b).norm();
	}



	void RBF_Warping::Warp() {
		QImage* ptr_image = new QImage;
		*ptr_image = *m_ptr_image;
		ptr_image->fill(qRgba(255, 255, 255, 0));

		auto len = m_Vpair.size();
		const auto width = ptr_image->width();
		const auto height = ptr_image->height();
		if (len <= 0) {
			return;
		}

		Eigen::MatrixXd MSp(len, 2);
		Eigen::MatrixXd MEp(len, 2);
		
		Eigen::MatrixXd M_coef(2*len+6,2*len+6) ;
		Eigen::MatrixXd M_b(2*len+6,1);

		M_coef.setZero();
		M_b.setZero();
		
		for (int i = 0; i < len; i++) {
			MSp(i, 0) = m_Vpair[i].Spoint.rx();
			MSp(i, 1) = m_Vpair[i].Spoint.ry();
			MEp(i, 0) = m_Vpair[i].Epoint.rx();
			MEp(i, 1) = m_Vpair[i].Epoint.ry();
		}

		
		for (int i = 0; i < len; i++) {
			M_coef(2 * i, 0) = MSp(i, 0);
			M_coef(2 * i, 1) = MSp(i, 1);
			M_coef(2 * i, 4) = 1;

			M_coef(2 * i + 1, 2) = MSp(i, 0);
			M_coef(2 * i + 1, 3) = MSp(i, 1);
			M_coef(2 * i + 1, 5) = 1;
			for (int j = 0; j < len; j++) {
				M_coef(2 * i, 6 + j) = g(MSp(i, 0), MSp(i, 1), MSp(j, 0), MSp(j, 1),dis(MSp.row(j),MEp.row(j)));
				M_coef(2 * i + 1, 6 + len + j) = g(MSp(i, 0), MSp(i, 1), MSp(j, 0), MSp(j, 1), dis(MSp.row(j), MEp.row(j)));
			}

			M_b(2 * i, 0) = MEp(i, 0);
			M_b(2 * i+1, 0) = MEp(i, 1);
		}
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < len; j++) {
				M_coef(2 * (len + i), 6 + j) = MSp(j, i);
				M_coef(2 * (len + i) + 1, 6 + len + j) = MSp(j, i);
			}
		}
		for (int i = 0; i < len; i++) {
			M_coef(2 * (len + 2), i + 6) = 1;
			M_coef(2 * (len + 2) + 1, 6 + i + len) = 1;
		}
		
		for (int i = 0; i < 6; i++) {
			M_b(2 * len + i, 0) = 0;
		}
		std::cout << M_coef << std::endl;
		std::cout << M_b << std::endl;
		
		//解方程
		Eigen::VectorXd M_Solutoin=M_coef.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(M_b);
		std::cout << "solution" << M_Solutoin << std::endl;
		Eigen::MatrixXd M_m(2, 2);
		Eigen::MatrixXd M_c(2, 1);
		Eigen::MatrixXd M_ab(2, len);
		Eigen::MatrixXd M_g(len, 1);
		M_m.setZero(); M_c.setZero(); M_ab.setZero(); M_g.setZero();

		if (len == 1) {
			M_m(0, 0) = 1;
			M_m(1, 1) = 1;
			M_c(0, 0) = MEp(0, 0) - MSp(0, 0);
			M_c(1, 0) = MEp(0, 1) - MSp(0, 1);
		}
		else {
			M_m(0, 0) = M_Solutoin(0, 0);
			M_m(0, 1) = M_Solutoin(1, 0);
			M_m(1, 0) = M_Solutoin(2, 0);
			M_m(1, 1) = M_Solutoin(3, 0);
			M_c(0, 0) = M_Solutoin(4, 0);
			M_c(1, 0) = M_Solutoin(5, 0);
			for (int i = 0; i < len; i++) {
				M_ab(0, i) = M_Solutoin(6 + i, 0);
				M_ab(1, i) = M_Solutoin(6 + len + i, 0);
			}
		}
		Eigen::MatrixXd P(2, 1);
		Eigen::MatrixXd P_t(2, 1);
		P.setZero();
		M_g.setZero();

		//开始变换
		std::vector<std::vector<bool>>isFill(width, std::vector<bool>(height));
		std::vector<std::vector<std::array<double, 2>>> Transform(width, std::vector<std::array<double, 2>>(height));

		for (double x = 0; x < width; x++) {
			for (double y = 0; y < height; y++) {
				P(0, 0) = x; P(1, 0) = y;
				for (int i = 0; i < len; i++) {
					M_g(i, 0) = g(x, y, MSp(i, 0), MSp(i, 1),dis(MSp.row(i),MEp.row(i)));
				}
				P_t = M_m * P + M_c + M_ab * M_g;
				if (isInPic(P_t(0, 0), P_t(1, 0), width, height)) {
					isFill[P_t(0, 0)][P_t(1, 0)]=true;
					Transform[P_t(0, 0)][P_t(1, 0)][0] = P_t(0, 0) - x;
					Transform[P_t(0, 0)][P_t(1, 0)][1] = P_t(1, 0) - y;
					ptr_image->setPixel(P_t(0, 0), P_t(1, 0), m_ptr_image->pixel(x, y));
				}
			}
		}

		//填补白点
		QPoint QP;
		std::array<double, 2>arrTemp;
		double Tempx, Tempy;
		for (double x = 0; x < width; x++) {
			for (double y = 0; y < height; y++) {
				if (!isFill[x][y]) {
					arrTemp = Find(x, y, isFill, width, height);
					if (isInPic(arrTemp[0], arrTemp[1], width, height)) {
						Tempx = x - Transform[arrTemp[0]][arrTemp[1]][0];
						Tempy = y - Transform[arrTemp[0]][arrTemp[1]][1];
						if (isInPic(Tempx, Tempy, width, height)) {
							QP.setX(Tempx);
							QP.setY(Tempy);
							ptr_image->setPixel(x, y, ptr_image->pixel(arrTemp[0], arrTemp[1]));
							//ptr_image->setPixel(x, y, m_ptr_image->pixel(Tempx,Tempy));
						}
					}
				}
			}
		}
		*m_ptr_image = *ptr_image;
	}
}