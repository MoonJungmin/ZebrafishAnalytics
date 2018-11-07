#include "LayerLabel.h"



LayerLabel::LayerLabel()
{
}


LayerLabel::~LayerLabel()
{
}

void LayerLabel::Init(std::string path) {
	layerPath = path;
	mifs.open(layerPath, std::ifstream::binary);


	this->headerReader();
}

void LayerLabel::headerReader() {
	Utils mUtil;
	std::string line1;
	std::getline(mifs, line1);
	std::vector<std::string> list_line1 = mUtil.Split(line1.c_str(), " : ");
	DataSizeX = atoi(list_line1.back().c_str());

	std::string line2;
	std::getline(mifs, line2);
	std::vector<std::string> list_line2 = mUtil.Split(line2.c_str(), " : ");
	DataSizeY = atoi(list_line2.back().c_str());

	std::string line3;
	std::getline(mifs, line3);
	std::vector<std::string> list_line3 = mUtil.Split(line3.c_str(), " : ");
	DataSizeZ = atoi(list_line3.back().c_str());

	std::string line4;
	std::getline(mifs, line4);
	std::vector<std::string> list_line4 = mUtil.Split(line4.c_str(), " : ");
	DataBlockSize = atoi(list_line4.back().c_str());

	std::string line5;
	std::getline(mifs, line5);
	std::vector<std::string> list_line5 = mUtil.Split(line5.c_str(), " : ");
	DataLevel = atoi(list_line5.back().c_str());

	std::string line6;
	std::getline(mifs, line6);
	std::vector<std::string> list_line6 = mUtil.Split(line6.c_str(), " : ");
	DataType = list_line6.back().c_str();

	std::vector<std::string> dir_pathlist = mUtil.Split(layerPath.c_str(), "/");
	std::string datapath;
	for (int i = 0; i < dir_pathlist.size() - 1; ++i) {
		datapath += dir_pathlist.at(i) + "/";
	}
	DataPathXY += datapath + "XY/";
	DataPathYZ += datapath + "YZ/";
	DataPathZX += datapath + "ZX/";

	qDebug() << QString::fromStdString(DataPathXY);
	qDebug() << QString::fromStdString(DataPathYZ);
	qDebug() << QString::fromStdString(DataPathZX);

	
}

int LayerLabel::LoadBlockBySerialIndex(label_layer tempblock) {

	std::ifstream frame;
	std::string frame_path;
	long long int lvalue = 1;
	long long int seekvalue = lvalue * DataBlockSize * DataBlockSize * sizeof(unsigned int);


	if (tempblock.load_axis_code == 1) {
		std::string filename = std::to_string(tempblock.index_z / DataBlockSize) + "_" + std::to_string(tempblock.index_y) + "_" + std::to_string(tempblock.index_x) + "_" + std::to_string(tempblock.level) + ".block";
		qDebug() << QString::fromStdString(DataPathXY + filename);
		frame_path = DataPathXY + filename;

		seekvalue *= (tempblock.index_z % DataBlockSize);
		//frame.open(DataPathXY + filename, std::ifstream::binary);
	}
	else if (tempblock.load_axis_code == 2) {
		std::string filename = std::to_string(tempblock.index_z) + "_" + std::to_string(tempblock.index_y) + "_" + std::to_string(tempblock.index_x / DataBlockSize) + "_" + std::to_string(tempblock.level) + ".block";
		qDebug() << QString::fromStdString(DataPathYZ + filename);
		frame_path = DataPathYZ + filename;

		seekvalue *= (tempblock.index_x % DataBlockSize);
		//frame.open(DataPathYZ + filename, std::ifstream::binary);
	}
	else if (tempblock.load_axis_code == 3) {
		std::string filename = std::to_string(tempblock.index_z) + "_" + std::to_string(tempblock.index_y / DataBlockSize) + "_" + std::to_string(tempblock.index_x) + "_" + std::to_string(tempblock.level) + ".block";;
		qDebug() << QString::fromStdString(DataPathZX + filename);
		frame_path = DataPathZX + filename;
		seekvalue *= (tempblock.index_y % DataBlockSize);
		//frame.open(DataPathZX + filename, std::ifstream::binary);
	}
	frame.open(frame_path, std::ifstream::binary);


	if (!frame) {
		qDebug() << "labellayer-openfail";
		frame.close();
		tempblock.data = NULL;
		tempblock.status = false;
		BlockList.push_back(tempblock);
		return BlockList.size() - 1;
	}
	else {

		tempblock.data = new unsigned int[DataBlockSize * DataBlockSize];
		tempblock.status = true;
		frame.seekg(seekvalue, std::ios::beg);
		frame.read((char *)&tempblock.data[0], sizeof(unsigned int)*DataBlockSize * DataBlockSize);
		BlockList.push_back(tempblock);

		frame.close();
		return BlockList.size() - 1;
/*


		tempblock.data = new unsigned int[DataBlockSize * DataBlockSize];
		tempblock.status = true;
		frame.read((char *)&tempblock.data[0], sizeof(unsigned int)*DataBlockSize * DataBlockSize);
		BlockList.push_back(tempblock);


		frame.close();
		return BlockList.size() - 1;*/
	}
}

int LayerLabel::checkBlockIndex(int idx_x, int idx_y, int idx_z) {
	int rt_value = -1;
	int index = 0;
	std::list<label_layer>::iterator iter;
	for (iter = BlockList.begin(); iter != BlockList.end(); ++iter) {
		if (iter->index_x == idx_x && iter->index_y == idx_y && iter->index_z == idx_z) {
			rt_value = 1;
			break;
		}
		index++;
	}
	if (rt_value == 1) {
		return index;
	}
	else {
		return -1;
	}
}

void LayerLabel::removeBlock() {
	while (BlockList.size() > 100) {
		BlockList.pop_front();
	}
}