#pragma once
#ifndef ANSWERWIDGET_H
#define ANSWERWIDGET_H

#include "pch.h"
#include "edge.h"
#include "nodeData.h"
struct NodeData;
class Node;


class AnswerWidget: public QGraphicsView
{
	Q_OBJECT

public:
	AnswerWidget(QWidget *parent = 0);
	void paintInit();
	bool getEdgeKind(Node* node);
	bool getEdgeKind(NodeData nodedata);

	AnswerWidget(std::vector<NodeData> node_datas, QWidget *parent = 0);
	virtual ~AnswerWidget();

private:
	void repaint(int ifcontructor = 0);
	void changeNode(QGraphicsItem* sourceItem, QGraphicsItem* destItem);
	void ChangeInFirstTime(QGraphicsItem* item);

	//void itemMoved();

public slots:
	void clickBeforeStepButton();
	//void clickAnswerButton();
	//void shuffle();
	//void zoomIn();
	//void zoomOut();

protected:
	//void keyPressEvent(QKeyEvent *event) override;
	//void timerEvent(QTimerEvent *event) override;
#if QT_CONFIG(wheelevent)
	//void wheelEvent(QWheelEvent *event) override;
#endif
	//void drawBackground(QPainter *painter, const QRectF &rect) override;
	//void mousePressEvent(QMouseEvent* event) override;
	//void putStack(QGraphicsItem* item,int v1, QGraphicsItem* item2,int v2);
	void putStack1(QGraphicsItem* item, QGraphicsItem* item2);
	bool isSame(double l, double r);
	//void scaleView(qreal scaleFactor);

private:
	using pdd = std::pair<double, double>;
	using pnn = std::pair<NodeData, NodeData>;
	int timerId;
	std::vector < NodeData> nodesData;
	//std::map < pdd, bool > edgeKind;
//	QStack<Edge> backsData;
	//std::stack<Edge*> backsData;
	std::stack<pnn> backsData1;
	QPushButton beforeStepButton;
	QPushButton answerButton;
	int changeTime;
	Edge *theFirstEdge;
	QMainWindow q;
	//QVector<Node*> nodes;
};


#endif ANSWERWIDGET_H