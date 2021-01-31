#ifndef CONTENT_VIEW_H
#define CONTENT_VIEW_H

#include <QColor>
#include <QGraphicsView>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QRubberBand>
#include <QScrollArea>
#include <QWidget>

class ContentView : public QWidget {
  Q_OBJECT

 public:
  enum Tool { NO_TOOL, LINE, ELLIPSE, RECTANGLE, STROKE };
  ContentView(QWidget* parent = nullptr);
  void loadImage(const QString& filepath);
  void showImage();
  void paintTest();
  QImage getImage();
  void setImage(const QImage& img);
  QString getFilePath();
  void setFilePath(const QString& filePath);
  bool isFilePathSet();
  void setSelection(bool value);
  QRect getSelectionRect();
  QImage getOriginal();
  void setOriginal(QImage);
  QGraphicsView* getGraphicsView();
  QRubberBand* mRubberBand;

 protected:
  void wheelEvent(QWheelEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseMoveEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);
  void mouseDoubleClickEvent(QMouseEvent* event);

 private:
  QSize sizeHint() const;

  // TODO(teodora): Should selection be a ContentView task? It makes sense,
  // since we are setting the selectionRect here, but this about it a bit more.
  bool allowSelection = false;

  QImage mCurrentImage;
  QString mFilePath;
  QString mProjectPath;
  QPoint mRubberBandOriginPoint;
  // TODO(teodora): All tools should get an additional selection rect argument,
  // and apply the effect only on that region. This should probably be set to
  // the size of the image every time an image is set.
  QRect mSelectionRect;
  QImage mOriginalImage = QImage(QPixmap(500, 500).toImage());

  QGraphicsView* mGraphicsView;
  QGraphicsScene* mSceneGV;
  QGraphicsItem* mItemGV;
};

#endif  // CONTENT_VIEW_H
