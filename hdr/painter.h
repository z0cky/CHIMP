#ifndef PAINTER_H
#define PAINTER_H

#include <QColor>
#include <QPen>
#include <QWidget>

#include "content_view.h"

class Painter : public QWidget {
  Q_OBJECT

 public:
  Painter(QWidget* parent = nullptr);
  void setCurrentTool(ContentView::Tool);
  void triggerBrushDrawing();
  void triggerEllipseDrawing();
  void triggerRectangleDrawing();
  void triggerLineDrawing();
  void setColor(const QColor& color);
  QColor getColor() const;
  void setWidth(int width);
  int getWidth() const;
  void setGradient(bool);
  bool getGradient() const;
  void setSquare(bool);
  bool getSquare() const;

 signals:
  void valueChanged();

 protected:
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseDoubleClickEvent(QMouseEvent* event);

 private:
  QSize sizeHint() const;

  void drawLineTo(const QPointF& endPoint);
  void drawEllipseTo(const QPointF& endPoint);
  void drawRectangleTo(const QPointF& endPoint);

  void drawTempLineTo(const QPointF& endPoint);
  void drawTempEllipseTo(const QPointF& endPoint);
  void drawTempRectangleTo(const QPointF& endPoint);

  bool allowStrokeDrawing = false;
  bool allowEllipseDrawing = false;
  bool allowRectangleDrawing = false;
  bool allowLineDrawing = false;

  bool strokeDrawingInProgress = false;
  bool ellipseDrawingInProgress = false;
  bool rectangleDrawingInProgress = false;
  bool lineDrawingInProgress = false;

  QPointF mLastPoint;
  QPen mPen;
  QImage mPaintDevice;
  QImage mOriginalImage;
  ContentView::Tool mCurrentTool;
  bool mGradient = false;
  ContentView* mContentView;
};

#endif  // PAINTER_H
