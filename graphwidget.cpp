#pragma once
#include "graphwidget.h"
#include "node.h"
#include "qdebug.h"
#include <QKeyEvent>
#include <cassert>


GraphWidget::GraphWidget(QWidget* parent)
	: QGraphicsView(parent), timerId(0),changeTime(1),theFirstEdge(nullptr)
{
	paintInit();
	repaint(1);
}

GraphWidget::GraphWidget(std::vector<NodeData> node_datas, QWidget* parent)
	: QGraphicsView(parent), timerId(0),changeTime(1),theFirstEdge(nullptr)
{
	for (auto a : node_datas)
		nodesData.push_back(a);
	
	paintInit();
	repaint(1);
	constNodesdata = nodesData;

}

GraphWidget::~GraphWidget()
{
	delete theFirstEdge;
	theFirstEdge = nullptr;
}


void GraphWidget::paintInit()
{
	// init
	QGraphicsScene *_scene = new QGraphicsScene(this);
	_scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	_scene->setSceneRect(-200, -200, 400, 400);
	setScene(_scene);
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	scale(qreal(0.8), qreal(0.8));
	setMinimumSize(400, 400);
	setWindowTitle(tr("Elastic Nodes"));

}

bool GraphWidget::getEdgeKind(Node* node)
{
	int flag = 0;
	int kind = -1;
	double tx = node->x();
	double ty = node->y();
	for (auto a : nodesData)
	{
		if(isSame(node->x(),a.x) 
			&& isSame(node->y(),a.y))
		{
			kind = a.kind;
			flag = 1;
			break;

		}

	}

	if(flag)
		return kind;
	assert(-1);
	return false;
}


bool GraphWidget::getEdgeKind(NodeData nodedata)
{
	
	
	return nodedata.kind;


}

void GraphWidget::repaint(int ifcontructor)
{

	scene()->clear();
	



	QVector<Node*> nodes;
	QFont font;
	font.setPointSize(20);
	for (int i = 0;i < (int)nodesData.size();i++)
	{
		
		Node *node = new Node(this);
		nodes.push_back(node);
		node->setPos({ nodesData[i].x, nodesData[i].y });
		node->setvalue(nodesData[i].v);
		scene()->addItem(node);

		scene()->addText(QString::number(nodesData[i].v),font)->setPos({ nodesData[i].x, nodesData[i].y });
	}

	for (int i = 1;i < (int)nodes.size();i++)
	{
		Edge *edge = new Edge(nodes[i - 1], nodes[i]);


		bool kind = getEdgeKind(nodes[i - 1]);
		QString n = kind ? "*" : "+";
		qDebug() << n << " ";
		scene()->addItem(edge);

		double x = (nodes[i - 1]->x() + nodes[i]->x()) / 2;
		double y = (nodes[i - 1]->y() + nodes[i]->y()) / 2;
		scene()->addText(n, font)->setPos(x, y);
	}

	if (nodes.size() > 1 && changeTime == 1)
	{
		Edge *edge = new Edge(nodes[nodes.size() - 1], nodes[0]);
		scene()->addItem(edge);
		int i = nodes.size();
		bool kind = getEdgeKind(nodes[i - 1]);
		QString n = kind ? "*" : "+";
		//qDebug() << n << " ";
		scene()->addItem(edge);
		
		double x = (nodes[i - 1]->x() + nodes[0]->x()) / 2;
		double y = (nodes[i - 1]->y() + nodes[0]->y()) / 2;
		scene()->addText(n, font)->setPos(x, y);
	}
}



void GraphWidget::changeNode(QGraphicsItem* sourceItem,QGraphicsItem *destItem)
{

	int _v = INT_MAX;
	bool sourKind = false;
	
	for (auto i = nodesData.begin();i != nodesData.end();i++)
	{
		if(isSame(i->x,sourceItem->x()) && isSame(i->y,sourceItem->y()))
		{
			_v = i->v;
			sourKind = getEdgeKind(*i);
			nodesData.erase(i);
			break;
		}
	}

	for (auto i = nodesData.begin();i!= nodesData.end();i++)
	{
		if (isSame(i->x, destItem->x()) && isSame(i->y, destItem->y()))
		{
			if (_v == INT_MAX) assert(-1);

			if (sourKind)
			{
				(*i).v *= _v;
			}
			else
			{
				(*i).v += _v;
			}
				break;
		}
	}
}

void GraphWidget::ChangeInFirstTime(QGraphicsItem *item)
{
	QVector<NodeData> temp;
	qDebug() << item->x() << "|" << item->y() << '\n';
	while (!(isSame(nodesData.front().x,item->x()) && isSame(nodesData.front().y,item->y())))
	{
		auto t = nodesData.front(); nodesData.erase(nodesData.begin());
		nodesData.push_back(t);
	}

	return;
}

void GraphWidget::clickBeforeStepButton()
{
	if(changeTime == 1) return;
	if (changeTime == 2)
	{
		delete theFirstEdge;
		theFirstEdge = nullptr;
		changeTime--;
		repaint();
		return;
		
	}
	if(changeTime > 2)
	{
		auto t = backsData1.top();
		backsData1.pop();

		for (auto i = nodesData.begin(); i != nodesData.end() ; i++)
		{


			if(isSame(i->x,t.second.x) 
				&& isSame(i->y,t.second.y))
			{
				bool k = t.first.kind;
				int bv = t.first.v;
				if(k)
				{
					i->v /= bv;
				}
				else
				{
					i->v -= bv;
				}
				nodesData.insert(i, t.first);
				break;
			}
		}
		repaint();
	}

	changeTime--;


	qDebug() << "before step" << '\n';
}

void GraphWidget::clickAnswerButton()
{

	AnswerWidget *widget = new AnswerWidget(constNodesdata);
	q.setCentralWidget(widget);
	q.show();


}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
	if (QGraphicsItem *item = itemAt(event->pos())) {
		if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
		{
			scene()->removeItem(item);
			
			if(changeTime == 1)
			{
				ChangeInFirstTime(edge->destNode());
				delete theFirstEdge;
				theFirstEdge = new Edge(edge->sourceNode(), edge->destNode());
				
			}
			else
			{
				putStack1(edge->sourceNode(), edge->destNode());
				changeNode(edge->sourceNode(), edge->destNode());


			}
			changeTime++;
			repaint();
		}

		
	}
	else {
		qDebug("You didn't click on an item.");
	}
}




void GraphWidget::putStack1(QGraphicsItem *item,QGraphicsItem* item2)
{
	NodeData n1, n2;
	for (auto a:nodesData)
	{
		if (isSame(item->x(), a.x)
			&& isSame(item->y(), a.y))
		{
			n1 = a;
			break;
		}
	}

	for (auto a : nodesData)
	{
		if (isSame(item2->x(), a.x)
			&& isSame(item2->y(), a.y))
		{
			n2 = a;
			break;
		}
	}

	backsData1.push({ n1,n2 });
}

bool isSame(double l,double r)
{
	const double eps = 1e-8;
	return abs(l - r) < eps;
}

