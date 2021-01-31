#include "hdr/painter.h"

#include <QBrush>
#include <QColor>
#include <QDebug>
#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QPen>
#include <QWidget>
#include <iostream>

#include "hdr/content_view.h"

#define INITIAL_BRUSH_WIDTH 5

Painter::Painter(QWidget *parent)
    : QWidget(parent),
      mContentView(dynamic_cast<ContentView*>(parent)) {
  // Initialize default pen.
  QPen defaultPen(QBrush(Qt::cyan, Qt::SolidPattern), INITIAL_BRUSH_WIDTH,
                  Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
  this->mPen = defaultPen;

  this->mCurrentTool = ContentView::NO_TOOL;

  qDebug() << "Painter initialized at location: " << this->pos().x() << ", "
           << this->pos().y() << ", " << this->size().width() << ", "
           << this->size().height();
}

void Painter::setCurrentTool(ContentView::Tool tool) {
  this->mCurrentTool = tool;
}

void Painter::setColor(const QColor &color) {
  this->mPen.setBrush(QBrush(color));
}

void Painter::setGradient(bool grad) {
  this->mGradient = grad;
}

bool Painter::getGradient() const {
  return this->mGradient;
}

QColor Painter::getColor() const {
  return mPen.color();
}

void Painter::setWidth(int width) {
  this->mPen.setWidth(width);
}

int Painter::getWidth() const {
  return this->mPen.width();
}

void Painter::setSquare(bool ind) {
  if (ind) {
    this->mPen.setCapStyle(Qt::SquareCap);
    mPen.setJoinStyle(Qt::RoundJoin);
  } else {
    mPen.setCapStyle(Qt::RoundCap);
    mPen.setJoinStyle(Qt::RoundJoin);
  }
}

bool Painter::getSquare() const {
  return mPen.capStyle() == Qt::SquareCap;
}

QSize Painter::sizeHint() const {
  return QSize(this->parentWidget()->size().width(),
               this->parentWidget()->size().height());
}

void Painter::triggerBrushDrawing() {
  this->mCurrentTool = ContentView::STROKE;
}

void Painter::triggerEllipseDrawing() {
  this->mCurrentTool = ContentView::ELLIPSE;
}

void Painter::triggerRectangleDrawing() {
  this->mCurrentTool = ContentView::RECTANGLE;
}

void Painter::triggerLineDrawing() {
  this->mCurrentTool = ContentView::LINE;
}

void Painter::mousePressEvent(QMouseEvent *event) {
  if (this->mCurrentTool == ContentView::NO_TOOL) {
    event->ignore();
    return;
  }

  if (event->button() == Qt::LeftButton) {
    if (this->mCurrentTool == ContentView::STROKE) {
      this->strokeDrawingInProgress = true;
    } else if (this->mCurrentTool == ContentView::ELLIPSE) {
      this->ellipseDrawingInProgress = true;
      this->mOriginalImage = this->mContentView->getImage();
    } else if (this->mCurrentTool == ContentView::RECTANGLE) {
      this->rectangleDrawingInProgress = true;
      this->mOriginalImage = this->mContentView->getImage();
    } else if (this->mCurrentTool == ContentView::LINE) {
      this->lineDrawingInProgress = true;
      this->mOriginalImage = this->mContentView->getImage();
    }
    this->mLastPoint =
        this->mContentView->getGraphicsView()->mapToScene(event->pos());
    event->accept();
  }
}

void Painter::mouseMoveEvent(QMouseEvent *event) {
  if (this->mCurrentTool == ContentView::NO_TOOL) {
    event->ignore();
    return;
  }

  if ((event->buttons() & Qt::LeftButton)) {
    if (this->strokeDrawingInProgress) {
      this->drawLineTo(
          this->mContentView->getGraphicsView()->mapToScene(event->pos()));
    } else if (this->ellipseDrawingInProgress) {
      this->drawTempEllipseTo(
          this->mContentView->getGraphicsView()->mapToScene(event->pos()));
    } else if (this->rectangleDrawingInProgress) {
      this->drawTempRectangleTo(
          this->mContentView->getGraphicsView()->mapToScene(event->pos()));
    } else if (this->lineDrawingInProgress) {
      this->drawTempLineTo(
          this->mContentView->getGraphicsView()->mapToScene(event->pos()));
    }
  }
  event->accept();
}

void Painter::mouseReleaseEvent(QMouseEvent *event) {
  if (this->mCurrentTool == ContentView::NO_TOOL) {
    event->ignore();
    return;
  }

  if (event->button() == Qt::LeftButton) {
    if (this->strokeDrawingInProgress) {
      this->drawLineTo(
          this->mContentView->getGraphicsView()->mapToScene(event->pos()));
      this->strokeDrawingInProgress = false;
    } else if (this->ellipseDrawingInProgress) {
      this->drawEllipseTo(
          this->mContentView->getGraphicsView()->mapToScene(event->pos()));
      this->ellipseDrawingInProgress = false;
    } else if (this->rectangleDrawingInProgress) {
      this->drawRectangleTo(
          this->mContentView->getGraphicsView()->mapToScene(event->pos()));
      this->rectangleDrawingInProgress = false;
    } else if (this->lineDrawingInProgress) {
      this->drawLineTo(
          this->mContentView->getGraphicsView()->mapToScene(event->pos()));
      this->lineDrawingInProgress = false;
    }
  }
  event->accept();
  emit valueChanged();
}

void Painter::mouseDoubleClickEvent(QMouseEvent *event) {
  if (this->mCurrentTool == ContentView::NO_TOOL) {
    event->ignore();
    return;
  }

  std::cout << "Hello double click" << std::endl;
  event->accept();
}

void Painter::drawTempEllipseTo(const QPointF &endPoint) {
  QPainter painter;
  this->mPaintDevice = this->mOriginalImage;
  painter.begin(&(this->mPaintDevice));
  painter.setPen(this->mPen);
  QRect rect(this->mLastPoint.x(), this->mLastPoint.y(),
             endPoint.x() - this->mLastPoint.x(),
             endPoint.y() - this->mLastPoint.y());
  painter.drawEllipse(rect);
  painter.end();
  this->mContentView->setImage(this->mPaintDevice);
}

void Painter::drawTempRectangleTo(const QPointF &endPoint) {
  QPainter painter;
  this->mPaintDevice = this->mOriginalImage;
  painter.begin(&(this->mPaintDevice));
  painter.setPen(this->mPen);
  QRect rect(this->mLastPoint.x(), this->mLastPoint.y(),
             endPoint.x() - this->mLastPoint.x(),
             endPoint.y() - this->mLastPoint.y());
  painter.drawRect(rect);
  painter.end();
  this->mContentView->setImage(this->mPaintDevice);
}

void Painter::drawTempLineTo(const QPointF &endPoint) {
  QPainter painter;
  this->mPaintDevice = this->mOriginalImage;
  painter.begin(&(this->mPaintDevice));

  if (this->mGradient) {
    QLinearGradient gradient(this->mLastPoint, endPoint);
    QColor color = mPen.color();
    color.setAlphaF(0.9);
    gradient.setColorAt(0, color);
    color.setAlphaF(0.1);
    gradient.setColorAt(1, color);
    QPen pen(QBrush(gradient), mPen.width());
    if (!this->getSquare()) {
      pen.setCapStyle(Qt::RoundCap);
    }
    painter.setPen(pen);
  } else {
    painter.setPen(this->mPen);
  }

  painter.drawLine(this->mLastPoint, endPoint);
  painter.end();
  this->mContentView->setImage(this->mPaintDevice);
}

void Painter::drawLineTo(const QPointF &endPoint) {
  QPainter painter;
  this->mPaintDevice = this->mContentView->getImage();
  painter.begin(&(this->mPaintDevice));

  if (this->mGradient) {
    QLinearGradient gradient(this->mLastPoint, endPoint);
    QColor color = mPen.color();
    color.setAlphaF(0.9);
    gradient.setColorAt(0, color);
    color.setAlphaF(0.1);
    gradient.setColorAt(1, color);
    QPen pen(QBrush(gradient), mPen.width());
    if (!this->getSquare()) {
      pen.setCapStyle(Qt::RoundCap);
    }
    painter.setPen(pen);
  } else {
    painter.setPen(this->mPen);
  }

  painter.drawLine(this->mLastPoint, endPoint);
  painter.end();

  // We also need to update the original image, because otherwise the same
  // parameters, applied to a modified image would produce a different result.
  // Meaning, just "clicking" on the slider value will modify the image, which
  // is not what we want to happen.
  QImage deviceOrg = this->mContentView->getOriginal();
  QPainter painterOrg;
  painterOrg.begin(&deviceOrg);

  if (this->mGradient) {
    QLinearGradient gradient(this->mLastPoint, endPoint);
    QColor color = mPen.color();
    color.setAlphaF(0.9);
    gradient.setColorAt(0, color);
    color.setAlphaF(0.1);
    gradient.setColorAt(1, color);
    QPen pen(QBrush(gradient), mPen.width());
    if (!this->getSquare()) {
      pen.setCapStyle(Qt::RoundCap);
    }
    painterOrg.setPen(pen);
  } else {
    painterOrg.setPen(this->mPen);
  }
  painterOrg.drawLine(this->mLastPoint, endPoint);
  painterOrg.end();
  this->mContentView->setOriginal(deviceOrg);

  this->mLastPoint = endPoint;
  this->mContentView->setImage(this->mPaintDevice);
}

void Painter::drawEllipseTo(const QPointF &endPoint) {
  QPainter painter;
  this->mPaintDevice = this->mContentView->getImage();
  painter.begin(&(this->mPaintDevice));
  painter.setPen(this->mPen);
  QRect rect(this->mLastPoint.x(), this->mLastPoint.y(),
             endPoint.x() - this->mLastPoint.x(),
             endPoint.y() - this->mLastPoint.y());
  painter.drawEllipse(rect);
  painter.end();

  // We also need to update the original image, because otherwise the same
  // parameters, applied to a modified image would produce a different result.
  // Meaning, just "clicking" on the slider value will modify the image, which
  // is not what we want to happen.
  QImage deviceOrg = this->mContentView->getOriginal();
  QPainter painterOrg;
  painterOrg.begin(&deviceOrg);
  painterOrg.setPen(this->mPen);
  painterOrg.drawEllipse(rect);
  painterOrg.end();
  this->mContentView->setOriginal(deviceOrg);

  this->mLastPoint = endPoint;
  this->mContentView->setImage(this->mPaintDevice);
}

void Painter::drawRectangleTo(const QPointF &endPoint) {
  QPainter painter;
  this->mPaintDevice = this->mContentView->getImage();
  painter.begin(&(this->mPaintDevice));
  painter.setPen(this->mPen);
  QRect rect(this->mLastPoint.x(), this->mLastPoint.y(),
             endPoint.x() - this->mLastPoint.x(),
             endPoint.y() - this->mLastPoint.y());
  painter.drawRect(rect);
  painter.end();

  // We also need to update the original image, because otherwise the same
  // parameters, applied to a modified image would produce a different result.
  // Meaning, just "clicking" on the slider value will modify the image, which
  // is not what we want to happen.
  QImage deviceOrg = this->mContentView->getOriginal();
  QPainter painterOrg;
  painterOrg.begin(&deviceOrg);
  painterOrg.setPen(this->mPen);
  painterOrg.drawRect(rect);
  painterOrg.end();
  this->mContentView->setOriginal(deviceOrg);

  this->mLastPoint = endPoint;
  this->mContentView->setImage(this->mPaintDevice);
}
