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
class LayerCell
{
public:
	LayerCell();
	~LayerCell();
	std::string CellPath;
	std::ifstream mLayerIFS;
	std::ifstream mCellIFS;

	
	std::list<label_layer> BlockList;


	std::vector<cell> mCellList;
	long long MaxCellIndex;
	long long MaxCellCount;
	bool CellColorGPU_On = false;


	void Init(std::string cell_path);
	void headerReader();
	void loadCellInformation();


	int checkBlockIndex(int idx_x, int idx_y, int idx_z);
	int LoadBlockBySerialIndex(label_layer tempblock);
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

