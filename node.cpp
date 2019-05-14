#pragma once
#include "node.h"
#include <QStyleOption>
#include "qdebug.h"

Node::Node(GraphWidget* graphWidget)
	:graph(graphWidget)
{
	setCacheMode(DeviceCoordinateCache);
	setZValue(-1);
}

Node::Node(AnswerWidget* answerwidget)
	:graphA(answerwidget)
{
	setCacheMode(DeviceCoordinateCache);
	setZValue(-1);
}


QRectF Node::boundingRect() const
{
	qreal adjust = 2;
	return QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

void Node::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->setPen(Qt::NoPen);
	painter->setBrush(Qt::darkGray);
	painter->drawEllipse(-7, -7, 20, 20);

	QRadialGradient gradient(-3, -3, 10);
	if (option->state & QStyle::State_Sunken) {
		gradient.setCenter(3, 3);
		gradient.setFocalPoint(3, 3);
		gradient.setColorAt(1, QColor(Qt::green).light(10));
		gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
	}
	else {
		gradient.setColorAt(0, Qt::yellow);
		gradient.setColorAt(1, Qt::darkYellow);
	}
	painter->setBrush(gradient);

	painter->setPen(QPen(Qt::black, 0));
	painter->drawEllipse(-10, -10, 23, 23);

}

void Node::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
	qDebug() << e << '\n';

}

void Node::setvalue(int v)
{
	value = v;
}

int Node::getvalue() const
{
	return value;
}

