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
#include "Source/view/ViewHistogram.h"
#include "Source/view/ViewHistogramViewer.h"
#include "Source/global.h"

class ViewHistogramBox : public QWidget
{
	Q_OBJECT

public:
	ViewHistogramBox(QWidget *parent);
	void Init(QString name, int index, std::vector<std::vector<long long>> *data_ptr);
	
	~ViewHistogramBox();
	
	QFrame *HistBoxFrame;
	int mIndex;
	void FocusedIn();
	void FocusedOut();

signals:
	void focusedHist(int index);

protected:
	void focusInEvent(QFocusEvent *event);


public slots:
	void update_move_box(float start, float end);
	void update_release_box(float start, float end);
	void clear_box();

private slots:
	void handleOutColor();
	void handleStartColor();
	void handleEndColor();

private:
	QVBoxLayout * HistBoxLayout;
	QHBoxLayout * HistInfoLayout;
	QLabel * HistName;
	QHBoxLayout * HistColorLayout;

	QPushButton * HistOutColorBtn;
	QPushButton * HistStartColorBtn;
	QPushButton * HistEndColorBtn;

	QColor HistOutColor;
	QColor HistStartColor;
	QColor HistEndColor;

	int HistStartIdx = 0;
	int HistEndIdx = 0;


	QWidget *mWidget;
	std::vector<std::vector<long long>> *mData;
	QString mName;
	QList<QBarSet *> mBarsets;


	void updateColorBox(QPushButton *target, QColor color);
	void updateCellColor();
};

