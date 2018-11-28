#include "LayerBackground.h"



LayerBackground::LayerBackground() {
}

LayerBackground::LayerBackground(std::string path) {
	
	BackgroundHeaderPath = path;
	
	Utils mUtil;
	std::vector<std::string> BackDirPath = mUtil.Split(path.c_str(), "header.bgl");
	BackgroundPath = BackDirPath[0];
	BackgroundThumbnailPath = BackDirPath[0] + "thumbnail.png";
	qDebug() << QString::fromStdString(BackgroundPath);

	this->headerReader();
}

LayerBackground::~LayerBackground() {
}

void LayerBackground::headerReader() {
	std::ifstream mifs(BackgroundHeaderPath, std::ifstream::binary);

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

	DataPathXY += BackgroundPath + "XY/";
	DataPathYZ += BackgroundPath + "YZ/";
	DataPathZX += BackgroundPath + "ZX/";

	qDebug() << QString::fromStdString(DataPathXY);
	qDebug() << QString::fromStdString(DataPathYZ);
	qDebug() << QString::fromStdString(DataPathZX);
}

int LayerBackground::LoadBlockBySerialIndex(back_layer tempblock){
	std::ifstream frame;
	std::string frame_path;
	long long int lvalue = 1;
	long long int seekvalue = lvalue * DataBlockSize * DataBlockSize;


	if (tempblock.load_axis_code == 1) {
		std::string filename = std::to_string(tempblock.index_z/DataBlockSize) + "_" + std::to_string(tempblock.index_y) + "_" + std::to_string(tempblock.index_x) + "_" + std::to_string(tempblock.level) + ".block";
		qDebug() << QString::fromStdString(DataPathXY + filename);
		frame_path = DataPathXY + filename;
		
		seekvalue *= (tempblock.index_z % DataBlockSize);
		//frame.open(DataPathXY + filename, std::ifstream::binary);
	}
	else if (tempblock.load_axis_code == 2) {
		std::string filename = std::to_string(tempblock.index_z) + "_" + std::to_string(tempblock.index_y) + "_" + std::to_string(tempblock.index_x/DataBlockSize) + "_" + std::to_string(tempblock.level) + ".block";
		qDebug() << QString::fromStdString(DataPathYZ + filename);
		frame_path = DataPathYZ + filename;

		seekvalue *= (tempblock.index_x % DataBlockSize);
		//frame.open(DataPathYZ + filename, std::ifstream::binary);
	}
	else if (tempblock.load_axis_code == 3) {
		std::string filename = std::to_string(tempblock.index_z) + "_" + std::to_string(tempblock.index_y/DataBlockSize) + "_" + std::to_string(tempblock.index_x) + "_" + std::to_string(tempblock.level) + ".block";;
		qDebug() << QString::fromStdString(DataPathZX + filename);
		frame_path = DataPathZX + filename;
		seekvalue *= (tempblock.index_y % DataBlockSize);
		//frame.open(DataPathZX + filename, std::ifstream::binary);
	}
	frame.open(frame_path, std::ifstream::binary);

	if (!frame) {
		qDebug() << "backlayer-openfail";
		frame.close();
		return -1;
	}
	else {

		tempblock.data = new unsigned char[DataBlockSize * DataBlockSize];
		tempblock.status = true;
		frame.seekg(seekvalue, std::ios::beg);
		frame.read((char *)&tempblock.data[0], DataBlockSize * DataBlockSize);
		BlockList.push_back(tempblock);

		frame.close();
		return BlockList.size() - 1;
	}
}

back_layer LayerBackground::initializeBlock(block_info info) {
	back_layer temp;
		
	if (info.axis == 1) {
		temp.index_x = info.x;
		temp.index_y = info.y;
		temp.index_z = info.z;
	}
	else if (info.axis == 2) {
		temp.index_x = info.z;
		temp.index_y = info.y;
		temp.index_z = info.x;
	}
	else if (info.axis == 3) {
		temp.index_x = info.x;
		temp.index_y = info.z;
		temp.index_z = info.y;
	}
	
	temp.pos_x = info.size * info.x;
	temp.pos_y = info.size * info.y;
	temp.pos_z = info.z;
	temp.size_x = info.size;
	temp.size_y = info.size;
	temp.size_z = info.size;
	temp.level = info.level;
	temp.load_axis_code = info.axis;
	return temp;
}

int LayerBackground::checkBlockIndex(int x, int y, int z, int level, int axis) {
	int rt_value = -1;
	int index = 0;

	int idx_x;
	int idx_y;
	int idx_z;

	if (axis == 1) {
		idx_x = x;
		idx_y = y;
		idx_z = z;
	}
	else if (axis == 2) {
		idx_x = z;
		idx_y = y;
		idx_z = x;
	}
	else if (axis == 3) {
		idx_x = x;
		idx_y = z;
		idx_z = y;
	}

	std::list<back_layer>::iterator iter;
	for (iter = BlockList.begin(); iter != BlockList.end(); ++iter) {
		if (iter->index_x == idx_x && iter->index_y == idx_y && iter->index_z == idx_z && iter->level == level && iter->load_axis_code == axis) {
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

void LayerBackground::removeBlock() {
	while (BlockList.size() > 100) {
		BlockList.pop_front();
	}
}
