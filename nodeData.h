#pragma once
#ifndef NODEDATA_H
#define NODEDATA_H

struct NodeData
{
	double x, y;
	int v;
	// 1 is mul,0 is plus clockwise
	bool kind;
	bool operator < (const NodeData &r) const
	{
		return  v < r.v;
	}
};


#endif NODEDATA_H
