#pragma once

#include "Image_Editing.h"
#include <Eigen/Dense>
#include <Eigen/Core>
#include <vector>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include <Eigen/IterativeLinearSolvers>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
namespace Image_Processing {
	class Possion_Image_Editing : public Image_Editing 
	{
	public:
		Possion_Image_Editing();
		void Edit();
	private:
		void Pre_Sparse();
		void calculate_partial();
		
		cv::Mat Mergin(std::vector<Eigen::MatrixXd>&);

		void Possion_Edit();
		
		std::vector<Eigen::MatrixXd> convertToEigen(cv::Mat &image);

		std::vector<Eigen::MatrixXd> m_v_image_Mat;//求解得到的矩阵
		std::vector<Eigen::MatrixXd> m_v_partial;//矩阵的右端
		Eigen::SimplicialLLT <Eigen::SparseMatrix<double>>  m_solver;//求解器
		QImage *m_image_backup;
		bool m_is_Pre_Sparse;
	};
}