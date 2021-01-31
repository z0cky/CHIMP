#include "hdr/content_view.h"

#include <QDebug>
#include <QGraphicsView>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QRect>
#include <QRubberBand>
#include <QWidget>
#include <QtWidgets>
#include <iostream>
#include <utility>

#include "hdr/constants.h"

constexpr int initial_image_width = 800;
constexpr int initial_image_height = 600;

ContentView::ContentView(QWidget *parent)
    : QWidget(parent),
      mRubberBand(new QRubberBand(QRubberBand::Rectangle, this)),
      mProjectPath(QString("")),
      mSceneGV(nullptr),
      mItemGV(nullptr) {
  QImage emptyImage(initial_image_width, initial_image_height,
                    QImage::Format_ARGB32);
  emptyImage.fill(Qt::transparent);
  this->mCurrentImage = emptyImage;

  this->mGraphicsView = dynamic_cast<QGraphicsView*>(parent);
}

void ContentView::loadImage(const QString &filepath) {
  QImage loaded;
  loaded.load(filepath);
  this->setImage(loaded);
}

QGraphicsView *ContentView::getGraphicsView() {
  return this->mGraphicsView;
}

void ContentView::setImage(const QImage &img) {
  this->mCurrentImage = img;
  this->mSceneGV = new QGraphicsScene();
  this->mItemGV = new QGraphicsPixmapItem(QPixmap::fromImage(img));
  this->mSceneGV->addItem(mItemGV);
  this->mGraphicsView->setScene(mSceneGV);
  this->mGraphicsView->show();
}

QImage ContentView::getImage() {
  return this->mCurrentImage;
}

QString ContentView::getFilePath() {
  return this->mFilePath;
}

void ContentView::setFilePath(const QString& filePath) {
  this->mFilePath = filePath;
}

bool ContentView::isFilePathSet() {
  return this->mFilePath != "";
}

void ContentView::wheelEvent(QWheelEvent *event) {
  this->mGraphicsView->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

  if (event->delta() > 0) {
    this->mGraphicsView->scale(ZOOM_VALUE, ZOOM_VALUE);
  } else {
    this->mGraphicsView->scale(1 / ZOOM_VALUE, 1 / ZOOM_VALUE);
  }
}

void ContentView::mousePressEvent(QMouseEvent *event) {
  if (!this->allowSelection) {
    event->ignore();
    return;
  }

  this->mRubberBandOriginPoint = event->pos();
  this->mRubberBand->setGeometry(QRect(this->mRubberBandOriginPoint, QSize()));
  this->mRubberBand->show();
}

void ContentView::mouseMoveEvent(QMouseEvent *event) {
  if (!this->allowSelection) {
    event->ignore();
    return;
  }

  this->mRubberBand->setGeometry(
      QRect(this->mRubberBandOriginPoint, event->pos()).normalized());
}

void ContentView::mouseReleaseEvent(QMouseEvent *event) {
  if (!this->allowSelection) {
    event->ignore();
    return;
  }

  QPointF upperLeft =
      this->mGraphicsView->mapToScene(this->mRubberBandOriginPoint);
  QPointF lowerRight = this->mGraphicsView->mapToScene(event->pos());
  QRect newSelectionRect(upperLeft.toPoint(), lowerRight.toPoint());
  this->mSelectionRect = newSelectionRect;
  qDebug() << "New selection: " << newSelectionRect;
}

void ContentView::mouseDoubleClickEvent(QMouseEvent *event) {
  static_cast<void>(*event);
  qDebug() << "Double click from ContentView";
}

QSize ContentView::sizeHint() const {
  return QSize(this->parentWidget()->size().width(),
               this->parentWidget()->size().height());
}

void ContentView::setSelection(bool value) {
  this->allowSelection = value;
}

QRect ContentView::getSelectionRect() {
  return this->mSelectionRect;
}

QImage ContentView::getOriginal() {
  return this->mOriginalImage;
}

void ContentView::setOriginal(QImage img) {
  this->mOriginalImage = std::move(img);
}
