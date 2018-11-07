#include "DataProject.h"



DataProject::DataProject()
{
}
DataProject::~DataProject()
{
}

void DataProject::ProjectMake(QString aProjName, QString aProjPath, QString aLayerBGPath, QString aLayerLBPath, QString aAnalyticsPath) {
	ProjectName = aProjName;
	ProjectPath = aProjPath;

	LayerBackgroundPath = aLayerBGPath;
	LayerLabelPath = aLayerLBPath;

	AnalyticsResultPath = aAnalyticsPath;;
	mAnalytics = new DataCellAnalytics;
	mAnalytics->Init(AnalyticsResultPath.toStdString());


	mLayerBack = new LayerBackground;
	mLayerBack->Init(LayerBackgroundPath.toStdString());
	mLayerLabel = new LayerLabel;
	mLayerLabel->Init(LayerLabelPath.toStdString());

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
	LayerBackgroundPath = QString::fromStdString(list_line3.back());
	qDebug() << LayerBackgroundPath;

	std::string line4;
	std::getline(mIFS, line4);
	std::vector<std::string> list_line4 = mUtil.Split(line4.c_str(), " : ");
	LayerLabelPath = QString::fromStdString(list_line4.back());
	qDebug() << LayerLabelPath;
/*
	std::string line5;
	std::getline(mIFS, line5);
	std::vector<std::string> list_line5 = mUtil.Split(line5.c_str(), " : ");
	AnalyticsResultPath = QString::fromStdString(list_line5.back());
*/
	

	AnalyticsStatus = true;
	std::vector<std::string> dir_path = mUtil.Split(LayerLabelPath.toStdString().c_str(), "header.lbl");
	std::string infoPath = dir_path[0] + "cell.dat";
	qDebug() << QString::fromStdString(infoPath);
	mAnalytics = new DataCellAnalytics;
	mAnalytics->Init(infoPath);


	////emit analytics_on();

	//
	mLayerBack = new LayerBackground;
	mLayerBack->Init(LayerBackgroundPath.toStdString());
	mLayerLabel = new LayerLabel;
	mLayerLabel->Init(LayerLabelPath.toStdString());




	ProjectStatus = true;
	ProjectSizeLoad();
}
void DataProject::ProjectSave(QString aProjPath) {
	std::ofstream mOFS(aProjPath.toStdString());

	std::string proj_name = "#project name : " + ProjectName.toStdString();
	std::string proj_path = "#project path : " + ProjectPath.toStdString();
	std::string layerbackground = "#layer background path : " + LayerBackgroundPath.toStdString();
	std::string layerlabel = "#layer labeled path : " + LayerLabelPath.toStdString();
	//std::string analytics_path = "#analytics result path : " + AnalyticsResultPath.toStdString();

	mOFS << proj_name << std::endl;
	mOFS << proj_path << std::endl;
	mOFS << layerbackground << std::endl;
	mOFS << layerlabel << std::endl;
	//mOFS << analytics_path << std::endl;
	
	mOFS.close();
}

void DataProject::ProjectSizeLoad() {
	Utils mUtil;
	std::ifstream tempIfs(LayerBackgroundPath.toStdString(), std::ifstream::binary);
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
	ViewPos_Z = 0;
	ViewZoomLevel = 2;
	
	emit project_on();
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

