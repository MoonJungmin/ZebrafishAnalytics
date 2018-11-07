#include "DataCellAnalytics.h"



DataCellAnalytics::DataCellAnalytics()
{

}
void DataCellAnalytics::Init(std::string path) {
	AnalyticsResultPath = path;
	mifs.open(AnalyticsResultPath);
	qDebug() << "Init Cell";
	qDebug() << QString::fromStdString(AnalyticsResultPath);
	this->LoadCellInformation();
}

void DataCellAnalytics::LoadCellInformation() {
	QTime myTimer;
	myTimer.start();

	while (!mifs.eof()) {
		cell tmp;
		mifs >> tmp.index >> tmp.minbox.x >> tmp.minbox.y >> tmp.minbox.z >> tmp.maxbox.x >> tmp.maxbox.y >> tmp.maxbox.z;
/*
		if (volumeMax < tmp.volume) volumeMax = tmp.volume;
		if (sphericityMax < tmp.sphericity) sphericityMax = tmp.sphericity;
		if (surfaceareaMax < tmp.surfacearea) surfaceareaMax = tmp.surfacearea;
		if (eccentricityMax < tmp.eccentricity) eccentricityMax = tmp.eccentricity;
		if (intensityMax < tmp.intensity) intensityMax = tmp.intensity;

		if (volumeMin > tmp.volume) volumeMin = tmp.volume;
		if (sphericityMin > tmp.sphericity) sphericityMin = tmp.sphericity;
		if (surfaceareaMin > tmp.surfacearea) surfaceareaMin = tmp.surfacearea;
		if (eccentricityMin > tmp.eccentricity) eccentricityMin = tmp.eccentricity;
		if (intensityMin > tmp.intensity) intensityMin = tmp.intensity;*/

		mCellList.push_back(tmp);
	}

	MaxCellCount = mCellList.size();
	MaxCellIndex = mCellList[mCellList.size() - 1].index;
	
	qDebug() << "DataCellAnalytics";
	qDebug() << MaxCellCount;
	qDebug() << MaxCellIndex;
	
	
	GenerateHistogram();
	int nMilliseconds = myTimer.elapsed();
	int seconds = nMilliseconds / 1000 / 1000;
	qDebug() << nMilliseconds;
}
void DataCellAnalytics::LoadFeature() {


}


void DataCellAnalytics::GenerateHistogram() {
	int HistSize = HistWidth / HistBarWidth;
	/*volumeHist.reserve(HistSize);
	surfaceareaHist.reserve(HistSize);
	sphericityHist.reserve(HistSize);
	eccentricityHist.reserve(HistSize);
	intensityHist.reserve(HistSize);*/
	for (int i = 0; i < HistSize; ++i) {
		/*volumeHist[i] = 0;
		surfaceareaHist[i] = 0;
		sphericityHist[i] = 0;
		eccentricityHist[i] = 0;
		intensityHist[i] = 0;*/
		std::vector<long long> v;

		volumeHist.push_back(v);
		surfaceareaHist.push_back(v);
		sphericityHist.push_back(v);
		eccentricityHist.push_back(v);
		intensityHist.push_back(v);
	}
	qDebug() << "vector init done";

	std::vector<cell>::iterator iter = mCellList.begin();
	//for (iter = mCellList.begin(); iter != mCellList.end(); ++iter) {
	//	if (iter->intensity != -1) {
	//		int volume_hist_index = ((float)(iter->volume - volumeMin) / (float)(volumeMax - volumeMin)) * (float)(HistSize-1);
	//		int surfacearea_hist_index = ((float)(iter->surfacearea - surfaceareaMin) / (float)(surfaceareaMax - surfaceareaMin)) * (float)(HistSize - 1);
	//		int sphericity_hist_index = ((float)(iter->sphericity - sphericityMin) / (float)(sphericityMax - sphericityMin)) * (float)(HistSize - 1);
	//		int eccentricity_hist_index = ((float)(iter->eccentricity - eccentricityMin) / (float)(eccentricityMax - eccentricityMin)) * (float)(HistSize - 1);\
	//		int intensity_hist_index = ((float)(iter->intensity - intensityMin) / (float)(intensityMax - intensityMin)) * (float)(HistSize - 1);

	//		//qDebug() << hist_index;
	//		volumeHist[volume_hist_index].push_back(iter->index);
	//		surfaceareaHist[surfacearea_hist_index].push_back(iter->index);
	//		sphericityHist[sphericity_hist_index].push_back(iter->index);
	//		eccentricityHist[eccentricity_hist_index].push_back(iter->index);
	//		intensityHist[intensity_hist_index].push_back(iter->index);
	//	}
	//}
}


DataCellAnalytics::~DataCellAnalytics()
{
}
