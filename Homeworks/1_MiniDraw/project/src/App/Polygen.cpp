#include "Polygen.h"

my_class::Polygen::Polygen()
{
	pLNow = new Line();
}

my_class::Polygen::~Polygen()
{
}

void my_class::Polygen::Draw(QPainter& painter)
{
	for (auto& aLine : vLine) {
		aLine.Draw(painter);
	}
	pLNow->set_start(start);
	pLNow->set_end(end);
	pLNow->Draw(painter);
}

void my_class::Polygen::AppendLine()
{
	pLNow->set_start(start);
	pLNow->set_end(end);
	vLine.push_back(*pLNow);
}