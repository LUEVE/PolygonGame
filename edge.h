#pragma once
#include <QGraphicsItem>

class Node;
class Edge:public QGraphicsItem
{
public:
	Edge(Node *sourceNode, Node *destNode);
	//Edge(const Edge &e);
	enum {Type = UserType + 2};
	int type() const override { return Type; };

	Node *sourceNode() const;
	Node *destNode() const;
protected:
	QRectF boundingRect() const override;
	void adjust();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
	bool edgeKind;
		
	Node *source, *dest;

	QPointF sourcePoint;
	QPointF destPoint;
	//qreal arrowSize;
};
