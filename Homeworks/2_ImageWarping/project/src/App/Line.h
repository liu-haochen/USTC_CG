#pragma once

#include "shape.h"

class Line : public Shape {
public:
	Line();
	~Line();

	void Draw(QPainter& painter);//绘制直线,传入的是画布
};
