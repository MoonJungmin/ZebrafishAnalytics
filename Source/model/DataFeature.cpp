#include "DataFeature.h"



DataFeature::DataFeature() {
}

DataFeature::DataFeature(std::string name, std::string path) {
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

DataFeature::~DataFeature() {
}


void DataFeature::setFeatureValue(unsigned int index, double value) {
	FeatureMap.insert(std::pair<unsigned int, double>(index, value));
}
