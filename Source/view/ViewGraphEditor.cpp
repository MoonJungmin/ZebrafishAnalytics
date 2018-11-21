#include "ViewGraphEditor.h"

ViewGraphEditor::ViewGraphEditor(QWidget *parent)
	: QWidget(parent)
{
	mWidget = parent;
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


void ViewGraphEditor::addNode(int type, QString name) {
	if (type == 0) {
		qDebug() << "Sub Data bucket / " << name;
		addDataBucket(name.toStdString());
;	}
	else if (type == 1) {
		qDebug() << "Feature / " << name;
		addFeature(name.toStdString());
	}
	else if (type == 2) {
		qDebug() << "Subregion / " << name;
		addSubregion(name.toStdString());
	}
	else if (type == 3) {
		qDebug() << "Set / " << name;
		addSet(name.toStdString());
	}
	else if (type == 4) {
		qDebug() << "Similarity / " << name;
		addSimilarity(name.toStdString());
	}
	
	//QStringList node_type_list = { "Sub Data bucket", "Operation(Feature)", "Operation(Subregion)", "Operation(Set)", "Operation(Similarity)" };

}
void ViewGraphEditor::deleteNode(QNEBlock *ptr) {
	GraphScene->removeItem((QGraphicsItem*)ptr);
}


void ViewGraphEditor::addOrigin() {
	QNEBlock *b = new QNEBlock(0);
	GraphScene->addItem(b);
	b->setBlockFlagAndSize(QNEBlock::OriginBlock, 180, 120, data_node_color, this);
	b->setInputData(&mGlobals.CurrentProject->mLayerCell->mCellList);
	b->addPort("Origin", 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Origin Data bucket", 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, QNEPort::DataWidgetPort, 0, QNEPort::Center);
	b->addPort("", 0, QNEPort::DataSizePort, 0, QNEPort::Left);
	b->addPort("", 0, QNEPort::ToolBoxPort, 0, QNEPort::Right);
	b->addOutputPort("");



	//QWidget *toolbox_widget = new QWidget(this);
	//temp->toolbox = 
	//mNodeList.push_back(temp);
	//connect(b->BucketColorBtn, SIGNAL(clicked()), this, SLOT(handleBlockColor()));
}



void ViewGraphEditor::addDataBucket(std::string name) {
	QNEBlock *b = new QNEBlock(0);
	GraphScene->addItem(b);
	b->setBlockFlagAndSize(QNEBlock::DataBlock, 180, 120, data_node_color, this);
	b->addPort(name.c_str(), 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Sub Data bucket", 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, QNEPort::DataWidgetPort, 0, QNEPort::Center);
	b->addPort("", 0, QNEPort::DataSizePort, 0, QNEPort::Left);
	b->addPort("", 0, QNEPort::ToolBoxPort, 0, QNEPort::Right);
	b->addInputPort("");
	b->addOutputPort("");


	connect(b->mBlock, SIGNAL(deleteNode(QNEBlock*)), this, SLOT(deleteNode(QNEBlock*)));
	//connect(b->BucketColorBtn,)
}
void ViewGraphEditor::addSubregion(std::string name) {
	QNEBlock *b = new QNEBlock(0);
	GraphScene->addItem(b);
	b->setBlockFlagAndSize(QNEBlock::SubregionBlock, 250, 80, operation_node_color, this);
	b->addPort(name.c_str(), 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Operation(Subregion)", 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, QNEPort::SubregionWidgetPort, 0, QNEPort::Center);
	b->addPort("", 0, QNEPort::SubregionDropdownPort, 0, QNEPort::Center);
	b->addPort("", 0, QNEPort::ToolBoxPort, 0, QNEPort::Right);
	b->addInputPort("");
	b->addOutputPort("");
	
	//mNodeList.push_back(b);
}
void ViewGraphEditor::addSet(std::string name) {

}
void ViewGraphEditor::addFeature(std::string name) {
	QNEBlock *b = new QNEBlock(0);
	GraphScene->addItem(b);
	b->setBlockFlagAndSize(QNEBlock::FeatureBlock, 320, 90, operation_node_color, this);
	b->addPort(name.c_str(), 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Operation(Feature)", 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, QNEPort::FeatureWidgetPort, 0, QNEPort::Center);
	b->addPort("", 0, QNEPort::FeatureDropdownPort, 0, QNEPort::Center);
	b->addPort("", 0, QNEPort::ToolBoxPort, 0, QNEPort::Right);
	b->addInputPort("");
	b->addOutputPort("");
}
void ViewGraphEditor::addSimilarity(std::string name) {

}


ViewGraphEditor::~ViewGraphEditor()
{
}

