#include "Possion_Image_Editing.h"
//#include <omp.h>



#include<iostream>
namespace Image_Processing {
	std::vector<Eigen::MatrixXd> Possion_Image_Editing::qImageToEigenVector(const QImage& image)
	{

		if (image.isNull() )
		{
			std::cout << "无效" << std::endl;
			return std::vector<Eigen::MatrixXd>();
		}

		int width = image.width();
		int height = image.height();

		std::vector<Eigen::MatrixXd> matrices(3);

		for (int channel = 0; channel < 3; ++channel)
		{
			Eigen::MatrixXd matrix(height, width);

			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					QRgb pixel = image.pixel(x, y);
					int value = qRed(pixel);
					if (channel == 1)  // 绿色通道
						value = qGreen(pixel);
					else if (channel == 2)  // 蓝色通道
						value = qBlue(pixel);
					matrix(y, x) = static_cast<double>(value);
				}
			}

			matrices[channel] = matrix;
		}

		return matrices;
	}



	Possion_Image_Editing::Possion_Image_Editing() 
	{
		m_image_backup = nullptr;
		m_is_Pre_Sparse = false;
		m_solver = nullptr;
	}

	void Possion_Image_Editing:: limitValues(std::vector<Eigen::MatrixXd>& matrices)
	{
		for (auto& matrix : matrices)
		{
			matrix = matrix.cwiseMin(255.0).cwiseMax(0.0);
		}
	}

	void Possion_Image_Editing::Edit()
	{
		if (m_bg_image == nullptr || m_image == nullptr) {
			return;
		}
		m_EMat_image_bg_GBR = qImageToEigenVector(*m_bg_image);
		if (m_image!=m_image_backup) {
			m_v_image_Mat.clear();
			m_v_partial.clear();
			m_v_partial2.clear();

			if (m_solver != nullptr) {
				delete m_solver;
			}
			m_solver = new Eigen::SimplicialLDLT <Eigen::SparseMatrix<double>>;

			m_EMat_image_GBR = qImageToEigenVector(*m_image);
			Pre_Sparse();
			m_image_backup=m_image;
			std::cout << "Pre_Sparse" << std::endl;
		}
		
		calculate_partial();

		std::cout << "partial" << std::endl;
		
		Possion_Edit();

		std::cout << "Possion_Edit" << std::endl;

		auto w = m_image->width();
		auto h = m_image->height();


		limitValues(m_v_image_Mat);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				m_Edited_image->setPixelColor(j, i, QColor(m_v_image_Mat[0](i, j), m_v_image_Mat[1](i, j), m_v_image_Mat[2](i, j)));
			}
		}

		std::cout << "Possion create\n";

	}

	bool Possion_Image_Editing:: isIndexIn(int x,int size) {
		return x >= 0 && x < size;
	}

	void Possion_Image_Editing::Pre_Sparse()
	{

		auto w = m_image->width();
		auto h = m_image->height();
		int i_row = -1;
		auto size = (w - 2) * (h - 2);

		Eigen::SparseMatrix<double> A(size, size);

		std::vector<Eigen::Triplet<double>> coefficients;
		coefficients.reserve(5 * size); // 每个内部网格点最多有5个非零系数
		Eigen::MatrixXd TV(size, 1);
		TV.setZero();
		m_v_partial2.push_back(TV);
		m_v_partial2.push_back(TV);
		m_v_partial2.push_back(TV);
		auto& v_partial2 = m_v_partial2;
		//计算系数矩阵和部分b;
		for (int i = 1; i < h - 1; ++i) {
			for (int j = 1; j < w - 1; ++j) {
				int idx = (i - 1) * (w - 2) + (j - 1); // 内部网格点在矩阵中的索引
				if (isIndexIn(idx, size)) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx, 4.0));
				}
				if (j>1 ) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx - 1, -1.0));
				}
				
				if (j<w-2) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx + 1, -1.0));
				}
				
				if (i>1) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx - (w - 2), -1.0));
				}
				
				if (i<h-2) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx + (w - 2), -1.0));
				}
				
				for (int k = 0; k < 3; k++) {
					v_partial2[k](idx, 0) = -4 * m_EMat_image_GBR[k](i, j)
						+ m_EMat_image_GBR[k](i - 1, j)
						+ m_EMat_image_GBR[k](i + 1, j)
						+ m_EMat_image_GBR[k](i, j - 1)
						+ m_EMat_image_GBR[k](i, j + 1);
				}
			}
		}
		
		A.setFromTriplets(coefficients.begin(), coefficients.end());
		m_Sparse_A = A;
		A.makeCompressed();
		//std::cout << A << std::endl;
		m_solver->compute(A);
		if (m_solver->info() != Eigen::Success)
		{
			std::cerr << "Cholesky分解失败" << std::endl;
		}


	}
	void Possion_Image_Editing::calculate_partial()
	{
		

		m_v_partial.clear();

		auto w = m_image->width();
		auto h = m_image->height();

		auto size = (w-2)*(h-2);

		Eigen::MatrixXd TV(size, 1);
		TV.setZero();
		m_v_partial.push_back(TV);
		m_v_partial.push_back(TV);
		m_v_partial.push_back(TV);
		auto& v_partial = m_v_partial;
		for (int i = 1; i < h - 1; ++i) {
			for (int j = 1; j < w - 1; ++j) {
				int idx = (i - 1) * (w - 2) + (j - 1); // 内部网格点在矩阵中的索引
				if (! (j>1) ) {
					for (int k = 0; k < 3; k++) {
						v_partial[k](idx, 0) += m_EMat_image_bg_GBR[k](i, j - 1);
					}
				}
				if (!(j<w-2)) {
					for (int k = 0; k < 3; k++) {
						v_partial[k](idx, 0) += m_EMat_image_bg_GBR[k](i, j + 1);
					}
				}
				if (!(i>1)) {
					for (int k = 0; k < 3; k++) {
						v_partial[k](idx, 0) += m_EMat_image_bg_GBR[k](i - 1, j);
					}
				}
				
				if (!(i<h-2)) {
					for (int k = 0; k < 3; k++) {
						v_partial[k](idx, 0) += m_EMat_image_bg_GBR[k](i + 1, j);
					}
				}
			}
		}
		
		

	}

	void Possion_Image_Editing::Possion_Edit()
	{
		auto w = m_image->width();
		auto h = m_image->height();
		m_v_image_Mat.clear();
		for (int GBR = 0; GBR < 3; GBR++) {
			std::cout << "solving!" << std::endl;
			Eigen::MatrixXd v=m_solver->solve(m_v_partial[GBR]-m_v_partial2[GBR]);
			std::cout << "solved" << std::endl;


			Eigen::MatrixXd MTemp = m_EMat_image_bg_GBR[GBR];
			std::cout << "here" << std::endl;
			for (int i = 1; i < h-1; i++) {
				for (int j = 1; j < w-1; j++) {
					MTemp(i, j) = v((i-1)*(w-2)+j-1);
				}
			}
			std::cout << "for out"<<std::endl;
			m_v_image_Mat.push_back(MTemp);
		}
		
	}

}