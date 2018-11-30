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

class QNEBlock;
class ViewHeatmapGLWidget;
class ViewHistogramWidget;

class BlockWidget : public QObject
{
	Q_OBJECT

signals:
	void deleteNode(QNEBlock *ptr);

public:
	enum { OriginBlock = 1, DataBlock = 2, FeatureBlock = 3, SubregionBlock = 4, SimilarityBlock = 5, SetBlock = 6 };

	BlockWidget(QObject *parent);
	~BlockWidget();
	void initialize(std::string name, int aflags, int awidth, int aheight, QColor acolor, QNEBlock *parent);

	QColor BackgroundColor;
	QString BackgroundColor_style;
	QColor DataColor;

	QWidget *ToolBox;
	ViewHeatmapGLWidget *DataHeatmap;

	QWidget *DataInputOutput;
	
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

private slots:
	void handleBlockColorBtn();
	void handleCloseBtn();
	void handleAnnotationBtn();
	void handleDropdownChange(int index);
	void handleHistogramUpdate();
	void handleStatisticsBtn();
	void handleHistogramSetBtn();

private:
	QNEBlock * m_block;
	std::string BlockName;
	QLabel *count_input1;
	QLabel *count_input2;
	QLabel *count_output;

	QLineEdit *histogram_start;
	QLineEdit *histogram_end;


	void generate_ToolBox(int aflag);
	void generate_DataHeatmap(int width, int height);
	void generate_DataInputOutput(int width, int height);
	void generate_SubregionDropdown(int width, int height);
	void generate_FeatureHistogram(int width, int height);
	void generate_FeatureDropdown(int width, int height);
	void generate_SetDropdown(int width, int height);
	void updateColorBox(QPushButton *target, QColor color);

	void checkNextBlock();
	void set_operation(int index);


	QFont mFont;

};
