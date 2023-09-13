#pragma once
#include "Warping.h"

namespace Image_Processing {
	class RBF_Warping :public Warping {
	public:
		double g(double x, double y, double X, double Y, double sigma = 10);
		void Warp();
	};

}