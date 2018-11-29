#include "DataProject.h"



DataProject::DataProject()
{
	SelectedColor = QColor(55, 55, 55);
	UnSelectedColor = QColor(125, 11, 11);
}
DataProject::~DataProject()
{
}

void DataProject::ProjectMake(QString aProjName, QString aProjPath, QString aLayerBGPath, QString aLayerLBPath, QString aAnalyticsPath) {
	ProjectName = aProjName;
	ProjectPath = aProjPath;

	mLayerBack = new LayerBackground(aLayerBGPath.toStdString());
	mLayerCell = new LayerCell(aLayerLBPath.toStdString());

	ProjectStatus = true;
	ProjectSizeLoad();
	ProjectSave(ProjectPath);
}
void DataProject::ProjectOpen(QString aProjPath) {
	Utils mUtil;

	mIFS.open(aProjPath.toStdString());
	std::string line1;
	std::getline(mIFS, line1);
	std::vector<std::string> list_line1 = mUtil.Split(line1.c_str(), " : ");
	ProjectName = QString::fromStdString(list_line1.back());
	qDebug() << ProjectName;

	std::string line2;
	std::getline(mIFS, line2);
	std::vector<std::string> list_line2 = mUtil.Split(line2.c_str(), " : ");
	ProjectPath = QString::fromStdString(list_line2.back());
	qDebug() << ProjectPath;

	std::string line3;
	std::getline(mIFS, line3);
	std::vector<std::string> list_line3 = mUtil.Split(line3.c_str(), " : ");
	QString aLayerBGPath = QString::fromStdString(list_line3.back());
	qDebug() << aLayerBGPath;
	mLayerBack = new LayerBackground(aLayerBGPath.toStdString());
	

	std::string line4;
	std::getline(mIFS, line4);
	std::vector<std::string> list_line4 = mUtil.Split(line4.c_str(), " : ");
	QString aLayerLBPath = QString::fromStdString(list_line4.back());
	qDebug() << aLayerLBPath;
	mLayerCell = new LayerCell(aLayerLBPath.toStdString());
	
	ProjectStatus = true;
	ProjectSizeLoad();
}
void DataProject::ProjectSave(QString aProjPath) {
	std::ofstream mOFS(aProjPath.toStdString());

	std::string proj_name = "#project name : " + ProjectName.toStdString();
	std::string proj_path = "#project path : " + ProjectPath.toStdString();
	std::string layerbackground = "#layer background path : " + mLayerBack->BackgroundHeaderPath;
	std::string LayerCell = "#layer labeled path : " + mLayerCell->CellHeaderPath;
	//std::string analytics_path = "#analytics result path : " + AnalyticsResultPath.toStdString();

	mOFS << proj_name << std::endl;
	mOFS << proj_path << std::endl;
	mOFS << layerbackground << std::endl;
	mOFS << LayerCell << std::endl;
	//mOFS << analytics_path << std::endl;
	
	mOFS.close();
}

void DataProject::ProjectSizeLoad() {
	Utils mUtil;
	std::ifstream tempIfs(mLayerBack->BackgroundHeaderPath, std::ifstream::binary);
	std::string line1;
	std::getline(tempIfs, line1);
	std::vector<std::string> list_line1 = mUtil.Split(line1.c_str(), ":");
	DataSizeX = atoi(list_line1.back().c_str());

	std::string line2;
	std::getline(tempIfs, line2);
	std::vector<std::string> list_line2 = mUtil.Split(line2.c_str(), ":");
	DataSizeY = atoi(list_line2.back().c_str());

	std::string line3;
	std::getline(tempIfs, line3);
	std::vector<std::string> list_line3 = mUtil.Split(line3.c_str(), ":");
	DataSizeZ = atoi(list_line3.back().c_str());

	std::string line4;
	std::getline(tempIfs, line4);
	std::vector<std::string> list_line4 = mUtil.Split(line4.c_str(), ":");
	DataBlockSize = atoi(list_line4.back().c_str());

	std::string line5;
	std::getline(tempIfs, line5);
	std::vector<std::string> list_line5 = mUtil.Split(line5.c_str(), " : ");
	DataLevel = atoi(list_line5.back().c_str());

	tempIfs.close();

	ViewPos_X = DataSizeX / 2;
	ViewPos_Y = DataSizeY / 2;
	ViewPos_Z = DataSizeZ / 2;
	ViewZoomLevel = 1;
	
	emit project_on();
}

void DataProject::AddFeature(QString aPath) {
	std::string path = aPath.toStdString();
	Utils mUtil;
	std::vector<std::string> list_line1 = mUtil.Split(path.c_str(), "/");
	std::vector<std::string> list_line2 = mUtil.Split(list_line1.back().c_str(), ".dat");
	std::string name = list_line2.front();


	DataFeature temp(name, path);
	mFeature.push_back(temp);
}

void DataProject::removeFeature(int index) {
	int step = 0;
	std::vector<DataFeature>::iterator iter;
	for (iter = this->mFeature.begin(); iter != this->mFeature.end(); ++iter) {
		if (step == index) {
			this->mFeature.erase(iter);
			break;
		}
	}
	step++;
}

void DataProject::AddSubregion(QString aPath) {
	std::string path = aPath.toStdString();
	Utils mUtil;
	std::vector<std::string> list_line1 = mUtil.Split(path.c_str(), "/");
	std::string name = list_line1[list_line1.size()-2];
	
	LayerSubregion temp(name, path);
	mSubregion.push_back(temp);
}

void DataProject::removeSubregion(int index) {
	std::vector<LayerSubregion>::iterator iter;
	for (iter = this->mSubregion.begin(); iter != this->mSubregion.end(); ++iter) {
		if (iter->SubregionIndex == index) {
			this->mSubregion.erase(iter);
			break;
		}
	}
	qDebug() << "removeSubregion";
}

int DataProject::getSerialIndex(int x, int y, int z, int lv) {
	Utils mUtil;
	if (x < 0 || y < 0 || z < 0) {
		return -1;
	}
	else {
		int count_sum = 0;
		int size_x;
		int size_y;
		int size_z;
		int x_count;
		int y_count;
		int z_count;

		for (int i = 0; i < lv; ++i) {
			mUtil.CalcXYZCount(&x_count, &y_count, &z_count, i, DataSizeX, DataSizeY, DataSizeZ, DataBlockSize);
			count_sum = x_count * y_count * z_count;
		}
		mUtil.CalcXYZCount(&x_count, &y_count, &z_count, lv, DataSizeX, DataSizeY, DataSizeZ, DataBlockSize);
		if (x < x_count && y < y_count && z < z_count)
			return count_sum + x_count * y_count*z + x_count * y + x;
		else
			return -1;
	}
}

