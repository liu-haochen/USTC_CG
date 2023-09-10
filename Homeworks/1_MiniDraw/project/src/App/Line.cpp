#include "Line.h"

Line::Line()
{
}

Line::~Line()
{
}

void Line::Draw(QPainter& painter)
{
	painter.drawLine(start, end);//在画布上绘制直线
}
