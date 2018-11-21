#include "DataFeature.h"



DataFeature::DataFeature()
{
}


DataFeature::~DataFeature()
{
}

void DataFeature::Initialize(std::string name, std::string path) {
	qDebug() << "DataFeature : " << QString::fromStdString(name) << " " << QString::fromStdString(path);
	FeatureName = name;
	std::ifstream mIfs;
	mIfs.open(path);
	while (!mIfs.eof()) {
		unsigned int index;
		double value;
		mIfs >> index >> value;
		setFeatureValue(index, value);
	}
	mIfs.close();
}

void DataFeature::setFeatureValue(unsigned int index, double value) {
	FeatureMap.insert(std::pair<unsigned int, double>(index, value));
}
