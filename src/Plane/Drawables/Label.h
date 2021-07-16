#ifndef LABEL_H
#define LABEL_H

#include "Arc.h"

class Label : public DrawableObject, public UserInputRequester
{
public:
	Label();
	Label(QPointF location, QString text);

	//file handling
	void loadData(nlohmann::json jsonInput) override;
	nlohmann::json toJson() override;

	//getters and setters
	QString getText();
	void setText(QString text);
	QPointF getLocation();
	void setLocation(QPointF location);

	void requestText();

	//QGraphicsItem overrides
	QRectF boundingRect() const override;
	void paint(QPainter *painter,
			   const QStyleOptionGraphicsItem *option,
			   QWidget *widget
			   ) override;

private:
	QString text;
	QPointF location;

	int textWidth = 10;
	int textHeight = 20;

	//events
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

public slots:

	//user input requests
	void recieveString(QString value) override;
};

#endif // LABEL_H
