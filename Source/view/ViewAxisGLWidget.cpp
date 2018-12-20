#include "ViewAxisGLWidget.h"

ViewAxisGLWidget::ViewAxisGLWidget(QWidget *parent)
	: QOpenGLWidget(parent),
	clearColor(Qt::black),
	xRot(0),
	yRot(0),
	zRot(0),
	program(0)
{
	setFocusPolicy(Qt::StrongFocus);
	qDebug() << "create gl";
}
void ViewAxisGLWidget::setAxisIndex(int axis, int index) {
	AxisCode = axis;
	GLView_Index = index;
	qDebug() << "set axis";
	qDebug() << axis;
}

ViewAxisGLWidget::~ViewAxisGLWidget()
{
	qDebug() << "destroy gl";
	makeCurrent();

	delete program;
	doneCurrent();
}

QSize ViewAxisGLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize ViewAxisGLWidget::sizeHint() const
{
	return QSize(200, 200);
}

void ViewAxisGLWidget::rotateBy(int xAngle, int yAngle, int zAngle)
{
	xRot += xAngle;
	yRot += yAngle;
	zRot += zAngle;
	update();
}

void ViewAxisGLWidget::updateByInterface(bool scaleflag) {
	qDebug() << "updateByInterface";
	update();
}
void ViewAxisGLWidget::updateByAnotherView(bool scaleflag) {
	qDebug() << "updateByAnotherView";
	update();
}

void ViewAxisGLWidget::initializeGL()
{
	qDebug() << "init GL";
	initializeOpenGLFunctions();
	//glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//lBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);

	vshader->compileSourceFile("Resources/glsl/vshader_axisview.glsl");

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	
	fshader->compileSourceFile("Resources/glsl/fshader_axisview.glsl");

	program = new QOpenGLShaderProgram;
	
	program->addShader(vshader);
	program->addShader(fshader);
	program->link();
	program->bind();

	int blocksize = mGlobals.CurrentProject->DataBlockSize;

	con_EMTex = program->uniformLocation("em_texture");
	con_LBTex = program->uniformLocation("cell_texture");
	for (int i = 0; i<10; i++) {
		char t[20];
		sprintf(t, "subregion_texture[%d]", i);
		con_SBTex[i] = program->uniformLocation(t);
	}


	con_CellColorTex = program->uniformLocation("cell_color_texture");

	glGenTextures(1, &EMTex);
	glBindTexture(GL_TEXTURE_2D, EMTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, blocksize, blocksize, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	for (int i = 0; i < 10; ++i) {
		glGenTextures(1, &SBTex[i]);
		glBindTexture(GL_TEXTURE_2D, SBTex[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, blocksize, blocksize, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	}
	
	glGenTextures(1, &LBTex);
	glBindTexture(GL_TEXTURE_2D, LBTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, blocksize, blocksize, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);

	CellTableWidth = sqrt(mGlobals.CurrentProject->mLayerCell->MaxCellCount)+1;
	long long lvalue = 1;
	long long tablesize = lvalue * CellTableWidth * CellTableWidth;
	cell_color_data = new unsigned int[tablesize];


	//qDebug() << " Table Width : " << CellTableWidth;
	//qDebug() << " Cell Count : " << mGlobals.CurrentProject->mLayerCell->MaxCellCount;

	glGenTextures(1, &CellColorTex);
	glBindTexture(GL_TEXTURE_2D, CellColorTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, blocksize, blocksize, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
	
	//emptyTexLabel = new unsigned int[mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize];
	//memset(emptyTexLabel, 0, mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize*sizeof(unsigned int));
}


block_info ViewAxisGLWidget::calcBlockIndex() {
	block_info info;
	
	float leftTopPosX;
	float leftTopPosY;
	float rightBottomPosX;
	float rightBottomPosY;

	int tilevalue = mGlobals.CurrentProject->DataBlockSize * pow(2, (int)mGlobals.CurrentProject->ViewZoomLevel);

	int tIdx_StartX;
	int tIdx_StartY;
	int tIdx_EndX;
	int tIdx_EndY;
	int tIdx_Z;
	
	int x_count;
	int y_count;
	int z_count;
	Utils mUtil;
	mUtil.CalcXYZCount(&x_count, &y_count, &z_count, (int)mGlobals.CurrentProject->ViewZoomLevel, mGlobals.CurrentProject->DataSizeX, mGlobals.CurrentProject->DataSizeY, mGlobals.CurrentProject->DataSizeZ, mGlobals.CurrentProject->DataBlockSize);


	if (AxisCode == 1) {
		leftTopPosX = mGlobals.CurrentProject->ViewPos_X - ((float)WidgetWidth);
		leftTopPosY = mGlobals.CurrentProject->ViewPos_Y - ((float)WidgetHeight);
		rightBottomPosX = mGlobals.CurrentProject->ViewPos_X + ((float)WidgetWidth);
		rightBottomPosY = mGlobals.CurrentProject->ViewPos_Y + ((float)WidgetHeight);
		
		tIdx_Z = (int)(mGlobals.CurrentProject->ViewPos_Z);
	}
	else if (AxisCode == 2) {
		leftTopPosX = mGlobals.CurrentProject->ViewPos_Z - ((float)WidgetWidth);
		leftTopPosY = mGlobals.CurrentProject->ViewPos_Y - ((float)WidgetHeight);
		rightBottomPosX = mGlobals.CurrentProject->ViewPos_Z + ((float)WidgetWidth);
		rightBottomPosY = mGlobals.CurrentProject->ViewPos_Y + ((float)WidgetHeight);
		
		tIdx_Z = (int)(mGlobals.CurrentProject->ViewPos_X);
	}
	else if (AxisCode == 3) {
		leftTopPosX = mGlobals.CurrentProject->ViewPos_X - ((float)WidgetWidth);
		leftTopPosY = mGlobals.CurrentProject->ViewPos_Z - ((float)WidgetHeight);
		rightBottomPosX = mGlobals.CurrentProject->ViewPos_X + ((float)WidgetWidth);
		rightBottomPosY = mGlobals.CurrentProject->ViewPos_Z + ((float)WidgetHeight);
		
		tIdx_Z = (int)(mGlobals.CurrentProject->ViewPos_Y);
	}
	tIdx_StartX = (int)(leftTopPosX / tilevalue);
	tIdx_StartY = (int)(leftTopPosY / tilevalue);
	tIdx_EndX = (int)(rightBottomPosX / tilevalue) + 1;
	tIdx_EndY = (int)(rightBottomPosY / tilevalue) + 1;

	info.start_x = tIdx_StartX;
	info.end_x = tIdx_EndX;
	info.start_y = tIdx_StartY;
	info.end_y = tIdx_EndY;
	info.z = tIdx_Z;
	info.size = tilevalue;
	info.level = (int)mGlobals.CurrentProject->ViewZoomLevel;
	info.axis = AxisCode;
	return info;
}

void ViewAxisGLWidget::uploadCellColor(){
	if (mGlobals.CurrentProject->mLayerCell->CellColorGPU_On == false) {
		for (int i = 0; i < mGlobals.CurrentProject->mLayerCell->mCellList.size(); ++i) {
			unsigned int index = mGlobals.CurrentProject->mLayerCell->mCellList.at(i).index;
			if (mGlobals.CurrentProject->mLayerCell->mCellList.at(i).status) {
				cell_color_data[index] = 1;
			}
			else {
				cell_color_data[index] = 0;
			}
			//cell_color_data[index] = mGlobals.CurrentProject->mLayerCell->mCellList.at(i).color.red();
		}
		mGlobals.CurrentProject->mLayerCell->CellColorGPU_On = true;
	
		glBindTexture(GL_TEXTURE_2D, CellColorTex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CellTableWidth, CellTableWidth, GL_RED_INTEGER, GL_UNSIGNED_INT, cell_color_data);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	
}

bool ViewAxisGLWidget::bindEMLayer(block_info info) {
	int stat = mGlobals.CurrentProject->mLayerBack->checkBlockIndex(info.x, info.y, info.z, info.level, info.axis);
	if (stat == -1) {
		back_layer mblock = mGlobals.CurrentProject->mLayerBack->initializeBlock(info);
		stat = mGlobals.CurrentProject->mLayerBack->LoadBlockBySerialIndex(mblock);
	}

	if (stat != -1) {
		std::list<back_layer>::iterator iter_em = mGlobals.CurrentProject->mLayerBack->BlockList.begin();
		std::advance(iter_em, stat);
		glBindTexture(GL_TEXTURE_2D, EMTex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mGlobals.CurrentProject->DataBlockSize, mGlobals.CurrentProject->DataBlockSize, GL_RED, GL_UNSIGNED_BYTE, iter_em->data);
		glBindTexture(GL_TEXTURE_2D, 0);
		return 1;
	}
	else {
		return 0;
	}
}
bool ViewAxisGLWidget::bindCellLayer(block_info info) {
	int stat = mGlobals.CurrentProject->mLayerCell->checkBlockIndex(info.x, info.y, info.z, info.level, info.axis);
	if (stat == -1) {
		label_layer mblock = mGlobals.CurrentProject->mLayerCell->initializeBlock(info);
		stat = mGlobals.CurrentProject->mLayerCell->LoadBlockBySerialIndex(mblock);
	}

	if (stat != -1) {
		std::list<label_layer>::iterator iter_lb = mGlobals.CurrentProject->mLayerCell->BlockList.begin();
		std::advance(iter_lb, stat);

		int size = mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize;
		for (int i = 0; i < size; ++i) {
			unsigned int lb = iter_lb->data[i];
			if (lb != 0) {
				if (mGlobals.CurrentProject->mLayerCell->mCellList[lb - 1].status) {
					iter_lb->color_data[i] = 255;
				}
				else {
					iter_lb->color_data[i] = 50;
				}
			}
			else {
				iter_lb->color_data[i] = 0;
			}
		}

		glBindTexture(GL_TEXTURE_2D, LBTex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mGlobals.CurrentProject->DataBlockSize, mGlobals.CurrentProject->DataBlockSize, GL_RED_INTEGER, GL_UNSIGNED_INT, iter_lb->data);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, CellColorTex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mGlobals.CurrentProject->DataBlockSize, mGlobals.CurrentProject->DataBlockSize, GL_RED, GL_UNSIGNED_BYTE, iter_lb->color_data);
		glBindTexture(GL_TEXTURE_2D, 0);
		return true;
	}
	else {
		return false;
	}

}

int ViewAxisGLWidget::bindSubregionLayer(block_info info) {
	qDebug() << "bindSubregionLayer";
	for (int i = 0; i < 10; ++i) {
		subregion_opacity[i] = 0;
		subregion_color[i][0] = 0;
		subregion_color[i][1] = 0;
		subregion_color[i][2] = 0;
		subregion_color[i][3] = 0;
	}
	int subregion_index = 0;
	std::vector<LayerSubregion>::iterator iter;
	for (iter = mGlobals.CurrentProject->mSubregion.begin(); iter != mGlobals.CurrentProject->mSubregion.end(); ++iter) {
		if (iter->SubregionOpacity > 0 ) {
			int stat = iter->checkBlockIndex(info.x, info.y, info.z, info.level, info.axis);

			if (stat == -1) {
				subregion_layer mblock = iter->initializeBlock(info);
				stat = iter->LoadBlockBySerialIndex(mblock);
			}

			if (stat != -1) {
				std::list<subregion_layer>::iterator iter_sb = iter->BlockList.begin();
				std::advance(iter_sb, stat);
				glBindTexture(GL_TEXTURE_2D, SBTex[subregion_index]);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mGlobals.CurrentProject->DataBlockSize, mGlobals.CurrentProject->DataBlockSize, GL_RED, GL_UNSIGNED_BYTE, iter_sb->data);
				glBindTexture(GL_TEXTURE_2D, 0);
				subregion_color[subregion_index][0] = iter->SubregionColor.redF();
				subregion_color[subregion_index][1] = iter->SubregionColor.greenF();
				subregion_color[subregion_index][2] = iter->SubregionColor.blueF();
				subregion_color[subregion_index][3] = iter->SubregionColor.alphaF();

				subregion_opacity[subregion_index] = iter->SubregionOpacity * 100;
				subregion_index++;
			}

			//subregion_color[subregion_index] = iter->SubregionColor;


		}
	}
	return subregion_index;
}

void ViewAxisGLWidget::paintGL()
{
	//qDebug() << "Paint GL : " << GLView_Index << " " << WidgetWidth << "/" << WidgetHeight;
	
	QMatrix4x4 modelViewMatrix;
	//modelViewMatrix.rotate(180, 0.0, 1.0, 0.0);
	modelViewMatrix.scale(1.0/mGlobals.CurrentProject->ViewZoomLevel, -1.0 / mGlobals.CurrentProject->ViewZoomLevel, 1);
	float xt;
	float yt;
	float zt;

	if (AxisCode == 1) {
		xt = (mGlobals.CurrentProject->ViewPos_X) / WidgetHeight * (-1);
		yt = (mGlobals.CurrentProject->ViewPos_Y) / WidgetHeight * (-1);
		zt = 0;
	}
	else if (AxisCode == 2) {
		xt = mGlobals.CurrentProject->ViewPos_Z / WidgetHeight * (-1);
		//yt = (mGlobals.CurrentProject->DataSizeY - mGlobals.CurrentProject->ViewPos_Y) / WidgetHeight * (-1);
		yt = (mGlobals.CurrentProject->ViewPos_Y) / WidgetHeight * (-1);
		zt = 0;

	}
	else if (AxisCode == 3) {
		xt = (mGlobals.CurrentProject->ViewPos_X) / WidgetHeight * (-1);
		yt = (mGlobals.CurrentProject->ViewPos_Z) / WidgetHeight * (-1);
		zt = 0;
		//zt = (mGlobals.CurrentProject->ViewPos_Y) / WidgetHeight * (-1);

	}

	modelViewMatrix.translate(xt, yt, zt);
	paintState = true;
	glClearColor(0.2, 0.2, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	block_info BlockInfo = calcBlockIndex();

	qDebug() << "paint";
	
	for (int x = BlockInfo.start_x; x < BlockInfo.end_x; ++x) {
		for (int y = BlockInfo.start_y; y < BlockInfo.end_y; ++y) {
			program->bind();


			BlockInfo.x = x;
			BlockInfo.y = y;
			bool EMStat = bindEMLayer(BlockInfo);
			bool CellStat = bindCellLayer(BlockInfo);

			

			int subregion_count = bindSubregionLayer(BlockInfo);
			//uploadCellColor();

			program->setUniformValue("matrix", projMatrix*modelViewMatrix);
			program->setUniformValue("visualize_method", mGlobals.CurrentProject->VisualizeMethod_Index);
			program->setUniformValue("em_opacity", 1.0f);
			program->setUniformValue("cell_opacity", mGlobals.CurrentProject->mLayerCell->Opacity);
			program->setUniformValue("selected_color", mGlobals.CurrentProject->SelectedColor);
			program->setUniformValue("unselected_color", mGlobals.CurrentProject->UnSelectedColor);
			//glUniform1fv(glGetUniformLocation(program, "v"), 10, v);
			

			program->setUniformValueArray("subregion_opacity", subregion_opacity, 10);
			program->setUniformValueArray("subregion_color", (QVector4D*)subregion_color, 10);


			if (EMStat) {
				glActiveTexture(GL_TEXTURE0);
				glUniform1i(con_EMTex, 0);
				glBindTexture(GL_TEXTURE_2D, EMTex);
			}
			else {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			if (CellStat) {

				glActiveTexture(GL_TEXTURE1);
				glUniform1i(con_LBTex, 1);
				glBindTexture(GL_TEXTURE_2D, LBTex);

				glActiveTexture(GL_TEXTURE2);
				glUniform1i(con_CellColorTex, 2);
				glBindTexture(GL_TEXTURE_2D, CellColorTex);

			}
			else {

				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, 0);

				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
						


			
			for (int subregion_index = 0; subregion_index < subregion_count; ++subregion_index) {
				glActiveTexture(GL_TEXTURE3 + subregion_index);
				glUniform1i(con_SBTex[subregion_index], subregion_index + 3);
				glBindTexture(GL_TEXTURE_2D, SBTex[subregion_index]);
			}
			for (subregion_count; subregion_count < 10; ++subregion_count) {
				glActiveTexture(GL_TEXTURE3 + subregion_count);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
			


			drawTile(BlockInfo);	
			program->release();
		}
	}		
	
	

	
	mGlobals.CurrentProject->mLayerBack->removeBlock();
	mGlobals.CurrentProject->mLayerCell->removeBlock();

	drawCenterLine();
	paintState = false;
}
void ViewAxisGLWidget::drawTile(block_info info) {

	float posXmin = (float)(info.size * info.x) / (float)WidgetHeight;
	float posXmax = (float)(info.size * info.x + info.size) / (float)WidgetHeight;
	float posYmin = (float)(info.size * info.y) / (float)WidgetHeight;
	float posYmax = (float)(info.size * info.y + info.size) / (float)WidgetHeight;

	int texcoord_index = program->attributeLocation("TexCoord");

	glBegin(GL_QUADS);
	glVertexAttrib2f(texcoord_index, 0.0f, 0.0f);
	glVertex3f(posXmin, posYmin, 0);
	glVertexAttrib2f(texcoord_index, 1.0f, 0.0f);
	glVertex3f(posXmax, posYmin, 0);
	glVertexAttrib2f(texcoord_index, 1.0f, 1.0f);
	glVertex3f(posXmax, posYmax, 0);
	glVertexAttrib2f(texcoord_index, 0.0f, 1.0f);
	glVertex3f(posXmin, posYmax, 0);
	glEnd();

}

void ViewAxisGLWidget::drawCenterLine() {
	glLineWidth(0.5);

	glBegin(GL_LINES);
	glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
	glVertex2f(-1.0f, 0.0f);	
	glVertex2f(1.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
	glVertex2f(0.0f, -1.0f);
	glVertex2f(0.0f, 1.0f);
	glEnd();
}


void ViewAxisGLWidget::resizeGL(int width, int height)
{
	qDebug() << "resize GL";
	WidgetWidth = width;
	WidgetHeight = height;

	projMatrix.setToIdentity();
	projMatrix.viewport(0, 0, width, height);
	float ratio = (float)height / (float)width;
	

	projMatrix.ortho(0, (float)width/2/ratio, 0, height/2, 1, -1);

	emit update_view_state(mGlobals.CurrentProject->ViewPos_X, mGlobals.CurrentProject->ViewPos_Y, mGlobals.CurrentProject->ViewPos_Z, mGlobals.CurrentProject->ViewZoomLevel);
}

void ViewAxisGLWidget::mousePressEvent(QMouseEvent *event)
{
	//qDebug() << "mouse Press GL";
	lastPos = event->pos();

	if (CtrlKeyFlag) {
		qDebug() << "Ctrl key pressed";
	}

}

void ViewAxisGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	
	if (event->buttons() & Qt::LeftButton) {
		//qDebug() << "mouse Move GL";
		if (AxisCode == 1) {
			mGlobals.CurrentProject->ViewPos_X -= (float)dx * mGlobals.CurrentProject->ViewZoomLevel;
			mGlobals.CurrentProject->ViewPos_Y -= (float)dy * mGlobals.CurrentProject->ViewZoomLevel;
		}
		else if (AxisCode == 2) {
			mGlobals.CurrentProject->ViewPos_Z -= (float)dx * mGlobals.CurrentProject->ViewZoomLevel;
			mGlobals.CurrentProject->ViewPos_Y -= (float)dy * mGlobals.CurrentProject->ViewZoomLevel;
		}
		else if (AxisCode == 3) {
			mGlobals.CurrentProject->ViewPos_X -= (float)dx * mGlobals.CurrentProject->ViewZoomLevel;
			mGlobals.CurrentProject->ViewPos_Z -= (float)dy * mGlobals.CurrentProject->ViewZoomLevel;
		}

		emit update_view_state(mGlobals.CurrentProject->ViewPos_X, mGlobals.CurrentProject->ViewPos_Y, mGlobals.CurrentProject->ViewPos_Z, mGlobals.CurrentProject->ViewZoomLevel);
		emit updateAll(GLView_Index, false);
		update();		
	}

	lastPos = event->pos();
}

void ViewAxisGLWidget::mouseReleaseEvent(QMouseEvent * /* event */)
{
	//qDebug() << "mouse Release GL";
	emit clicked();
}

void ViewAxisGLWidget::wheelEvent(QWheelEvent *event){
	float delta = (float)event->angleDelta().y();

	if (delta > 0) {
		mGlobals.CurrentProject->ViewZoomLevel -= 0.05;
		emit update_view_state(mGlobals.CurrentProject->ViewPos_X, mGlobals.CurrentProject->ViewPos_Y, mGlobals.CurrentProject->ViewPos_Z, mGlobals.CurrentProject->ViewZoomLevel);
		emit updateAll(GLView_Index, true);
		update();
	}
	else {
		mGlobals.CurrentProject->ViewZoomLevel += 0.05;
		emit update_view_state(mGlobals.CurrentProject->ViewPos_X, mGlobals.CurrentProject->ViewPos_Y, mGlobals.CurrentProject->ViewPos_Z, mGlobals.CurrentProject->ViewZoomLevel);
		emit updateAll(GLView_Index, true);
		update();
	}
}

void ViewAxisGLWidget::keyPressEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Left) {
		qDebug() << "left";
	}
	else if (event->key() == Qt::Key_Right) {
		qDebug() << "right";
	}
	else if (event->key() == Qt::Key_Up) {

		if (AxisCode == 1) {
			if (mGlobals.CurrentProject->ViewPos_Z > 0) {
				mGlobals.CurrentProject->ViewPos_Z -= 1.0;
			}
		}
		else if (AxisCode == 2) {
			if (mGlobals.CurrentProject->ViewPos_X > 0) {
				mGlobals.CurrentProject->ViewPos_X -= 1.0;
			}
		}
		else if (AxisCode == 3) {
			if (mGlobals.CurrentProject->ViewPos_Y > 0) {
				mGlobals.CurrentProject->ViewPos_Y -= 1.0;
			}
		}
		emit update_view_state(mGlobals.CurrentProject->ViewPos_X, mGlobals.CurrentProject->ViewPos_Y, mGlobals.CurrentProject->ViewPos_Z, mGlobals.CurrentProject->ViewZoomLevel);
		emit updateAll(GLView_Index, false);
		update();
	}
	else if (event->key() == Qt::Key_Down) {
		if (AxisCode == 1) {
			if (mGlobals.CurrentProject->ViewPos_Z > 0) {
				mGlobals.CurrentProject->ViewPos_Z += 1.0;
			}
		}
		else if (AxisCode == 2) {
			if (mGlobals.CurrentProject->ViewPos_X > 0) {
				mGlobals.CurrentProject->ViewPos_X += 1.0;
			}
		}
		else if (AxisCode == 3) {
			if (mGlobals.CurrentProject->ViewPos_Y > 0) {
				mGlobals.CurrentProject->ViewPos_Y += 1.0;
			}
		}
		emit update_view_state(mGlobals.CurrentProject->ViewPos_X, mGlobals.CurrentProject->ViewPos_Y, mGlobals.CurrentProject->ViewPos_Z, mGlobals.CurrentProject->ViewZoomLevel);
		emit updateAll(GLView_Index, false);
		update();
	}
	else if (event->key() == Qt::Key_Control) {
		CtrlKeyFlag = true;
	}
}


void ViewAxisGLWidget::keyReleaseEvent(QKeyEvent* event) {
	if (event->key() == Qt::Key_Control) {
		CtrlKeyFlag = false;
	}
	
}

