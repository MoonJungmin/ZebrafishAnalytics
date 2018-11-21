#pragma once

#include <QObject>
#include <QtWidgets>
#include <QPushButton>
#include <QColor>
#include <QColorDialog>
#include <QDebug>

#include "Source/Utils.h"
#include "qneblock.h"

class QNEBlock;

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
	QWidget *CenterBox;

	QPushButton *BucketColorBtn;
	QPushButton *CloseBtn;
	QPushButton *AnnotationBtn;

	std::list<unsigned int> CellIndexListInput;
	std::list<unsigned int> CellIndexListOutput;


private slots:
	void handleBlockColorBtn();
	void handleCloseBtn();
	void handleAnnotationBtn();


private:
	QNEBlock * m_block;

	void generateToolBox(int aflag);
	void generateCenterBox(int width, int height);
	void updateColorBox(QPushButton *target, QColor color);
};
