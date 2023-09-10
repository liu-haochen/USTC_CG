#pragma once

#include "shape.h"
#include "Line.h"

#include <vector>
namespace my_class
{
	class Polygen :public Shape{
	public:
		Polygen();
		~Polygen();

		void Draw(QPainter& painter);
		void AppendLine();
	private:
		Line* pLNow;
		std::vector<Line> vLine;
	};
};