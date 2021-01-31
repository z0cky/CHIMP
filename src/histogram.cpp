#include "hdr/histogram.h"

#include <utility>

Histogram::Histogram(QGraphicsView *view, QImage img) : mView(view) {
  Histogram::mImg = new QImage(std::move(img));
}

Histogram::~Histogram() {
  delete Histogram::mView;
}

void Histogram::setImage(QImage img) {
  Histogram::mImg = new QImage(std::move(img));
}

void Histogram::showBWHistogram() {
  std::array<int, PIX_RGB> bars;

  for (int &bar : bars) {
    bar = 0;
  }
  QRgb *line = nullptr;
  int maxPix = 0;

  for (int y = 0; y < this->mImg->height(); y++) {
    line = (QRgb *)this->mImg->scanLine(y);
    for (int x = 0; x < this->mImg->width(); x++) {
      int average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x])) / 3;
      average = average > 255 ? 255 : average;
      average = average < 0 ? 0 : average;
      bars[average]++;
      maxPix = bars[average] > maxPix ? bars[average] : maxPix;
    }
  }

  int viewHeight = this->mView->size().rheight();
  int recWidth = (this->mView->size().rwidth()) / 256;
  int x = 10;
  QGraphicsScene *scene = new QGraphicsScene();
  this->mView->setScene(scene);
  this->mView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
  QBrush bWbrush(Qt::blue);
  QPen bWpen(Qt::white);
  bWpen.setWidth(6);

  for (auto i = 0; i < PIX_RGB; i++) {
    QGraphicsRectItem *rectangle = nullptr;
    rectangle = scene->addRect(
        i, (maxPix - bars[i]) * viewHeight / (maxPix * 2), recWidth,
        bars[i] * viewHeight / (maxPix * 2), bWpen, bWbrush);
    x += recWidth;
  }

  this->mView->show();
}

void Histogram::showRGBHistogram(int signal) {
  std::array<int, PIX_RGB> redBars;
  std::array<int, PIX_RGB> greenBars;
  std::array<int, PIX_RGB> blueBars;

  for (int i = 0; i < PIX_RGB; i++) {
    redBars[i] = 0;
    greenBars[i] = 0;
    blueBars[i] = 0;
  }
  QRgb *line = nullptr;

  int maxRed = 0;
  int maxGreen = 0;
  int maxBlue = 0;

  for (int y = 0; y < this->mImg->height(); y++) {
    line = (QRgb *)this->mImg->scanLine(y);
    for (int x = 0; x < this->mImg->width(); x++) {
      int red = qRed(line[x]);
      int green = qGreen(line[x]);
      int blue = qBlue(line[x]);
      redBars[red]++;
      greenBars[green]++;
      blueBars[blue]++;
      maxRed = redBars[red] > maxRed ? redBars[red] : maxRed;
      maxGreen = greenBars[green] > maxGreen ? greenBars[green] : maxGreen;
      maxBlue = blueBars[blue] > maxBlue ? blueBars[blue] : maxBlue;
    }
  }

  int viewHeight = this->mView->size().rheight();
  int recWidth = (this->mView->size().rwidth()) / 256;
  int x = 10;
  QGraphicsScene *scene = new QGraphicsScene();
  this->mView->setScene(scene);
  this->mView->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
  QBrush rbrush(Qt::red);
  QPen rpen(Qt::red);
  QBrush gbrush(Qt::green);
  QPen gpen(Qt::green);
  QBrush bbrush(Qt::blue);
  QPen bpen(Qt::blue);
  rpen.setWidth(6);
  gpen.setWidth(6);
  bpen.setWidth(6);

  for (auto i = 0; i < PIX_RGB; i++) {
    QGraphicsRectItem *rectangle;
    if (signal % DRAW_RED == 0) {
      rectangle = scene->addRect(
          i, (maxRed - redBars[i]) * viewHeight / (maxRed * 2), recWidth,
          redBars[i] * viewHeight / (maxRed * 2), rpen, rbrush);
    }
    if (signal % DRAW_GREEN == 0) {
      rectangle = scene->addRect(
          i, (maxGreen - greenBars[i]) * viewHeight / (maxGreen * 2), recWidth,
          greenBars[i] * viewHeight / (maxGreen * 2), gpen, gbrush);
    }
    if (signal % DRAW_BLUE == 0) {
      rectangle = scene->addRect(
          i, (maxBlue - blueBars[i]) * viewHeight / (maxBlue * 2), recWidth,
          blueBars[i] * viewHeight / (maxBlue * 2), bpen, bbrush);
    }
    x += recWidth;
  }

  this->mView->show();
}
