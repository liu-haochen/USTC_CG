#include "Possion_Image_Editing.h"



#include<iostream>
namespace Image_Processing {
	std::vector<Eigen::MatrixXd> qImageToEigenVector(const QImage& image)
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
	}

	void limitValues(std::vector<Eigen::MatrixXd>& matrices)
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
		if (!m_is_Pre_Sparse||m_image!=m_image_backup) {
			Pre_Sparse();
			m_image_backup=m_image;
		}
		std::cout << "Pre_Sparse"<<std::endl;
		calculate_partial();

		std::cout << "partial" << std::endl;
		
		Possion_Edit();

		auto res_mat=Mergin(m_v_image_Mat);
		cv::imshow("im", res_mat);
		/*for (auto &Mm: m_v_image_Mat)
		{
			std::cout << Mm<<std::endl;
		}*/
		/*for (auto& Mm :m_v_partial)
		{
			std::cout << Mm << std::endl;
		}*/

		auto w = m_image->width();
		auto h = m_image->height();
		std::cout << "w::"<<w <<"h::"<<h<<std::endl;

		limitValues(m_v_image_Mat);
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				m_Edited_image->setPixelColor(j, i, QColor(m_v_image_Mat[0](i, j), m_v_image_Mat[1](i, j), m_v_image_Mat[2](i, j)));
			}
		}

		std::cout << "Possion create\n";

	}

	void Possion_Image_Editing::Pre_Sparse()
	{
		auto w = m_image->width();
		auto h = m_image->height();
		int i_row = -1;
		Eigen::SparseMatrix<double> M(w*h,w*h);
		for (int j = 0; j < w; j++) {
			i_row++;
			M.insert(i_row, j) = 1;
			i_row++;
			M.insert(i_row, w * (h - 1) + j) = 1;
		}

		//std::cout << i_row << std::endl;
		for (int i = 1; i < h - 1; i++) {
			i_row++;
			M.insert(i_row, w * i) = 1;
			i_row++;
			M.insert(i_row, w * i + w - 1) = 1;
		}

		//std::cout << w * h << std::endl;
		for (int i = 1; i < h - 1; i++) {
			for (int j = 1; j < w - 1; j++) {
				i_row ++;

				M.insert(i_row , w * i + j) = -4;
				M.insert(i_row, w * (i - 1) + j) = 1;
				M.insert(i_row, w * i + j - 1) = 1;
				M.insert(i_row, w * i + j + 1) = 1;
				M.insert(i_row, w * (i + 1) +j) = 1;
			}
		}
		M.finalize();
		//std::cout << i_row<< std::endl;


		//std::cout << i_row << std::endl;
		std::cout << M<<std::endl;

		M.makeCompressed();
		//std::cout << M << std::endl;
		//auto M_selfadjoint = M.selfadjointView<Eigen::Lower>();
		//std::cout << M_selfadjoint <<std::endl;
		//m_solver.compute(M_selfadjoint);
		//if (m_solver.info() != Eigen::Success)
		//{
		//	std::cerr << "Cholesky分解失败" << std::endl;
		//}
		/*m_solver.analyzePattern(M);
		m_solver.factorize(M);*/
		m_solver.compute(M);
		if (m_solver.info() != Eigen::Success)
		{
			std::cerr << "Cholesky分解失败" << std::endl;
		}
	}
	void Possion_Image_Editing::calculate_partial()
	{
		QImage TempImage;
		TempImage = *m_image;
		TempImage.convertTo(QImage::Format_BGR888);
		cv::Mat cv_image(TempImage.height(), TempImage.width(), CV_8UC3, TempImage.bits(), TempImage.bytesPerLine());

		TempImage = *m_bg_image;
		TempImage.convertTo(QImage::Format_BGR888);
		cv::Mat cv_bg_image(TempImage.height(), TempImage.width(), CV_8UC3, TempImage.bits(), TempImage.bytesPerLine());
		
		auto EMat_image_GBR = qImageToEigenVector(*m_image);
		auto EMat_image_bg_GBR = qImageToEigenVector(*m_bg_image);
		/*for (auto& M : EMat_image_GBR)
		{
			std::cout << M << std::endl;
		}*/

		m_v_partial.clear();

		auto w = m_image->width();
		auto h = m_image->height();

		for (int GBR = 0; GBR < 3; GBR++)
		{
			int i_row = -1;
			Eigen::MatrixXd v_b(w * h, 1);
			v_b.setZero();
			const auto& M = EMat_image_GBR[GBR];
			const auto& M_bg = EMat_image_bg_GBR[GBR];

			for (int j = 0; j < w; j++) {
				i_row++;
				v_b(i_row, 0) = M_bg(0, j);
				i_row++;
				v_b(i_row, 0) = M_bg(h - 1, j);

			}
			for (int i = 1; i < h - 1; i++) {
				i_row++;
				v_b(i_row, 0) = M_bg(i, 0);
				i_row++;
				v_b(i_row, 0) = M_bg(i, w - 1);
			}

			for (int i = 1; i < h - 1; i++) {
				for (int j = 1; j < w - 1; j++) {
					i_row++;
					v_b(i_row, 0) = -4*M(i,j)+M(i-1,j)+M(i+1,j)+M(i,j-1)+M(i,j+1);

				}
			}
			
			m_v_partial.push_back(v_b);
		}


	}

	void Possion_Image_Editing::Possion_Edit()
	{
		auto w = m_image->width();
		auto h = m_image->height();
		m_v_image_Mat.clear();
		for (int GBR = 0; GBR < 3; GBR++) {
			auto v=m_solver.solve(m_v_partial[GBR]) ;
			Eigen::MatrixXd MTemp(h,w);
			for (int i = 0; i < h; i++) {
				for (int j = 0; j < w; j++) {
					MTemp(i, j) = v(i*w+j,0);
				}
			}
			m_v_image_Mat.push_back(MTemp);
		}
		
	}

	std::vector<Eigen::MatrixXd> Possion_Image_Editing::convertToEigen(cv::Mat &image)
	{
		std::vector<cv::Mat> channels;
		cv::split(image, channels);

		std::vector<Eigen::MatrixXd> eigenChannels(channels.size());
		for (size_t i = 0; i < channels.size(); i++) {
			cv::Mat channel = channels[i];

			// 转换为Eigen库中的MatrixXd类型
			Eigen::MatrixXd eigenChannel(channel.rows, channel.cols);
			for (int row = 0; row < channel.rows; ++row) {
				for (int col = 0; col < channel.cols; ++col) {
					eigenChannel(row, col) = static_cast<double>(channel.at<uchar>(row, col));
				}
			}

			eigenChannels[i] = eigenChannel;
		}
		
		return eigenChannels;
	}

	cv::Mat Possion_Image_Editing::Mergin(std::vector<Eigen::MatrixXd> &eigenChannels)
	{
		size_t numChannels = eigenChannels.size();
		if (numChannels == 0) {
			return cv::Mat();
		}

		int numRows = eigenChannels[0].rows();
		int numCols = eigenChannels[0].cols();

		// 创建一个具有相应大小和类型的空白cv::Mat图像
		cv::Mat result(numRows, numCols, CV_8UC3);

		std::vector<cv::Mat> channels(numChannels);
		for (size_t i = 0; i < numChannels; ++i) {
			const Eigen::MatrixXd& eigenChannel = eigenChannels[i];

			// 创建一个临时的cv::Mat通道
			cv::Mat channel(numRows, numCols, CV_8UC1);

			// 复制Eigen矩阵数据到cv::Mat通道中
			for (int row = 0; row < numRows; ++row) {
				for (int col = 0; col < numCols; ++col) {
					channel.at<uchar>(row, col) = static_cast<uchar>(eigenChannel(row, col));
				}
			}

			channels[i] = channel;
		}

		// 将通道合并到最终结果的cv::Mat中
		cv::merge(channels, result);

		return result;


	}
}