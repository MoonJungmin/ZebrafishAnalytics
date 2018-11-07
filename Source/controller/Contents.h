#pragma once

#include <QObject>
#include <QPushButton>
#include <QtWidgets>
#include <QLabel>
#include <QSizePolicy>
#include <math.h>


#include "Source/controller/node/qneblock.h"
#include "Source/controller/node/qnodeseditor.h"
#include "Source/controller/node/qneport.h"

#include <QGraphicsScene>
#include <QFileDialog>


#include "Source/global.h"
#include "Source/controller/TopInterface.h"
#include "Source/view/ViewHistogramBox.h"
#include "Source/view/ViewAxisGLWidget.h"

class Contents : public QObject
{
	Q_OBJECT

public:
	void initialize(QLayout *parent_layout, TopInterface *aTopInterface);
	Contents(QWidget *parent);
	~Contents();

public slots:
	void focusmanage_histogram(int index);

private slots:
	void handleProjectOn();
	void handleAnalyticsOn();
	void updateAllWidget(int index, bool scaleflag);

	

private:

	void updateColorBox(QWidget *target, QColor color);

	void LeftTabInit(QTabWidget *target, int width, int height);
	void CenterTabInit(QTabWidget *target, int width, int height);
	void RightTabInit(QTabWidget *target, int width, int height);

	void replaceTab(QTabWidget * tabs, int index, QWidget * replacement, QString label);


	void CenterContentsOn();
	void RightContentsOn();

	void YZ_View(QGLWidget *widget);
	void ZX_View(QGLWidget *widget);

	TopInterface *mInterface;

	QWidget * mWidget;

	QTabWidget *contents_left_tabwidget;
	QTabWidget *contents_center_tabwidget;
	QTabWidget *contents_right_tabwidget;

	ViewAxisGLWidget *GL_XYAxis_Main;
	ViewAxisGLWidget *GL_YZAxis_Main;
	ViewAxisGLWidget *GL_ZXAxis_Main;
	ViewAxisGLWidget *GL_XYAxis_Sub;
	ViewAxisGLWidget *GL_YZAxis_Sub;
	ViewAxisGLWidget *GL_ZXAxis_Sub;

	ViewHistogramBox *MorphoHistBox[5];

};
