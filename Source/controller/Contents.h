#pragma once

#include <QObject>
#include <QPushButton>
#include <QtWidgets>
#include <QLabel>
#include <QSizePolicy>
#include <math.h>

#include <QGraphicsScene>
#include <QFileDialog>


#include "Source/global.h"
#include "Source/view/ViewHistogramBox.h"
#include "Source/view/ViewAxisGLWidget.h"
#include "Source/view/ViewGraphEditor.h"

class ViewAxisGLWidget;
class ViewGraphEditor;


class Contents : public QObject
{
	Q_OBJECT

public:
	Contents(QWidget *parent);
	Contents(QWidget *parent, QLayout *parent_layout);
	~Contents();

	void InitGLView();
	void InitGraph();
	void InitProjectInfo();
	void InitSubregionFeatureList();
	
	ViewAxisGLWidget *GL_XYAxis_Main;
	ViewAxisGLWidget *GL_YZAxis_Main;
	ViewAxisGLWidget *GL_ZXAxis_Main;
	ViewAxisGLWidget *GL_XYAxis_Sub;
	ViewAxisGLWidget *GL_YZAxis_Sub;
	ViewAxisGLWidget *GL_ZXAxis_Sub;

	ViewGraphEditor *GraphEditorView;

	QLineEdit *ProjectName;
	QLineEdit *ProjectPath;
	QLineEdit *BackPath;
	QLineEdit *CellPath;


public slots:
	void feature_updated();
	void subregion_updated();

private slots:
	void updateAllWidget(int index, bool scaleflag);
	void handleSubregionAddBtn();
	void handleFeatureAddBtn();
	void handleDetailBtn();
	void handleDeleteBtn();



private:
	
	void updateColorBox(QWidget *target, QColor color);
	void LeftTabInit(QTabWidget *target, int width, int height);
	void CenterTabInit(QTabWidget *target, int width, int height);
	//void RightTabInit(QTabWidget *target, int width, int height);

	void replaceTab(QTabWidget * tabs, int index, QWidget * replacement, QString label);


	//void RightContentsOn();

	QWidget * mWidget;

	QTabWidget *contents_left_tabwidget;
	QTabWidget *contents_center_tabwidget;
	QVBoxLayout *contents_center_layout;

	QListWidget *subregionList;
	QListWidget *featureList;




	//ViewHistogramBox *MorphoHistBox[5];

	

};
