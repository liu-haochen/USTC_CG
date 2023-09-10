#include "Ellipse.h"

my_class::Ellipse::Ellipse()
{
}

my_class::Ellipse::~Ellipse()
{
}

void my_class::Ellipse::Draw(QPainter& painter)
{
	painter.drawEllipse(start.rx(), start.ry(), -start.rx()+end.rx(), -start.ry()+end.ry());
}