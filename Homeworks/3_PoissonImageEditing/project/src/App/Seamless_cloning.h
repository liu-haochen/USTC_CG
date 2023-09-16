#pragma once

#include "Image_Editing.h"
#include <Eigen/Dense>
#include <Eigen/Core>
#include <vector>
#include <Eigen/SparseCholesky>
//#include <Eigen/CholmodSupport>
//#include <Eigen/SparseLU>
#include <Eigen/IterativeLinearSolvers>

//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//#include<opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
namespace Image_Processing {
	class Seamless_cloning : public Image_Editing
	{
	public:
		Seamless_cloning();
		void Edit();
	private:
		void Pre_Sparse();
		void calculate_partial();
		void Possion_Edit();

		std::vector<Eigen::MatrixXd> qImageToEigenVector(const QImage& image);
		void limitValues(std::vector<Eigen::MatrixXd>& matrices);
		bool isIndexIn(int x, int size);
		Eigen::MatrixXd cal_grad(const Eigen::MatrixXd&);


		std::vector<Eigen::MatrixXd> m_v_image_Mat;//求解得到的矩阵
		std::vector<Eigen::MatrixXd> m_v_partial;//矩阵的右端
		std::vector<Eigen::MatrixXd> m_v_partial2;//矩阵的右端
		Eigen::SparseMatrix<double> m_Sparse_A;//稀疏矩阵

		std::vector<Eigen::MatrixXd> m_EMat_image_GBR;
		std::vector<Eigen::MatrixXd> m_EMat_image_bg_GBR;//分离通道


		Eigen::SimplicialLLT <Eigen::SparseMatrix<double>>* m_solver;//求解器

		QImage* m_image_backup;

		std::vector<Eigen::MatrixXd> m_grad;
		Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> m_res;
		bool m_is_Pre_Sparse;
	};
}