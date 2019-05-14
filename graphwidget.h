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

	GraphWidget(std::vector<NodeData> node_datas, QWidget *parent = 0);
	virtual ~GraphWidget();

private:
	void repaint(int ifcontructor = 0);
	void changeNode(QGraphicsItem* sourceItem, QGraphicsItem* destItem);
	void ChangeInFirstTime(QGraphicsItem* item);

public slots:
	void clickBeforeStepButton();
	void clickAnswerButton();
;

protected:
#if QT_CONFIG(wheelevent)
#endif
	void mousePressEvent(QMouseEvent* event) override;
	void putStack1(QGraphicsItem* item, QGraphicsItem* item2);

private:
	using pdd = std::pair<double, double>;
	using pnn = std::pair<NodeData, NodeData>;
	int timerId;
	std::vector<NodeData> constNodesdata;
	std::vector < NodeData> nodesData;
	std::stack<pnn> backsData1;
	int changeTime;
	Edge *theFirstEdge;
	QMainWindow q;

};

#endif GRAPHWIDGET_H
