#pragma once


#include <QMainWindow>

#include "Source/controller/TopToolbar.h"
#include "Source/controller/TopInterface.h"
#include "Source/controller/Contents.h"

#include "Source/controller/DialogNewProject.h"
#include "Source/controller/DialogGenBGLayer.h"
#include "Source/controller/DialogGenLBLayer.h"
#include "Source/controller/DialogGenSRLayer.h"
#include "Source/controller/DialogPreference.h"

#include "Source/global.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class ZebrafishAnalytics : public QMainWindow
{
	Q_OBJECT

public:
	ZebrafishAnalytics();



protected:

private slots:
	void handleButton();
	void handleNewProject();
	void handleOpenProject();
	void handleGenerateBGLayer();
	void handleGenerateLBLayer();
	void handleGenerateSRLayer();
	void handlePreference();
	void projectLoad();


private:
	void createMenus();
	void createAction();
	QMenu * fileMenu;
	QAction *newProjectAction;
	QAction *generateBackgroundLayer;
	QAction *generateLabelLayer;
	QAction *generateSubregionLayer;

	QAction *preference;

};
