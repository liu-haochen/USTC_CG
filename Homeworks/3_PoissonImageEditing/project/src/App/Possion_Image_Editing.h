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
	class Possion_Image_Editing : public Image_Editing 
	{
	public:
		Possion_Image_Editing();
		void Edit();
	private:
		void Pre_Sparse();
		void calculate_partial();
		void Possion_Edit();

		std::vector<Eigen::MatrixXd> qImageToEigenVector(const QImage& image);
		void limitValues(std::vector<Eigen::MatrixXd>& matrices);
		bool isIndexIn(int x, int size);


		std::vector<Eigen::MatrixXd> m_v_image_Mat;//���õ��ľ���
		std::vector<Eigen::MatrixXd> m_v_partial;//������Ҷ�
		std::vector<Eigen::MatrixXd> m_v_partial2;//������Ҷ�		
		std::vector<Eigen::MatrixXd> m_EMat_image_GBR;
		std::vector<Eigen::MatrixXd> m_EMat_image_bg_GBR;//����ͨ��

		Eigen::SparseMatrix<double> m_Sparse_A;//ϡ�����

		Eigen::SimplicialLDLT <Eigen::SparseMatrix<double>> * m_solver;//�����

		QImage *m_image_backup;

		bool m_is_Pre_Sparse;
	};
}