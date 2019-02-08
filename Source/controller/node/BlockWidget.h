#pragma once

#include <QObject>
#include <QtWidgets>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QImage>
#include <algorithm>


#include "Source/Utils.h"
#include "qneblock.h"
#include "Source/view/ViewHeatmapGLWidget.h"
#include "Source/view/ViewHistogramWidget.h"
#include "Source/model/process/ThreadSubregion.h"

class QNEBlock;
class ViewHeatmapGLWidget;
class ViewHistogramWidget;
class ThreadSubregion;

class BlockWidget : public QObject
{
	Q_OBJECT

signals:
	void deleteNode(QNEBlock *ptr);

public slots:
	void handleSubregionInformationUpdate(QString);

public:
	enum { OriginBlock = 1, DataBlock = 2, FeatureBlock = 3, SubregionBlock = 4, SimilarityBlock = 5, SetBlock = 6 };

	BlockWidget(QObject *parent);
	~BlockWidget();
	void initialize(std::string name, int aflags, int awidth, int aheight, QColor acolor, QNEBlock *parent);
	ThreadSubregion *mThread;
	QColor BackgroundColor;
	QString BackgroundColor_style;
	QColor DataColor;

	QWidget *ToolBox;
	ViewHeatmapGLWidget *DataHeatmap;

	QWidget *DataInputOutput;
	
	QWidget *SubregionInformationMaster;
	QLabel *subregion_volume_unit;
	QWidget *SubregionDropdownMaster;
	QComboBox *SubregionDropdown;
	QComboBox *SubregionSelectMethod;
	

	QWidget *FeatureHistogramMaster;
	ViewHistogramWidget *FeatureHistogram;


	QWidget  *FeatureDropdownMaster;
	QComboBox *FeatureDropdown;
	
	QWidget  *SetDropdownMaster;
	QComboBox *SetDropdown;
	
	QWidget *SimilarityTarget;
	QListWidget *SimilarityList;


	QPushButton *BucketColorBtn;
	QPushButton *CloseBtn;
	QPushButton *AnnotationBtn;
	QPushButton *StatisticsBtn;

	std::list<unsigned int> CellIndexListInput;
	std::list<unsigned int> CellIndexListInput_Sub;

	std::list<unsigned int> CellIndexListOutput;
	std::list<annotation> mAnnotation;

	void addAnnotation(QString cmt);
	void removeAnnotation(int index);
	void updatedInputList();
	void updatedCellColor();
	void updatedSubregionList();
	void updatedFeatureList();

	int BlockFlag;

	int SubregionPrecision;
	double SubregionVolume = 0.0f;
	std::map<unsigned int, bool> TouchIndex;
	std::map<unsigned int, bool> IntersectIndex;
	std::map<unsigned int, bool> CompleteIndex;
	double plane_pos_x = 0;
	double plane_pos_y = 0;
	double plane_pos_z = 0;
	double plane_up_x = 1;
	double plane_up_y = 0;
	double plane_up_z = 0;


private slots:
	void handleBlockColorBtn();
	void handleCloseBtn();
	void handleAnnotationBtn();
	void handleDropdownChange(int index);
	void handleHistogramUpdate();
	void handleStatisticsBtn();
	void handleHistogramSetBtn_Percent();
	void handleHistogramSetBtn_Value();
	void handleSubregionSetBtn();

	void subregion_progess_update(float v);
	void subregion_progess_log_update(QString);


private:
	QNEBlock * m_block;
	std::string BlockName;
	QLabel *count_input1;
	QLabel *count_input2;
	QLabel *count_output;

	QLineEdit *histogram_start_percent;
	QLineEdit *histogram_end_percent;
	QLineEdit *histogram_start_value;
	QLineEdit *histogram_end_value;




	QLabel *subreigon_precision;
	QLineEdit *subreigon_volume;
	QLabel *SubregionProgressbarLog;
	QProgressBar *SubregionProgressbar;
	QLineEdit *subregion_pos_x;
	QLineEdit *subregion_pos_y;
	QLineEdit *subregion_pos_z;
	QLineEdit *subregion_up_x;
	QLineEdit *subregion_up_y;
	QLineEdit *subregion_up_z;

	/*bool SubregionActivated = true;
	bool SubregionVolumeStatus = false;*/



	void generate_ToolBox(int aflag);
	void generate_DataHeatmap(int width, int height);
	void generate_DataInputOutput(int width, int height);
	void generate_SubregionInformation(int awidth, int aheight);
	void generate_SubregionDropdown(int width, int height);
	void generate_FeatureHistogram(int width, int height);
	void generate_FeatureDropdown(int width, int height);
	void generate_SetDropdown(int width, int height);
	void updateColorBox(QPushButton *target, QColor color);

	void checkNextBlock();
	void set_operation(int index);


	QFont mFont;

};

