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
	qDebug() << "minimumSizeHint";
	return QSize(50, 50);
}

QSize ViewAxisGLWidget::sizeHint() const
{
	qDebug() << "sizeHint";
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

	QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);

	vshader->compileSourceFile("Resources/glsl/vshader.glsl");

	QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
	
	fshader->compileSourceFile("Resources/glsl/fshader.glsl");

	program = new QOpenGLShaderProgram;
	
	program->addShader(vshader);
	program->addShader(fshader);
	program->link();
	program->bind();

	int blocksize = mGlobals.CurrentProject->DataBlockSize;

	con_EMTex = program->uniformLocation("em_texture");
	con_LBTex = program->uniformLocation("lb_texture");
	con_CellColorTex = program->uniformLocation("cell_color_texture");

	glGenTextures(1, &EMTex);
	glBindTexture(GL_TEXTURE_2D, EMTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, blocksize, blocksize, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glGenTextures(1, &LBTex);
	glBindTexture(GL_TEXTURE_2D, LBTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, blocksize, blocksize, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, 0);

	CellTableWidth = sqrt(mGlobals.CurrentProject->mAnalytics->MaxCellCount)+1;
	long long lvalue = 1;
	long long tablesize = lvalue * CellTableWidth * CellTableWidth * 3;
	cell_color_data = new unsigned char[tablesize];


	qDebug() << " Table Width : " << CellTableWidth;
	qDebug() << " Cell Count : " << mGlobals.CurrentProject->mAnalytics->MaxCellCount;

	glGenTextures(1, &CellColorTex);
	glBindTexture(GL_TEXTURE_2D, CellColorTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CellTableWidth, CellTableWidth, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	
	emptyTexLabel = new unsigned int[mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize];
	memset(emptyTexLabel, 0, mGlobals.CurrentProject->DataBlockSize * mGlobals.CurrentProject->DataBlockSize*sizeof(unsigned int));
}


std::vector<int> ViewAxisGLWidget::calcBlockIndexXY() {
	std::vector<int> LoadIndexList;
	
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


	/*
	
	qDebug() << tIdx_StartX;
	qDebug() << tIdx_StartY;
	qDebug() << tIdx_EndX;
	qDebug() << tIdx_EndY;*/

	for (int x = tIdx_StartX; x < tIdx_EndX; x++) {
		for (int y = tIdx_StartY; y < tIdx_EndY; y++) {
			int stat;
			if (AxisCode == 1) {
				stat = mGlobals.CurrentProject->mLayerBack->checkBlockIndex(x, y, tIdx_Z, (int)mGlobals.CurrentProject->ViewZoomLevel, AxisCode);
			}
			else if (AxisCode == 2) {
				stat = mGlobals.CurrentProject->mLayerBack->checkBlockIndex(tIdx_Z, y, x, (int)mGlobals.CurrentProject->ViewZoomLevel, AxisCode);
			}
			else if (AxisCode == 3) {
				stat = mGlobals.CurrentProject->mLayerBack->checkBlockIndex(x, tIdx_Z, y, (int)mGlobals.CurrentProject->ViewZoomLevel, AxisCode);
			}
			
			if (stat == -1) {
				back_layer tempBlock1;
				if (AxisCode == 1) {
					tempBlock1.index_x = x;
					tempBlock1.index_y = y;
					tempBlock1.index_z = tIdx_Z;
				}
				else if (AxisCode == 2) {
					tempBlock1.index_x = tIdx_Z;
					tempBlock1.index_y = y;
					tempBlock1.index_z = x;
				}
				else if (AxisCode == 3) {
					tempBlock1.index_x = x;
					tempBlock1.index_y = tIdx_Z ;
					tempBlock1.index_z = y;
				}

				tempBlock1.pos_x = (tilevalue * x);
				
				tempBlock1.pos_y = (tilevalue * (y));
				
				qDebug() << tempBlock1.index_x << " " << tempBlock1.index_y << " " << tempBlock1.index_z;
				
				tempBlock1.pos_z = tIdx_Z;

				tempBlock1.size_x = tilevalue;
				tempBlock1.size_y = tilevalue;
				tempBlock1.size_z = tilevalue;

				tempBlock1.level = (int)mGlobals.CurrentProject->ViewZoomLevel;
				tempBlock1.load_axis_code = AxisCode;

				label_layer tempBlock2;
				tempBlock2.index_x = tempBlock1.index_x;
				tempBlock2.index_y = tempBlock1.index_y;
				tempBlock2.index_z = tempBlock1.index_z;

				tempBlock2.pos_x = tempBlock1.pos_x;
				tempBlock2.pos_y = tempBlock1.pos_y;
				tempBlock2.pos_z = tempBlock1.pos_z;

				tempBlock2.size_x = tempBlock1.size_x;
				tempBlock2.size_y = tempBlock1.size_y;
				tempBlock2.size_z = tempBlock1.size_z;

				tempBlock2.level = tempBlock1.level;

				tempBlock2.load_axis_code = tempBlock1.load_axis_code;

				int status = mGlobals.CurrentProject->mLayerBack->LoadBlockBySerialIndex(tempBlock1);
				if (status != -1) {
					mGlobals.CurrentProject->mLayerLabel->LoadBlockBySerialIndex(tempBlock2);
					qDebug() << "status : " << status;
					LoadIndexList.push_back(status);
				}
			}
			else {
				LoadIndexList.push_back(stat);
			}
			
			
			
		}
	}
	return LoadIndexList;

	qDebug() << "Tile Done";
}

void ViewAxisGLWidget::uploadCellColor(){
	if (mGlobals.CurrentProject->mAnalytics->CellColorGPU_On == false) {
		for (int i = 0; i < mGlobals.CurrentProject->mAnalytics->mCellList.size(); ++i) {
			//qDebug() << mGlobals.CurrentProject->mAnalytics->mCellList.at(i).color.red() << " " << mGlobals.CurrentProject->mAnalytics->mCellList.at(i).color.green() << " " << mGlobals.CurrentProject->mAnalytics->mCellList.at(i).color.blue();
			cell_color_data[3*i] = mGlobals.CurrentProject->mAnalytics->mCellList.at(i).color.red();
			cell_color_data[3*i+1] = mGlobals.CurrentProject->mAnalytics->mCellList.at(i).color.green();
			cell_color_data[3*i+2] = mGlobals.CurrentProject->mAnalytics->mCellList.at(i).color.blue();
		}
		mGlobals.CurrentProject->mAnalytics->CellColorGPU_On = true;
	
		glBindTexture(GL_TEXTURE_2D, CellColorTex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, CellTableWidth, CellTableWidth, GL_RGB, GL_UNSIGNED_BYTE, cell_color_data);
		glBindTexture(GL_TEXTURE_2D, 0);

	}
	
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
	std::vector<int> IndexList = calcBlockIndexXY();

	
	program->bind();
	program->setUniformValue("matrix", projMatrix*modelViewMatrix);
	program->setUniformValue("visualize_method", mGlobals.CurrentProject->VisualizeMethod_Index);
	program->setUniformValue("cell_color_table_width", CellTableWidth);
	uploadCellColor();
  
	/*qDebug() << "size";
	qDebug() << mGlobals.CurrentProject->mLayerBack->BlockList.size();
*/
	for (int i = 0; i < IndexList.size(); ++i) {
		
		std::list<back_layer>::iterator iter_em = mGlobals.CurrentProject->mLayerBack->BlockList.begin();
		std::list<label_layer>::iterator iter_lb = mGlobals.CurrentProject->mLayerLabel->BlockList.begin();;

		std::advance(iter_em, IndexList.at(i));
		std::advance(iter_lb, IndexList.at(i));
		
		int blocksize = mGlobals.CurrentProject->DataBlockSize;
			
		glBindTexture(GL_TEXTURE_2D, EMTex);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, blocksize, blocksize, GL_RED, GL_UNSIGNED_BYTE, iter_em->data);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (iter_lb->status) {
			glBindTexture(GL_TEXTURE_2D, LBTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, blocksize, blocksize, GL_RED_INTEGER, GL_UNSIGNED_INT, iter_lb->data);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		else {
			glBindTexture(GL_TEXTURE_2D, LBTex);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, blocksize, blocksize, GL_RED_INTEGER, GL_UNSIGNED_INT, emptyTexLabel);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		//qDebug() << "size";
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(con_EMTex, 0);
		glBindTexture(GL_TEXTURE_2D, EMTex);

		glActiveTexture(GL_TEXTURE1);
		glUniform1i(con_LBTex, 1);
		glBindTexture(GL_TEXTURE_2D, LBTex);

		glActiveTexture(GL_TEXTURE2);
		glUniform1i(con_CellColorTex, 2);
		glBindTexture(GL_TEXTURE_2D, CellColorTex);

				
		float posXmin = (float)(iter_em->pos_x) / (float)WidgetHeight;
		float posXmax = (float)(iter_em->pos_x + iter_em->size_x) / (float)WidgetHeight;
		float posYmin = (float)(iter_em->pos_y) / (float)WidgetHeight;
		float posYmax = (float)(iter_em->pos_y + iter_em->size_y) / (float)WidgetHeight;

		
		/*qDebug() << iter_em->pos_x;
		qDebug() << iter_em->pos_y;
		qDebug() << iter_em->size_x;
		qDebug() << iter_em->size_y;
*/
		//qDebug() << posXmin;
		//qDebug() << posXmax;
		//qDebug() << posYmin;
		//qDebug() << posYmax;



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


	program->release();
	mGlobals.CurrentProject->mLayerBack->removeBlock();
	mGlobals.CurrentProject->mLayerLabel->removeBlock();

	drawCenterLine();
	paintState = false;
}
void ViewAxisGLWidget::drawTile() {

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
		mGlobals.CurrentProject->ViewZoomLevel -= 0.01;
		emit update_view_state(mGlobals.CurrentProject->ViewPos_X, mGlobals.CurrentProject->ViewPos_Y, mGlobals.CurrentProject->ViewPos_Z, mGlobals.CurrentProject->ViewZoomLevel);
		emit updateAll(GLView_Index, true);
		update();
	}
	else {
		mGlobals.CurrentProject->ViewZoomLevel += 0.01;
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
}

