#include "Seamless_cloning.h"
//#include <omp.h>



#include<iostream>
namespace Image_Processing {
	std::vector<Eigen::MatrixXd> Seamless_cloning::qImageToEigenVector(const QImage& image)
	{

		if (image.isNull())
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
	void computeGradient(const Eigen::MatrixXd& matrix, Eigen::MatrixXd& gradientX, Eigen::MatrixXd& gradientY)
	{
		int numRows = matrix.rows();
		int numCols = matrix.cols();
		gradientX.resize(numRows, numCols);
		gradientY.resize(numRows, numCols);
		gradientX.setZero();
		gradientY.setZero();
		gradientX.block(0, 1, numRows, numCols - 2) = (matrix.block(0, 2, numRows, numCols - 2) + matrix.block(0, 0, numRows, numCols - 2) - 2 * matrix.block(0, 1, numRows, numCols - 2));
		gradientY.block(1, 0, numRows - 2, numCols) = (matrix.block(2, 0, numRows - 2, numCols) + matrix.block(0, 0, numRows - 2, numCols) - 2 * matrix.block(1, 0, numRows - 2, numCols));

	}

	Eigen::MatrixXd computeDivergence(const Eigen::MatrixXd& gradientX, const Eigen::MatrixXd& gradientY)
	{
		int numRows = gradientX.rows();
		int numCols = gradientX.cols();
		Eigen::MatrixXd divergence(numRows, numCols);
		divergence.setZero();
		divergence = gradientX + gradientY;
		return divergence;
	}
	void computeGradientLRUD(const Eigen::MatrixXd& matrix, Eigen::MatrixXd& gradientL, Eigen::MatrixXd& gradientR, Eigen::MatrixXd& gradientU, Eigen::MatrixXd& gradientD)
	{
		int numRows = matrix.rows();
		int numCols = matrix.cols();
		gradientL.resize(numRows, numCols);
		gradientR.resize(numRows, numCols);
		gradientU.resize(numRows, numCols);
		gradientD.resize(numRows, numCols);
		gradientL.setZero();
		gradientR.setZero();
		gradientU.setZero();
		gradientD.setZero();
		//计算上下左右的梯度
		gradientL.block(0, 1, numRows, numCols - 1) = matrix.block(0, 0, numRows, numCols - 1)- matrix.block(0, 1, numRows, numCols - 1);
		gradientR.block(0, 0, numRows, numCols - 1) = matrix.block(0, 1, numRows, numCols - 1) - matrix.block(0, 0, numRows, numCols - 1);
		gradientU.block(1,0, numRows-1, numCols)= matrix.block(0, 0, numRows-1, numCols ) - matrix.block(1, 0, numRows-1, numCols);
		gradientD.block(0, 0, numRows - 1, numCols) = matrix.block(1, 0, numRows - 1, numCols) - matrix.block(0, 0, numRows - 1, numCols);
		
	}

	Eigen::MatrixXd computeDivergenceLRUD(Eigen::MatrixXd& gradientL,Eigen::MatrixXd& gradientR, Eigen::MatrixXd& gradientU, Eigen::MatrixXd& gradientD)
	{
		int numRows = gradientL.rows();
		int numCols = gradientL.cols();
		Eigen::MatrixXd divergence(numRows, numCols);
		divergence.setZero();
		divergence = gradientL + gradientR + gradientU + gradientD;
		return divergence;
	}


	Seamless_cloning::Seamless_cloning()
	{
		m_image_backup = nullptr;
		m_is_Pre_Sparse = false;
		m_solver = nullptr;
	}

	void Seamless_cloning::limitValues(std::vector<Eigen::MatrixXd>& matrices)
	{
		for (auto& matrix : matrices)
		{
			matrix = matrix.cwiseMin(255.0).cwiseMax(0.0);
			//auto Max = matrix.maxCoeff() > 255 ? matrix.maxCoeff() : 255;
			//auto Min = matrix.minCoeff() < 0 ? matrix.minCoeff() : 0;
			//matrix = (matrix.array() - Min)/(Max-Min)*255 ;
			//std::cout <<"Matrix:"<<std::endl << matrix << std::endl;
		}
	}

	void Seamless_cloning::Edit()
	{
		if (m_bg_image == nullptr || m_image == nullptr) {
			return;
		}
		m_EMat_image_bg_GBR = qImageToEigenVector(*m_bg_image);
		if (m_image != m_image_backup) {
			m_v_image_Mat.clear();
			m_v_partial.clear();
			m_v_partial2.clear();
			m_grad.clear();

			

			if (m_solver != nullptr) {
				delete m_solver;
			}
			m_solver = new Eigen::SimplicialLLT <Eigen::SparseMatrix<double>>;

			m_EMat_image_GBR = qImageToEigenVector(*m_image);
			for (int k = 0; k < 3; k++) {
				m_grad.push_back(cal_grad(m_EMat_image_GBR[k]));
			}
			Pre_Sparse();
			m_image_backup = m_image;
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
		std::cout << m_v_image_Mat[0](0, 0) << "=" << m_EMat_image_bg_GBR[0](0, 0);
		std::cout << "Seamless cloning\n";

	}

	bool Seamless_cloning::isIndexIn(int x, int size) {
		return x >= 0 && x < size;
	}

	Eigen::MatrixXd Seamless_cloning::cal_grad(const Eigen::MatrixXd& matrix) {
		Eigen::MatrixXd gradientX(matrix.rows(), matrix.cols());
		gradientX.topRows(matrix.rows() - 1) = matrix.block(1, 0, matrix.rows() - 1, matrix.cols()) - matrix.block(0, 0, matrix.rows() - 1, matrix.cols());
		gradientX.bottomRows(1).setZero();

		// 计算列梯度
		Eigen::MatrixXd gradientY(matrix.rows(), matrix.cols());
		gradientY.leftCols(matrix.cols() - 1) = matrix.block(0, 1, matrix.rows(), matrix.cols() - 1) - matrix.block(0, 0, matrix.rows(), matrix.cols() - 1);
		gradientY.rightCols(1).setZero();

		// 计算梯度模长
		Eigen::MatrixXd gradientMagnitude = (gradientX.array().square() + gradientY.array().square()).sqrt();

		return gradientMagnitude;
	}

	void Seamless_cloning::Pre_Sparse()
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
		//计算系数矩阵和部分b;
		for (int i = 1; i < h - 1; ++i) {
			for (int j = 1; j < w - 1; ++j) {
				int idx = (i - 1) * (w - 2) + (j - 1); // 内部网格点在矩阵中的索引
				if (isIndexIn(idx, size)) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx, 4.0));
				}
				if (j > 1 ) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx - 1, -1.0));
				}

				if (j<w-2 ) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx + 1, -1.0));
				}

				if (i>1) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx - (w - 2), -1.0));
				}

				if (i<h-2) {
					coefficients.push_back(Eigen::Triplet<double>(idx, idx + (w - 2), -1.0));
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
	void Seamless_cloning::calculate_partial()
	{


		m_v_partial.clear();

		auto w = m_image->width();
		auto h = m_image->height();

		auto size = (w - 2) * (h - 2);

		Eigen::MatrixXd TV(size, 1);
		TV.setZero();
		m_v_partial.push_back(TV);
		Eigen::MatrixXd TV2(size, 1);
		TV2.setZero();
		m_v_partial.push_back(TV2);
		Eigen::MatrixXd TV3(size, 1);
		TV3.setZero();
		m_v_partial.push_back(TV3);

		Eigen::MatrixXd TV1(size, 1);
		TV1.setZero();
		m_v_partial2.push_back(TV1);

		Eigen::MatrixXd TV4(size, 1);
		TV4.setZero();
		m_v_partial2.push_back(TV4);

		Eigen::MatrixXd TV5(size, 1);
		TV5.setZero();
		m_v_partial2.push_back(TV5);
		auto& v_partial = m_v_partial;
		auto& v_partial2 = m_v_partial2;


		//计算RBG的梯度gX,gY以及背景图的gX,gY
		std::vector<Eigen::MatrixXd> grid_X(3);
		std::vector<Eigen::MatrixXd> grid_Y(3);
		std::vector<Eigen::MatrixXd> grid_bg_X(3);
		std::vector<Eigen::MatrixXd> grid_bg_Y(3);
		for (int k = 0; k < 3; k++) {
			computeGradient(m_EMat_image_GBR[k], grid_X[k], grid_Y[k]);
			computeGradient(m_EMat_image_bg_GBR[k], grid_bg_X[k], grid_bg_Y[k]);
		}
		std::vector<Eigen::MatrixXd> grid_res_X;
		std::vector<Eigen::MatrixXd> grid_res_Y;
		grid_res_X = grid_X;
		grid_res_Y = grid_Y;

		//比较|gX|+|gY|的大小并将大者赋予到gRes
	/*	Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> result =
			(grid_X[0].array().square() + grid_Y[0].array().square()+
				grid_X[1].array().square() + grid_Y[1].array().square()+
				grid_X[2].array().square() + grid_Y[2].array().square()) <=
			(grid_bg_X[0].array().square() + grid_bg_Y[0].array().square()+
				grid_bg_X[1].array().square() + grid_bg_Y[1].array().square()+
				grid_bg_X[2].array().square() + grid_bg_Y[2].array().square());*/


		for (int k = 0; k < 3; k++) {
			/*Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> result=
			(grid_X[k].array().square() + grid_Y[k].array().square()) <= 
			(grid_bg_X[k].array().square() + grid_bg_Y[k].array().square());*/

			Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> resultX =
				(grid_X[k].array().square()) <=
				(grid_bg_X[k].array().square() );

			Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> resultY =
				(grid_Y[k].array().square()) <=
				(grid_bg_Y[k].array().square());


			//m_res = result;

			for (int i = 1; i < h - 1; i++) {
				for (int j = 1; j < w - 1; j++) {
					/*if (result(i,j)) {
						grid_res_X[k](i, j) = grid_bg_X[k](i, j);
						grid_res_Y[k](i, j) = grid_bg_Y[k](i, j);
					}*/
					if(resultX(i,j))
					{
						grid_res_X[k](i, j) = grid_bg_X[k](i, j);
					}
					if (resultY(i, j))
					{
						grid_res_Y[k](i, j) = grid_bg_Y[k](i, j);
					}
				}
			}
		
		}
		/*grid_res_X = grid_X;
		grid_res_Y = grid_Y;*/

		//计算grid_res_X,grid_res_Y得到的梯度
		std::vector<Eigen::MatrixXd> Divergence;
		for (int k = 0; k < 3; k++) {
			Divergence.push_back(computeDivergence(grid_res_X[k], grid_res_Y[k]));
		}
		//limitValues(Divergence);

		std::vector<Eigen::MatrixXd> grid_L(3);
		std::vector<Eigen::MatrixXd> grid_R(3);
		std::vector<Eigen::MatrixXd> grid_U(3);
		std::vector<Eigen::MatrixXd> grid_D(3);
		std::vector<Eigen::MatrixXd> grid_bg_L(3);
		std::vector<Eigen::MatrixXd> grid_bg_R(3);
		std::vector<Eigen::MatrixXd> grid_bg_U(3);
		std::vector<Eigen::MatrixXd> grid_bg_D(3);

		for (int k = 0; k < 3; k++) {
			computeGradientLRUD(m_EMat_image_GBR[k], grid_L[k], grid_R[k], grid_U[k], grid_D[k]);
			computeGradientLRUD(m_EMat_image_bg_GBR[k], grid_bg_L[k], grid_bg_R[k], grid_bg_U[k], grid_bg_D[k]);
		}


		std::vector<Eigen::MatrixXd> grid_res_L;
		std::vector<Eigen::MatrixXd> grid_res_R;
		std::vector<Eigen::MatrixXd> grid_res_U;
		std::vector<Eigen::MatrixXd> grid_res_D;
		grid_res_L = grid_L;
		grid_res_R = grid_R; 
		grid_res_U = grid_U;
		grid_res_D = grid_D;


		for (int k = 0; k < 3; k++) {
			/*Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> result=
			(grid_X[k].array().square() + grid_Y[k].array().square()) <=
			(grid_bg_X[k].array().square() + grid_bg_Y[k].array().square());*/
			//std::cout << grid_L[k]<<std::endl;

			Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> resultL =
				(grid_L[k].array().square()) <=
				(grid_bg_L[k].array().square());

			Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> resultR =
				(grid_R[k].array().square()) <=
				(grid_bg_R[k].array().square());

			Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> resultU =
				(grid_U[k].array().square()) <=
				(grid_bg_U[k].array().square());

			Eigen::Matrix<bool, Eigen::Dynamic, Eigen::Dynamic> resultD =
				(grid_D[k].array().square()) <=
				(grid_bg_D[k].array().square());

			//m_res = result;

			for (int i = 1; i < h - 1; i++) {
				for (int j = 1; j < w - 1; j++) {
					/*if (result(i,j)) {
						grid_res_X[k](i, j) = grid_bg_X[k](i, j);
						grid_res_Y[k](i, j) = grid_bg_Y[k](i, j);
					}*/
					if (resultL(i, j))
					{
						grid_res_L[k](i, j) = grid_bg_L[k](i, j);
					}
					if (resultR(i, j))
					{
						grid_res_R[k](i, j) = grid_bg_R[k](i, j);
					}
					if (resultU(i, j))
					{
						grid_res_U[k](i, j) = grid_bg_U[k](i, j);
					}
					if (resultD(i, j))
					{
						grid_res_D[k](i, j) = grid_bg_D[k](i, j);
					}
				}
			}
			//std::vector<Eigen::MatrixXd> Divergence;
			/*grid_res_L = grid_L;
			grid_res_R = grid_R;
			grid_res_U = grid_U;
			grid_res_D = grid_D;*/
			Divergence.clear();
			for (int k = 0; k < 3; k++) {
				Divergence.push_back(computeDivergenceLRUD(grid_res_L[k], grid_res_R[k], grid_res_U[k], grid_res_D[k]));
			}

		}










		for (int i = 1; i < h - 1; ++i) {
			for (int j = 1; j < w - 1; ++j) {
				int idx = (i - 1) * (w - 2) + (j - 1); // 内部网格点在矩阵中的索引
				if (! (j>1) ) {
					for (int k = 0; k < 3; k++) {
						v_partial[k](idx, 0) += m_EMat_image_bg_GBR[k](i, j - 1);
					}
				}
				if (!(j<w-2) ) {
					for (int k = 0; k < 3; k++) {
						v_partial[k](idx, 0) += m_EMat_image_bg_GBR[k](i, j + 1);
					}
				}
				if (!(i>1) ) {
					for (int k = 0; k < 3; k++) {
						v_partial[k](idx, 0) += m_EMat_image_bg_GBR[k](i - 1, j);
					}
				}

				if (!(i<h-2) ) {
					for (int k = 0; k < 3; k++) {
						v_partial[k](idx, 0) += m_EMat_image_bg_GBR[k](i + 1, j);
					}
				}
				for (int k = 0; k < 3; k++) {
					v_partial2[k](idx, 0) = Divergence[k](i,j);
				}
			}
		}



	}

	void Seamless_cloning::Possion_Edit()
	{
		auto w = m_image->width();
		auto h = m_image->height();
		m_v_image_Mat.clear();
		for (int GBR = 0; GBR < 3; GBR++) {
			std::cout << "solving!" << std::endl;
			Eigen::MatrixXd v = m_solver->solve(m_v_partial[GBR] - m_v_partial2[GBR]);
			std::cout << "solved" << std::endl;

			if (m_solver->info() != Eigen::Success) {
				std::cout << "solve error";
			}

			Eigen::MatrixXd MTemp = m_EMat_image_bg_GBR[GBR];
			std::cout << "here" << std::endl;
			for (int i = 1; i < h - 1; i++) {
				for (int j = 1; j < w - 1; j++) {
					//if (!m_res(i,j)) {
						MTemp(i, j) = v((i - 1) * (w - 2) + j - 1);
					//}
					
				}
			}
			std::cout << "for out" << std::endl;
			/*std::cout << "MTemp:"<<std::endl << MTemp << std::endl;
			std::cout << "image"<<std::endl<<m_EMat_image_GBR[GBR]<<std::endl;
			std::cout << "bg:" << std::endl << m_EMat_image_bg_GBR[GBR] << std::endl;*/
			m_v_image_Mat.push_back(MTemp);
		}

	}

}