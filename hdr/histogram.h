#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#define PIX_RGB 256
#define DRAW_RED 2
#define DRAW_GREEN 3
#define DRAW_BLUE 5

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QToolBox>

class Histogram {
 public:
  Histogram(QGraphicsView* view, QImage img);
  ~Histogram();
  void setImage(QImage img);
  void showBWHistogram();
  void showRGBHistogram(int signal);

 private:
  QGraphicsView* mView;
  QImage* mImg;
};

#endif  // HISTOGRAM_H
