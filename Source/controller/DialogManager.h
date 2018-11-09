#pragma once

#include "DialogAddNode.h"
#include "DialogGenBGLayer.h"
#include "DialogGenLBLayer.h"
#include "DialogGenSRLayer.h"
#include "DialogNewProject.h"
#include "DialogPreference.h"


class DialogManager
{
public:
	DialogManager();
	~DialogManager();

	QDialog * mDialogGenBGLayer;
	QDialog * mDialogAddNode;
	QDialog * mDialogGenLBLayer;
	QDialog * mDialogGenSRLayer;
	QDialog * mDialogNewProject;
	QDialog * mDialogPreference;
};

