#pragma once

#include "shape.h"
namespace my_class
{
	class Ellipse : public Shape {
	public:
		Ellipse();
		~Ellipse();

		void Draw(QPainter& paint);
	};
};