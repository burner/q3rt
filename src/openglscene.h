#ifndef OPENGLSCENE_H
#define OPENGLSCENE_H

#include <QGraphicsScene>
#include <QLabel>
#include <QTime>


class OpenGLScene : public QGraphicsScene
{
    Q_OBJECT

public:
    OpenGLScene();

    void drawBackground(QPainter *painter, const QRectF &rect);

public slots:
    void updateFps();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent * wheelEvent);

private:
	//fps
	int frames;
	int fpsCnt;
    QDialog *createDialog(const QString &windowTitle) const;

    QColor m_backgroundColor;

    QTime m_time;
    int m_lastTime;
    int m_mouseEventTime;

    float m_distance;

	QLabel *fps;
    QLabel *m_labels[4];
    QWidget *m_modelButton;

    QGraphicsRectItem *m_lightItem;
};

#endif
