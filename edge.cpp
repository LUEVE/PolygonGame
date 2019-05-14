#pragma once
#include "edge.h"
#include "node.h"
#include <Qpainter>
#include "qdebug.h"

QRectF Edge::boundingRect() const
{
	if (!source || !dest)
		return QRectF();

	qreal penWidth = 3;
	qreal extra = (penWidth) / 2.0;

	return QRectF(sourcePoint, QSizeF(destPoint.x() - sourcePoint.x(),
		destPoint.y() - sourcePoint.y()))
		.normalized()
		.adjusted(-extra, -extra, extra, extra);
}
void Edge::adjust()
{
	if (!source || !dest)
		return;

	QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
	qreal length = line.length();


		sourcePoint = line.p1();
		destPoint = line.p2();
}
void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QLineF line(sourcePoint, destPoint);
	if (qFuzzyCompare(line.length(), qreal(0.)))
		return;
	painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter->drawLine(line);
	painter->setBrush(Qt::black);

}

void Edge::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	qDebug() << event << '\n';
}

Edge::Edge(Node* sourceNode, Node* destNode)
{
	source = sourceNode;
	dest = destNode;
	adjust();
}
Node *Edge::sourceNode() const
{
	return source;
}

Node *Edge::destNode() const
{
	return dest;
}