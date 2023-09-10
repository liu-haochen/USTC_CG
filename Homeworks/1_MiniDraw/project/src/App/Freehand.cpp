#include "Freehand.h"

my_class::Freehand::Freehand()
{
}

my_class::Freehand::~Freehand()
{
}

void my_class::Freehand::Draw(QPainter& painter)
{
	vPoint.push_back(end);
	for (auto& it = vPoint.begin(); it != vPoint.end();it++ ) {
		if(next(it)!= vPoint.end()){
			painter.drawLine(*it,*next(it)) ;//在画布上绘制点
		}
	}
}
