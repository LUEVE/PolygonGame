#pragma once
#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "pch.h"
#include "nodeData.h"
#include "edge.h"
class Node;

bool isSame(double l, double r);
class GraphWidget : public QGraphicsView
{
	Q_OBJECT

public:
	GraphWidget(QWidget *parent = 0);
	void paintInit();
	bool getEdgeKind(Node* node);
	bool getEdgeKind(NodeData nodedata);
	//bool getEdgeKind(NodeData nodedata);
	//bool getEdgeKind(NodeData node_data);

	GraphWidget(std::vector<NodeData> node_datas, QWidget *parent = 0);
	virtual ~GraphWidget();

private:
	void repaint(int ifcontructor = 0);
	void paintSym(QVector<Node*> nodes, Edge* edge);
	void changeNode(QGraphicsItem* sourceItem, QGraphicsItem* destItem);
	void repainInBegin();
	void ChangeInFirstTime(QGraphicsItem* item);

	//void itemMoved();

public slots:
	void clickBeforeStepButton();
	void clickAnswerButton();
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
	void mousePressEvent(QMouseEvent* event) override;
	//void putStack(QGraphicsItem* item,int v1, QGraphicsItem* item2,int v2);
	void putStack1(QGraphicsItem* item, QGraphicsItem* item2);
	//void scaleView(qreal scaleFactor);

private:
	using pdd = std::pair<double, double>;
	using pnn = std::pair<NodeData, NodeData>;
	int timerId;
	std::vector<NodeData> constNodesdata;
	std::vector < NodeData> nodesData;

	std::stack<pnn> backsData1;
	QPushButton beforeStepButton;
	QPushButton answerButton;
	int changeTime;
	Edge *theFirstEdge;
	QMainWindow q;

};

#endif GRAPHWIDGET_H
