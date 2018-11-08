#include "ViewGraphEditor.h"

ViewGraphEditor::ViewGraphEditor(QWidget *parent)
	: QObject(parent)
{
}

void ViewGraphEditor::initialize(QLayout *parent_layout) {
	GraphScene = new QGraphicsScene();
	GraphView = new QGraphicsView();
	GraphView->setScene(GraphScene);
	GraphView->setFixedSize(1500, 870 * 0.40);
	GraphView->setRenderHint(QPainter::Antialiasing, true);

	GraphEditor = new QNodesEditor(this);
	GraphEditor->install(GraphScene);



	parent_layout->addWidget(GraphView);
}
void ViewGraphEditor::addOrigin() {
	QNEBlock *b = new QNEBlock(0);
	GraphScene->addItem(b);
	
	b->setBlockFlagAndSize(QNEBlock::OriginBlock, 180, 120);
	b->setInputData(&mGlobals.CurrentProject->mAnalytics->mCellList);
	b->addPort("Origin", 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Origin Data bucket", 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, QNEPort::DataWidgetPort, 0, QNEPort::Center);
	b->addPort("", 0, QNEPort::DataSizePort, 0, QNEPort::Left);
	b->addPort("", 0, QNEPort::AnnotationPort, 0, QNEPort::Left);
	
	b->addOutputPort("");
}


ViewGraphEditor::~ViewGraphEditor()
{
}
