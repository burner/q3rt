#include "openglscene.h"

#include <QtGui>
#include <QtOpenGL>
#include <iostream>

QDialog *OpenGLScene::createDialog(const QString &windowTitle) const {
	QDialog *dialog = new QDialog(0, Qt::CustomizeWindowHint | Qt::WindowTitleHint);

	dialog->setWindowOpacity(0.8);
	dialog->setWindowTitle(windowTitle);
	dialog->setLayout(new QVBoxLayout);

	return dialog;
}

OpenGLScene::OpenGLScene()
	: m_backgroundColor(0, 170, 255)
	, m_lastTime(0)
	, m_distance(1.4f) {

	QWidget *controls = createDialog(tr("Controls"));
	this->frames = 0;
	QWidget *statistics = createDialog(tr("Rendering stats"));
	//statistics->layout()->setMargin(20);

	for(int i = 0; i < 4; ++i) {
		m_labels[i] = new QLabel;
		statistics->layout()->addWidget(m_labels[i]);
	}
	this->fps = new QLabel("fps :%0");
	statistics->layout()->addWidget(this->fps);

	QWidget *instructions = createDialog(tr("Instructions"));
	instructions->layout()->addWidget(new QLabel(tr("Use mouse wheel to zoom model, and click and drag to rotate model")));
	instructions->layout()->addWidget(new QLabel(tr("Move the sun around to change the light position")));

	QWidget *widgets[] = { instructions, controls, statistics };

	for(uint i = 0; i < sizeof(widgets) / sizeof(*widgets); ++i) {
		QGraphicsProxyWidget *proxy = new QGraphicsProxyWidget(0, Qt::Dialog);
		proxy->setWidget(widgets[i]);
		addItem(proxy);
	}
/*
	QPointF pos(10, 10);
	foreach (QGraphicsItem *item, items()) {
		item->setFlag(QGraphicsItem::ItemIsMovable);
		item->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

		const QRectF rect = item->boundingRect();
		item->setPos(pos.x() - rect.x(), pos.y() - rect.y());
		pos += QPointF(0, 10 + rect.height());
	}

	QRadialGradient gradient(40, 40, 40, 40, 40);
	gradient.setColorAt(0.2, Qt::yellow);
	gradient.setColorAt(1, Qt::transparent);

	m_lightItem = new QGraphicsRectItem(0, 0, 80, 80);
	m_lightItem->setPen(Qt::NoPen);
	m_lightItem->setBrush(gradient);
	m_lightItem->setFlag(QGraphicsItem::ItemIsMovable);
	m_lightItem->setPos(800, 200);
	addItem(m_lightItem);
*/
	m_time.start();
	QTimer::singleShot(1000, this, SLOT(updateFps()));
}

void OpenGLScene::drawBackground(QPainter *painter, const QRectF &) {
	if(painter->paintEngine()->type() != QPaintEngine::OpenGL
		&& painter->paintEngine()->type() != QPaintEngine::OpenGL2) {
		qWarning("OpenGLScene: drawBackground needs a QGLWidget to be set as viewport on the graphics view");
		return;
	}

	painter->beginNativePainting();

	glClearColor(m_backgroundColor.redF(), m_backgroundColor.greenF(), m_backgroundColor.blueF(), 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	frames++;

	painter->endNativePainting();

	QTimer::singleShot(0, this, SLOT(update()));
}

void OpenGLScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseMoveEvent(event);
	if(event->isAccepted())
		return;
	if(event->buttons() & Qt::LeftButton) {
		const QPointF delta = event->scenePos() - event->lastScenePos();

		event->accept();
		update();
	}
}

void OpenGLScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mousePressEvent(event);
	if(event->isAccepted())
		return;

	event->accept();
}

void OpenGLScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
	QGraphicsScene::mouseReleaseEvent(event);
	if(event->isAccepted())
		return;

	event->accept();
	update();
}

void OpenGLScene::wheelEvent(QGraphicsSceneWheelEvent *event) {
	QGraphicsScene::wheelEvent(event);
	if(event->isAccepted())
		return;

	event->accept();
	update();
}

void OpenGLScene::updateFps() {
	fpsCnt = frames;
	frames = 0;
	this->fps->setText(tr("fps: %0").arg(fpsCnt));
	QTimer::singleShot(1000, this, SLOT(updateFps()));
}
