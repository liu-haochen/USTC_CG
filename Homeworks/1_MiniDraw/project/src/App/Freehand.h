#pragma once

#include "shape.h"

#include <vector>

namespace my_class {

	class Freehand :public Shape
	{
	public:
		Freehand();
		~Freehand();

		void Draw(QPainter& painter);
	private:
		QPoint nPoint;
		std::vector<QPoint> vPoint;
	};
}