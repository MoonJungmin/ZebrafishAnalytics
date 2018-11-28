#pragma once

#include <QObject>
#include <QtWidgets>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QSizePolicy>

#include <iostream>
#include <vector>
#include <math.h>
#include <QColorDialog>

#include "Source/Utils.h"
#include "Source/view/histogram/Histogram.h"
#include "Source/view/histogram/HistogramViewer.h"
#include "Source/global.h"

class ViewHistogramWidget : public QWidget
{
	Q_OBJECT

signals:
	void OutputUpdated();

public:
	ViewHistogramWidget(QWidget *parent);
	void setData(int feature_index, std::list<unsigned int> *in_ptr, std::list<unsigned int> *out_ptr);
	void setRenderingSize(int width, int height);
	void redraw();
	~ViewHistogramWidget();
	
	


public slots:
	void update_move_box(float start, float end);
	void update_release_box(float start, float end);
	void clear_box();


private:
	int mWidth;
	int mHeight;
	QVBoxLayout * HistBoxLayout;
	
	int HistStartIdx = 0;
	int HistEndIdx = 0;


	QWidget *mWidget;
	std::list<unsigned int> *mInput;
	std::list<unsigned int> *mOutput;
	std::vector<std::list<unsigned int>> HistogramData;
	double mDataStep;
	double mDataMaximum;
	double mDataMinimum;

	QList<QBarSet *> mBarsets;

	void clearHistogramData();
	//void updateColorBox(QPushButton *target, QColor color);
	//void updateCellColor();
};

