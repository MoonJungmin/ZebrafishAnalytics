#include "ViewGraphEditor.h"

ViewGraphEditor::ViewGraphEditor(QWidget *parent)
	: QWidget(parent)
{
	mWidget = parent;
	//setStyleSheet("QMessageBox{background: rgb(55,55,55);  border: none;font-family: Arial; font-style: normal;  font-size: 10pt; color: #ffffff ; }, QMessageBox QLabel{background: rgb(55,55,55);  border: none;font-family: Arial; font-style: normal;  font-size: 10pt; color: #ffffff ; }");
	
}

ViewGraphEditor::~ViewGraphEditor()
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
	b->setBlockFlagAndSize(QNEBlock::OriginBlock, 180, 215, data_node_color, this);
	b->setInputDataOrigin(&mGlobals.CurrentProject->mLayerCell->mCellList);
	b->addPort("Origin", 0, 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Origin Data bucket", 0, 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, 0, QNEPort::DataWidgetPort, 0, QNEPort::Center);
	b->addPort("", 0, 0, QNEPort::DataSizePort, 0, QNEPort::Left);
	b->addPort("", 0, 0, QNEPort::ToolBoxPort, 0, QNEPort::Right);
	b->addOutputPort("");
}

void ViewGraphEditor::addDataBucket(std::string name) {
	QNEBlock *b = new QNEBlock(0);
	GraphScene->addItem(b);
	b->setBlockFlagAndSize(QNEBlock::DataBlock, 180, 215, data_node_color, this);
	b->addPort(name.c_str(), 0, 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Sub Data bucket", 0, 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, 0, QNEPort::DataWidgetPort, 0, QNEPort::Center);
	b->addPort("", 0, 0, QNEPort::DataSizePort, 0, QNEPort::Left);
	b->addPort("", 0, 0, QNEPort::ToolBoxPort, 0, QNEPort::Right);
	b->addInputPort("");
	b->addOutputPort("");
	connect(b->mBlock, SIGNAL(deleteNode(QNEBlock*)), this, SLOT(deleteNode(QNEBlock*)));
}
void ViewGraphEditor::addSubregion(std::string name) {
	QNEBlock *b = new QNEBlock(0);
	GraphScene->addItem(b);
	b->setBlockFlagAndSize(QNEBlock::SubregionBlock, 250, 150, operation_node_color, this);
	b->addPort(name.c_str(), 0, 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Operation(Subregion)", 0, 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, 0, QNEPort::DataSizePort, 0, QNEPort::Left);
	b->addPort("", 0, 0, QNEPort::SubregionDropdownPort, 0, QNEPort::Center);
	b->addPort("", 0, 0, QNEPort::ToolBoxPort, 0, QNEPort::Right);
	b->addInputPort("");
	b->addOutputPort("");
	connect(b->mBlock, SIGNAL(deleteNode(QNEBlock*)), this, SLOT(deleteNode(QNEBlock*)));
}
void ViewGraphEditor::addSet(std::string name) {

}
void ViewGraphEditor::addFeature(std::string name) {
	QNEBlock *b = new QNEBlock(0);
	GraphScene->addItem(b);
	b->setBlockFlagAndSize(QNEBlock::FeatureBlock, 320, 240, operation_node_color, this);
	b->addPort(name.c_str(), 0, 0, QNEPort::NamePort, 0, QNEPort::Left);
	b->addPort("Operation(Feature)", 0, 0, QNEPort::TypePort, 0, QNEPort::Left);
	b->addPort("", 0, 0, QNEPort::FeatureDropdownPort, 0, QNEPort::Center);
	b->addPort("", 0, 0, QNEPort::DataSizePort, 0, QNEPort::Left);
	b->addPort("", 0, 0, QNEPort::FeatureWidgetPort, 0, QNEPort::Center);
	b->addPort("", 0, 0, QNEPort::ToolBoxPort, 0, QNEPort::Right);
	b->addInputPort("");
	b->addOutputPort("");
	connect(b->mBlock, SIGNAL(deleteNode(QNEBlock*)), this, SLOT(deleteNode(QNEBlock*)));
}
void ViewGraphEditor::addSimilarity(std::string name) {

}


void ViewGraphEditor::addNode(int type, QString name) {
	if (type == 0) {
		qDebug() << "Sub Data bucket / " << name;
		addDataBucket(name.toStdString());
	}
	else if (type == 1) {
		qDebug() << "Feature / " << name;
		if (mGlobals.CurrentProject->mFeature.size() == 0) {
			QMessageBox::warning(0, "Error", "Please add a feature first");
		}
		else {
			addFeature(name.toStdString());
		}
	}
	else if (type == 2) {
		qDebug() << "Subregion / " << name;
		if (mGlobals.CurrentProject->mSubregion.size() == 0) {
			QMessageBox::warning(0, "Error", "Please add a subregion first");
		}
		else {
			addSubregion(name.toStdString());
		}
	}
	else if (type == 3) {
		qDebug() << "Set / " << name;
		addSet(name.toStdString());
	}
	else if (type == 4) {
		qDebug() << "Similarity / " << name;
		addSimilarity(name.toStdString());
	}

}


void ViewGraphEditor::deleteNode(QNEBlock *ptr) {
	qDebug() << "deleteNode";
		
	
	foreach(QNEPort *port, ptr->ports()) {
		if (port->portAlign() == QNEPort::Input) {
			foreach(QNEConnection *conn, port->connections()) {
				GraphScene->removeItem((QGraphicsItem*)conn);
			}
		}
		if (port->portAlign() == QNEPort::Output) {
			foreach(QNEConnection *conn, port->connections()) {
				QNEBlock *next = conn->port2()->m_block;
				next->mBlock->CellIndexListInput.clear();
				next->mBlock->updatedInputList();
				GraphScene->removeItem((QGraphicsItem*)conn);
			}
			
		}
	}

	GraphScene->removeItem((QGraphicsItem*)ptr);
}

void ViewGraphEditor::updateSubregionList() {
	qDebug() << "Subregion updated";
	foreach(QGraphicsItem *ptr, GraphScene->items()) {
		if (ptr->type() == QNEBlock::Type) {
			QNEBlock *temp = (QNEBlock *)ptr;
			if (temp->mBlockFlags == QNEBlock::SubregionBlock) {
				temp->mBlock->updatedSubregionList();
			}
		}
	}
}

void ViewGraphEditor::updateFeatureList() {
	qDebug() << "Feature updated";
	foreach(QGraphicsItem *ptr, GraphScene->items()) {
		if (ptr->type() == QNEBlock::Type) {
			QNEBlock *temp = (QNEBlock *)ptr;
			if (temp->mBlockFlags == QNEBlock::FeatureBlock) {
				temp->mBlock->updatedFeatureList();
			}
		}
	}
}