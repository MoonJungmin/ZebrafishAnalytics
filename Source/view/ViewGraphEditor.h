#pragma once

#include <QObject>
#include <QtWidgets>
#include <QLabel>
#include <QSizePolicy>
#include <QGraphicsScene>
#include <QFileDialog>

#include "Source/controller/node/qneblock.h"
#include "Source/controller/node/qnodeseditor.h"
#include "Source/controller/node/qneport.h"


#include "Source/global.h"
#include "Source/Utils.h"


class ViewGraphEditor : public QObject
{
	Q_OBJECT

public:
	ViewGraphEditor(QWidget *parent);
	~ViewGraphEditor();
	void initialize(QLayout *parent_layout);
	void addOrigin();

	QGraphicsScene* GraphScene;
	QGraphicsView *GraphView;
	QNodesEditor *GraphEditor;
};
