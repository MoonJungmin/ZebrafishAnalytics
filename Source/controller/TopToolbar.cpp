#include "TopToolbar.h"

TopToolbar::TopToolbar(QWidget *parent)
	: QObject(parent)
{
	mWidget = parent;
}

void TopToolbar::initialize(QLayout *parent_layout) {
	QPushButton *m_button1 = new QPushButton;
	m_button1->setIcon(QIcon("Resources/icon_new.png"));
	m_button1->setIconSize(QSize(20, 20));
	m_button1->setFixedSize(QSize(30, 30));
	connect(m_button1, SIGNAL(released()), this, SLOT(handleNewProject()));

	QPushButton *m_button2 = new QPushButton;
	m_button2->setIcon(QIcon("Resources/icon_save.png"));
	m_button2->setIconSize(QSize(20, 20));
	m_button2->setFixedSize(QSize(30, 30));
	connect(m_button2, SIGNAL(released()), this, SLOT(handleSaveProject()));

	QPushButton *m_button3 = new QPushButton;
	m_button3->setIcon(QIcon("Resources/icon_open.png"));
	m_button3->setIconSize(QSize(20, 20));
	m_button3->setFixedSize(QSize(30, 30));
	connect(m_button3, SIGNAL(released()), this, SLOT(handleOpenProject()));

	QPushButton *m_button4 = new QPushButton;
	m_button4->setIcon(QIcon("Resources/icon_addnode.png"));
	m_button4->setIconSize(QSize(20, 20));
	m_button4->setFixedSize(QSize(30, 30));
	connect(m_button4, SIGNAL(released()), this, SLOT(handleAddNode()));

	QPushButton *m_button5 = new QPushButton("M", mWidget);
	m_button5->setFixedSize(QSize(30, 30));
	connect(m_button5, SIGNAL(released()), mWidget, SLOT(handleButton()));

	QPushButton *m_button6 = new QPushButton("M", mWidget);
	m_button6->setFixedSize(QSize(30, 30));
	connect(m_button6, SIGNAL(released()), mWidget, SLOT(handleButton()));

	parent_layout->addWidget(m_button1);
	parent_layout->addWidget(m_button2);
	parent_layout->addWidget(m_button3);
	parent_layout->addWidget(m_button4);
	parent_layout->addWidget(m_button5);
	parent_layout->addWidget(m_button6);
}

TopToolbar::~TopToolbar()
{
}

void TopToolbar::handleNewProject() {
	qDebug("New Project handle");
	DialogNewProject *md = new DialogNewProject(mWidget);
	md->exec();
}

void TopToolbar::handleSaveProject() {
	qDebug("Save Project handle");
	if (mGlobals.CurrentProject->ProjectStatus) {
		mGlobals.CurrentProject->ProjectSave(mGlobals.CurrentProject->ProjectPath);
	}
	else {
		QMessageBox::critical(0, QString::fromStdString("Project save error"), QString::fromStdString("Project save error"));
	}
	/*DialogNewProject *md = new DialogNewProject(mWidget);
	md->exec();*/
}
void TopToolbar::handleOpenProject() {
	qDebug("Open Project handle");
	QString filters("Background Layer file (*.aproj);;");
	QString dir = QFileDialog::getOpenFileName(mWidget, "Open Directory", QDir::currentPath(), filters);
	qDebug() << "Open dir";
	qDebug() << dir;
	if (!mGlobals.CurrentProject->ProjectStatus) {
		mGlobals.CurrentProject->ProjectOpen(dir);
	}
	/*DialogNewProject *md = new DialogNewProject(mWidget);
	md->exec();*/
}
void TopToolbar::handleAddNode() {
	qDebug("Open Add node handle");
	DialogAddNode *md = new DialogAddNode(mWidget);
	md->exec();
}
