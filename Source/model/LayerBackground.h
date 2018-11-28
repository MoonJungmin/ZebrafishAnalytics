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
	LayerBackground(std::string path);
	~LayerBackground();
	
	std::list<back_layer> BlockList;

	
	int checkBlockIndex(int idx_x, int idx_y, int idx_z, int level, int axis);
	back_layer initializeBlock(block_info info);
	int LoadBlockBySerialIndex(back_layer tempblock);
	void removeBlock();

	std::string BackgroundPath;
	std::string BackgroundHeaderPath;
	std::string BackgroundThumbnailPath;


private:
	void headerReader();

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

