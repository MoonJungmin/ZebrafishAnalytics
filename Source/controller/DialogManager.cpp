#include "DialogManager.h"



DialogManager::DialogManager()
{
}

DialogManager::DialogManager(QWidget *parent)
{
	mWidget = parent;
}


DialogManager::~DialogManager()
{
}


void DialogManager::initializeDialog() {
	mDialogAddNode = new DialogAddNode(mWidget);
	mDialogGenBGLayer = new DialogGenBGLayer(mWidget);
	mDialogGenLBLayer = new DialogGenLBLayer(mWidget);
	mDialogGenSRLayer = new DialogGenSRLayer(mWidget);
	mDialogNewProject = new DialogNewProject(mWidget);
	mDialogPreference = new DialogPreference(mWidget);
}

