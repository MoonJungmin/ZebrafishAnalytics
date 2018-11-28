#pragma once

#include <QObject>
#include <QtWidgets>
#include <QLabel>
#include <QSizePolicy>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QColor>
#include <QDebug>
#include <QMessageBox>


#include "Source/controller/node/qneblock.h"
#include "Source/controller/node/qneconnection.h"
#include "Source/controller/node/qnodeseditor.h"
#include "Source/controller/node/qneport.h"


#include "Source/global.h"
#include "Source/Utils.h"



class ViewGraphEditor : public QWidget
{
	Q_OBJECT

public slots:
	void addNode(int type, QString name);
	void deleteNode(QNEBlock *ptr);
	void updateSubregionList();
	void updateFeatureList();

public:
	ViewGraphEditor(QWidget *parent);
	~ViewGraphEditor();
	void initialize(QLayout *parent_layout);
	void addOrigin();

	QGraphicsScene* GraphScene;
	QGraphicsView *GraphView;
	QNodesEditor *GraphEditor;




private:
	QWidget * mWidget;

	QColor data_node_color = QColor("#3A79A0");
	QColor operation_node_color = QColor("#555555");

	void addDataBucket(std::string name);
	void addSubregion(std::string name);
	void addSet(std::string name);
	void addFeature(std::string name);
	void addSimilarity(std::string name);
};
