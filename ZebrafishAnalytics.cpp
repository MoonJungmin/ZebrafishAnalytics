#include "ZebrafishAnalytics.h"

ZebrafishAnalytics::ZebrafishAnalytics()
{
	QWidget *main_widget = new QWidget;
	setCentralWidget(main_widget);

	QVBoxLayout *main_layout = new QVBoxLayout;
	main_widget->setLayout(main_layout);

	QHBoxLayout *top_toolbar_layout = new QHBoxLayout;
	top_toolbar_layout->setAlignment(Qt::AlignLeft);
	top_toolbar_layout->setMargin(1);
	QWidget *top_widget = new QWidget;
	TopToolbar *mTopToolbar = new TopToolbar(top_widget);
	mTopToolbar->initialize(top_toolbar_layout);

	QHBoxLayout *top_interface_layout = new QHBoxLayout;
	top_interface_layout->setAlignment(Qt::AlignLeft);
	top_interface_layout->setMargin(1);
	QWidget *top_interface_widget = new QWidget;
	TopInterface *mTopInterface = new TopInterface(top_interface_widget);
	mTopInterface->initialize(top_interface_layout);

	QHBoxLayout *contents_layout = new QHBoxLayout;
	contents_layout->setAlignment(Qt::AlignLeft);
	contents_layout->setMargin(1);
	QWidget *contents_widget = new QWidget;
	contents_widget->setStyleSheet("background-color:red;");

	Contents *mContents = new Contents(contents_widget);
	mContents->initialize(contents_layout, mTopInterface);


	main_layout->setMargin(0);
	main_layout->addLayout(top_toolbar_layout);
	main_layout->addLayout(top_interface_layout);
	main_layout->addLayout(contents_layout);


	createAction();
	createMenus();

	QString message = tr("A context menu is available by right-clicking");
	statusBar()->showMessage(message);
}

void ZebrafishAnalytics::createMenus() {
	fileMenu = menuBar()->addMenu("File");
	fileMenu->addAction(newProjectAction);
	fileMenu->addAction(generateBackgroundLayer);
	fileMenu->addAction(generateLabelLayer);
	fileMenu->addAction(generateSubregionLayer);
	fileMenu->addAction(preference);
}
void ZebrafishAnalytics::createAction() {
	newProjectAction = new QAction("New Analytics Project", this);
	newProjectAction->setShortcuts(QKeySequence::New);
	newProjectAction->setStatusTip(tr("Generate Background Layer"));
	connect(newProjectAction, &QAction::triggered, this, &ZebrafishAnalytics::handleNewProject);

	newProjectAction = new QAction("Open Analytics Project", this);
	newProjectAction->setShortcuts(QKeySequence::Open);
	newProjectAction->setStatusTip(tr("Open Analytics Project"));
	connect(newProjectAction, &QAction::triggered, this, &ZebrafishAnalytics::handleOpenProject);

	generateBackgroundLayer = new QAction("Generate Background Layer", this);
	generateBackgroundLayer->setShortcut(tr("CTRL+SHIFT+B"));
	generateBackgroundLayer->setStatusTip(tr("Generate Background Layer"));
	connect(generateBackgroundLayer, &QAction::triggered, this, &ZebrafishAnalytics::handleGenerateBGLayer);

	generateLabelLayer = new QAction("Generate Labeled Layer", this);
	generateLabelLayer->setShortcut(tr("CTRL+SHIFT+L"));
	generateLabelLayer->setStatusTip(tr("Generate Labeled Layer"));
	connect(generateLabelLayer, &QAction::triggered, this, &ZebrafishAnalytics::handleGenerateLBLayer);

	generateSubregionLayer = new QAction("Generate Subregion Layer", this);
	generateSubregionLayer->setShortcut(tr("CTRL+SHIFT+S"));
	generateSubregionLayer->setStatusTip(tr("Generate Subregion Layer"));
	connect(generateSubregionLayer, &QAction::triggered, this, &ZebrafishAnalytics::handleGenerateSRLayer);

	preference = new QAction("Preference", this);
	preference->setShortcut(tr("CTRL+SHIFT+P"));
	preference->setStatusTip(tr("Preference"));
	connect(preference, &QAction::triggered, this, &ZebrafishAnalytics::handlePreference);

}

void ZebrafishAnalytics::handleGenerateBGLayer() {
	DialogGenBGLayer *md = new DialogGenBGLayer(this);
	md->exec();
}

void ZebrafishAnalytics::handleGenerateLBLayer() {
	DialogGenLBLayer*md = new DialogGenLBLayer(this);
	md->exec();
}

void ZebrafishAnalytics::handleGenerateSRLayer() {
	DialogGenSRLayer *md = new DialogGenSRLayer(this);
	md->exec();
}

void ZebrafishAnalytics::handleNewProject() {
	qDebug("New Project handle");
	DialogNewProject *md = new DialogNewProject(this);
	md->exec();

}

void ZebrafishAnalytics::handleOpenProject() {
	qDebug("Open Project handle");
	//QString filters("Background Layer file (*.aproj);;");
	//QString dir = QFileDialog::getOpenFileName(this, "Open Directory", QDir::currentPath(), filters);
	//qDebug() << "Open dir";
	//qDebug() << dir;
	//if (!mGlobals.CurrentProject->ProjectStatus) {
	//	mGlobals.CurrentProject->ProjectOpen(dir);
	//}
	///*DialogNewProject *md = new DialogNewProject(mWidget);
	//md->exec();*/

	mGlobals.CurrentProject->ProjectOpen("E:/TestProject.aproj");

}

void ZebrafishAnalytics::handlePreference() {
	qDebug("Preference");
	DialogPreference *md = new DialogPreference(this);
	md->exec();
}


void ZebrafishAnalytics::handleButton() {
	qDebug("TEST Button");
}

void ZebrafishAnalytics::projectLoad() {
	qDebug("Project Load");

}