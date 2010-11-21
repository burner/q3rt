#include "openglscene.h"

#include <QtGui>
#include <QGLFormat>
#include <QGLWidget>
#include <iostream>
#include <python3.1/Python.h>

#include <string>
#include <fstream>

void execPyScript(const std::string fileName) {
	std::fstream inFile;
	inFile.open(fileName.c_str());
	if(!inFile) {
		std::cout<<"couldn't open file "<<fileName<<std::endl;
		return;
	}
	std::string tmp;
	Py_Initialize();
	while(getline(inFile, tmp)) {
		PyRun_SimpleString(tmp.c_str());
	}
	Py_Finalize();
}

class GraphicsView : public QGraphicsView {
	public:
		GraphicsView() {
			setWindowTitle(tr("q3rt"));
		}
	
	protected:
		void resizeEvent(QResizeEvent *event) {
			if (scene())
				scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
			QGraphicsView::resizeEvent(event);
		}
};

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	execPyScript("pytest.py");

	GraphicsView view;
	QGLFormat fmt(QGL::SampleBuffers);
	fmt.setProfile(QGLFormat::CoreProfile);
	fmt.setVersion(3,1);
	QGLWidget *qglWidget = new QGLWidget(fmt);
	int pro = qglWidget->format().profile();
	std::cout<<"OpenGLContext = "<<pro<<std::endl;
	std::cout<<"OpenGLVersion = "<<qglWidget->format().majorVersion()<<"."<<qglWidget->format().minorVersion()<<std::endl;
	view.setViewport(qglWidget);
	view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	view.setScene(new OpenGLScene);
	view.show();

	view.resize(1024, 768);

	return app.exec();
}
