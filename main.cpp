#include "openglscene.h"

#include <QtGui>
#include <QGLWidget>
#include <iostream>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView()
    {
        setWindowTitle(tr("3D Model Viewer"));
    }

protected:
    void resizeEvent(QResizeEvent *event) {
        if (scene())
            scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
        QGraphicsView::resizeEvent(event);
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    GraphicsView view;
	QGLFormat fmt(QGL::SampleBuffers);
	fmt.setProfile(QGLFormat::CoreProfile);
	fmt.setVersion(3,3);
	const char* v = (const char*)glGetString(GL_VERSION);
	std::cout<<"QT Version "<<v<<"\n defined."<<std::endl;
	std::cout<<std::endl;
    //view.setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    view.setViewport(new QGLWidget(fmt));
    view.setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view.setScene(new OpenGLScene);
    view.show();

    view.resize(1024, 768);

    return app.exec();
}
