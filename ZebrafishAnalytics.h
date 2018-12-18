#pragma once


#include <QMainWindow>
#include "Source/controller/TopToolbar.h"
#include "Source/controller/TopInterface.h"
#include "Source/controller/Contents.h"
#include "Source/controller/Dialog/DialogManager.h"

#include "Source/global.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;

class TopToolbar;
class TopInterface;
class Contents;


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
	void handleProjectOn();


private:
	void connectAll();
	void createMenus();
	void createAction();
	QMenu * fileMenu;
	QAction *newProjectAction;
	QAction *generateBackgroundLayer;
	QAction *generateLabelLayer;
	QAction *generateSubregionLayer;
	QAction *preference;


	TopToolbar *mTopToolbar;
	TopInterface *mTopInterface;
	Contents *mContents;


};
