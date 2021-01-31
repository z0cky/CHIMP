#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <QImage>
#include "constants.h"

class Tools {
 public:
  static QImage changeContrast(const QImage& img, double contrast);
  static QImage changeBrightness(const QImage& img, double brightness);
  static QImage changeShadows(const QImage& img, double gamma);
  static QImage blacks(const QImage& img, double gamma = 4);
  static QImage changeHighlights(const QImage& img, double gamma = 1.5);
  static QImage whites(const QImage& img, double gamma = 1.5);
  static QImage changeColor(const QImage& img, double gamma,
                            const QColor& color);
  static QImage changeSaturation(const QImage& img, double saturation);
  static QImage crop(const QImage& img, const QRect& cropRect);
  static QImage negative(const QImage& img);
  static QImage mirrorX(const QImage& img);
  static QImage mirrorY(const QImage& img);
  static QImage rotateRight90(const QImage& img);
  static QImage rotateLeft90(const QImage& img);
};

#endif  // TOOLS_HPP
