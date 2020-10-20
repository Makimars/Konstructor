#ifndef STYLE_H
#define STYLE_H

#include <QPen>
#include <QBrush>

struct Style
{
	Style(){};
	Style(QPen pen, QBrush brush){
		this->pen = pen;
		this->brush = brush;
	};
	QPen pen;
	QBrush brush;
};

#endif // STYLE_H
