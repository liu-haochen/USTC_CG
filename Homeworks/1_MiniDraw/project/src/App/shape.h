#pragma once

#include <QtGui>
//������Shape

class Shape
{
public:
	Shape();
	virtual ~Shape();
	virtual void Draw(QPainter& paint) = 0;//���м̳е���Ҫ��д������ƺ���
	void set_start(QPoint s);//���û���ͼ�ε����
	void set_end(QPoint e);//���û���ͼ�ε��յ�
	virtual void AppendLine();

public:
	//ͼԪ����
	enum Type 
	{
		kDefault = 0,//��
		kLine = 1,//ֱ��
		kRect = 2,//����
		kEllipse = 3,//��Բ
		kPolygen = 4,//�����
		kFreehand = 5, 
	};

protected:
	QPoint start;//����ͼ�ε����
	QPoint end;//����ͼ�ε��յ�
};

