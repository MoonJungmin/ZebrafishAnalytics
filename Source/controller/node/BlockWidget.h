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
	void initialize(int aflags, int awidth, int aheight, QColor acolor, QNEBlock *parent);

	QColor BackgroundColor;
	QColor DataColor;

	QWidget *ToolBox;
	ViewHeatmapGLWidget *DataHeatmap;

	QWidget *DataInputOutput;
	QComboBox *SubregionDropdown;
	
	QWidget *FeatureHistogramMaster;
	ViewHistogramWidget *FeatureHistogram;

	QComboBox *FeatureDropdown;
	QComboBox *SetDropdown;
	
	QWidget *SimilarityTarget;
	QListWidget *SimilarityList;


	QPushButton *BucketColorBtn;
	QPushButton *CloseBtn;
	QPushButton *AnnotationBtn;

	std::list<unsigned int> CellIndexListInput;
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


private:
	QNEBlock * m_block;

	QLabel *count_input;
	QLabel *count_output;

	void generate_ToolBox(int aflag);
	void generate_DataHeatmap(int width, int height);
	void generate_DataInputOutput(int width, int height);
	void generate_SubregionDropdown(int width, int height);
	void generate_FeatureHistogram(int width, int height);
	void generate_FeatureDropdown(int width, int height);
	void generate_SetDropdown(int width, int height);
	void updateColorBox(QPushButton *target, QColor color);

	void checkNextBlock();

};
