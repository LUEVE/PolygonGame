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
	void changeNode(NodeData& sourceItem);
	void ChangeInFirstTime(NodeData& node_data);


public slots:
	void clickBeforeStepButton();
	void clickNextStepButton();

protected:
#if QT_CONFIG(wheelevent)
#endif
	void putStack1(NodeData& item);
	bool isSame(double l, double r);

private:
	using pdd = std::pair<double, double>;
	using pnn = std::pair<NodeData, NodeData>;
	int timerId;
	std::vector < NodeData> nodesData;
	std::vector<NodeData> constNodesdata;
	std::stack<pnn> backsData1;
	std::stack<NodeData> ansData;
	static int clickNextFlagForFirst;
	QPushButton beforeStepButton;
	QPushButton nextStepButton;
	int changeTime;
	Edge *theFirstEdge;
	QMainWindow q;
};


#endif ANSWERWIDGET_H