#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include <QString>
#include <QDebug>
#include <list>


#include "Source/Utils.h"


class LayerBackground
{
public:
	LayerBackground();
	~LayerBackground();
	
	std::string layerPath;

	std::ifstream mifs;
	std::list<back_layer> BlockList;

	
	void Init(std::string path);
	void headerReader();
	
	int checkBlockIndex(int idx_x, int idx_y, int idx_z, int level, int axis);
	int LoadBlockBySerialIndex(back_layer tempblock);
	void removeBlock();

private:

	int DataSizeX = 0;
	int DataSizeY = 0;
	int DataSizeZ = 0;
	int DataBlockSize = 0;
	int DataLevel = 0;

	std::string DataType;
	std::string DataPathXY;
	std::string DataPathYZ;
	std::string DataPathZX;

};

