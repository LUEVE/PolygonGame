#pragma once
#ifndef NODE_H
#define NODE_H


#include "edge.h"
#include "graphwidget.h"
#include "answerwidget.h"
class Node : public QGraphicsItem
{
public:
	Node(GraphWidget *graphWidget);
	Node(AnswerWidget *answerwidget);
	enum{Type = UserType + 1};
	int type() const override { return Type; }
	QRectF boundingRect() const override;
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	int getvalue() const;
	void setvalue(int v);
private:
	QList<Edge *> edgeList;
	QPointF newPos;
;	int value;
	GraphWidget *graph;
	AnswerWidget *graphA;
};




#endif 
