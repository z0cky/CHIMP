#include "hdr/tools.h"

#include <QImage>
#include <QRect>
#include <cmath>
#include <QPixmap>

double truncate(double x) {
  if (x > PIX_MAX) {
    return PIX_MAX;
  }
  if (x < 0) {
    return 0;
  }
  return x;
}

QImage Tools::changeContrast(const QImage& img, double contrast) {
  QImage newImage(img.width(), img.height(), QImage::Format_ARGB32);

  QRgb* line = nullptr;
  double factor;
  double newRed;
  double newGreen;
  double newBlue;

  factor = (259 * (contrast + PIX_MAX)) / (PIX_MAX * (259 - contrast));

  for (int y = 0; y < newImage.height(); y++) {
    line = (QRgb*)img.scanLine(y);

    for (int x = 0; x < newImage.width(); x++) {
      newRed = truncate(factor * (qRed(line[x]) - 128) + 128);
      newGreen = truncate(factor * (qGreen(line[x]) - 128) + 128);
      newBlue = truncate(factor * (qBlue(line[x]) - 128) + 128);
      newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
    }
  }

  return newImage;
}

QImage Tools::changeBrightness(const QImage& img, double brightness) {
  QImage newImage(img.width(), img.height(), QImage::Format_ARGB32);

  QRgb* line = nullptr;
  double newRed;
  double newGreen;
  double newBlue;

  for (int y = 0; y < newImage.height(); y++) {
    line = (QRgb*)img.scanLine(y);

    for (int x = 0; x < newImage.width(); x++) {
      newRed = truncate(qRed(line[x]) + brightness);
      newGreen = truncate(qGreen(line[x]) + brightness);
      newBlue = truncate(qBlue(line[x]) + brightness);
      newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
    }
  }

  return newImage;
}

// shadows - pocetna vrednost gamma je 1 - desne vrednosti slajdera se skaliraju
// na [1, 2.5], leve na [0.4, 1]
QImage Tools::changeShadows(const QImage& img, double gamma) {
  QImage newImage(img.width(), img.height(), QImage::Format_ARGB32);

  QRgb* line = nullptr;
  double newRed;
  double newGreen;
  double newBlue;

  if (gamma >= 100) {
    gamma = 1.5 * gamma -
            50;  // desne vrednosti slajdera se skaliraju sa 100-200 na 100-250
  } else {
    gamma = 0.6 * gamma +
            40;  // leve vrednosti slajdera se skaliraju sa 0-100 na 40-100
  }

  // double gammaCorrection = gamma;
  // TODO: Refine gammaCorrection
  double gammaCorrection = gamma / 100.0;

  for (int y = 0; y < newImage.height(); y++) {
    line = (QRgb*)img.scanLine(y);

    for (int x = 0; x < newImage.width(); x++) {
      newRed = truncate(PIX_MAX * pow((qRed(line[x]) / (PIX_MAX*1.0)), gammaCorrection));
      newGreen =
          truncate(PIX_MAX * pow((qGreen(line[x]) / (PIX_MAX*1.0)), gammaCorrection));
      newBlue = truncate(PIX_MAX * pow((qBlue(line[x]) / (PIX_MAX*1.0)), gammaCorrection));
      newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
    }
  }

  return newImage;
}

// highlights - pocetna vrednost gamma je 1 - desne vrednosti slajdera se
// skaliraju na [1, 2.5], leve na [0.4, 1]
QImage Tools::changeHighlights(const QImage& img, double gamma) {
  QImage newImage(img.width(), img.height(), QImage::Format_ARGB32);

  QRgb* line = nullptr;
  double newRed;
  double newGreen;
  double newBlue;

  if (gamma >= 100) {
    gamma = 1.5 * gamma -
            50;  // desne vrednosti slajdera se skaliraju sa 100-200 na 100-250
  } else {
    gamma = 0.6 * gamma +
            40;  // leve vrednosti slajdera se skaliraju sa 0-100 na 40-100
  }

  // double gammaCorrection = gamma; // TODO: Refine gammaCorrection
  double gammaCorrection = gamma / 100.0;

  for (int y = 0; y < newImage.height(); y++) {
    line = (QRgb*)img.scanLine(y);

    for (int x = 0; x < newImage.width(); x++) {
      newRed = PIX_MAX * (1 - (pow(1 - (qRed(line[x]) / (PIX_MAX*1.0)), gammaCorrection)));
      newGreen =
          PIX_MAX * (1 - (pow(1 - (qGreen(line[x]) / (PIX_MAX*1.0)), gammaCorrection)));
      newBlue =
          PIX_MAX * (1 - (pow(1 - (qBlue(line[x]) / (PIX_MAX*1.0)), gammaCorrection)));
      newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
    }
  }

  return newImage;
}

QImage Tools::changeColor(const QImage& img, double gamma,
                          const QColor& color) {
  QImage newImage(img.width(), img.height(), QImage::Format_ARGB32);

  QRgb* line = nullptr;
  double newRed;
  double newGreen;
  double newBlue;
  double rgamma = color.red() / (PIX_MAX*1.0);
  double ggamma = color.green() / (PIX_MAX*1.0);
  double bgamma = color.blue() / (PIX_MAX*1.0);
  double gammaCorrection = gamma / 100.0; //Precentage

  for (int y = 0; y < newImage.height(); y++) {
    line = (QRgb*)img.scanLine(y);

    for (int x = 0; x < newImage.width(); x++) {
      if (rgamma != 0) {
        newRed = truncate(
            PIX_MAX * pow((qRed(line[x]) / (PIX_MAX*1.0)), (1 - gammaCorrection * rgamma)));
      } else {
        newRed = qRed(line[x]);
      }

      if (ggamma != 0) {
        newGreen = truncate(PIX_MAX * pow((qGreen(line[x]) / (PIX_MAX*1.0)),
                                      (1 - gammaCorrection * ggamma)));
      } else {
        newGreen = qGreen(line[x]);
      }

      if (bgamma != 0) {
        newBlue = truncate(PIX_MAX * pow((qBlue(line[x]) / (PIX_MAX*1.0)),
                                     (1 - gammaCorrection * bgamma)));
      } else {
        newBlue = qBlue(line[x]);
      }
      newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
    }
  }

  return newImage;
}

QImage Tools::changeSaturation(const QImage& img, double saturation) {
  QImage newImage(img.width(), img.height(), QImage::Format_ARGB32);

  QRgb* line = nullptr;
  double newRed;
  double newGreen;
  double newBlue;
  double average;

  saturation /= 100;  // Sliders have integer values,
                      // saturation coeficient is between 0 and 2, so slider goes from 0 to 200 and is devided by 100.

  for (int y = 0; y < newImage.height(); y++) {
    line = (QRgb*)img.scanLine(y);

    for (int x = 0; x < newImage.width(); x++) {
      average = (qRed(line[x]) + qGreen(line[x]) + qRed(line[x])) / 3;
      newRed = truncate(average + (qRed(line[x]) - average) * saturation);
      newGreen = truncate(average + (qGreen(line[x]) - average) * saturation);
      newBlue = truncate(average + (qBlue(line[x]) - average) * saturation);
      newImage.setPixel(x, y, qRgb(newRed, newGreen, newBlue));
    }
  }

  return newImage;
}

QImage Tools::crop(const QImage& img, const QRect& cropRect) {
  return img.copy(cropRect);
}

QImage Tools::negative(const QImage& img) {
    QImage nImage;
    nImage = img;

    for (int w = 0; w < nImage.width(); w++) {
      for (int h = 0; h < nImage.height(); h++) {
        QRgb prevCol = nImage.pixel(w, h);
        prevCol = qRgb(INVERT - qRed(prevCol),
                       INVERT - qGreen(prevCol),
                       INVERT - qBlue(prevCol));
        nImage.setPixel(w, h, prevCol);
      }
    }

    return nImage;
}

QImage Tools::mirrorX(const QImage& img) {
    QImage mImage;
    QImage imgMirror;
    QColor mColor;

    mImage = img;
    imgMirror = QPixmap(mImage.width(), mImage.height()).toImage();

    for (int w = 0; w < mImage.width(); w++) {
      for (int h = 0; h < mImage.height(); h++) {
        mColor = mImage.pixel(w, h);
        imgMirror.setPixel(w, mImage.height() - h - 1, mColor.rgb());
      }
    }

    return imgMirror;
}

QImage Tools::mirrorY(const QImage& img) {
    QImage mImage, imgMirror;
    QColor mColor;

    mImage = img;
    imgMirror = QPixmap(mImage.width(), mImage.height()).toImage();

    for (int w = 0; w < mImage.width(); w++) {
      for (int h = 0; h < mImage.height(); h++) {
        mColor = mImage.pixel(w, h);
        imgMirror.setPixel(mImage.width() - w - 1, h, mColor.rgb());
      }
    }

    return imgMirror;
}

QImage Tools::rotateRight90(const QImage& img) {
    QImage rImage;
    QImage imgRotate;
    QColor rotColor;

    rImage = img;
    imgRotate = QPixmap(rImage.height(), rImage.width()).toImage();

    for (int w = 0; w < rImage.width(); w++) {
      for (int h = 0; h < rImage.height(); h++) {
        rotColor = rImage.pixel(w, h);
        imgRotate.setPixel(rImage.height() - h - 1, w, rotColor.rgb());
      }
    }

    return imgRotate;
}

QImage Tools::rotateLeft90(const QImage& img) {
    QImage rImage, imgRotate;
    QColor rotColor;

    rImage = img;
    imgRotate = QPixmap(rImage.height(), rImage.width()).toImage();

    for (int w = 0; w < rImage.width(); w++) {
      for (int h = 0; h < rImage.height(); h++) {
        rotColor = rImage.pixel(w, h);
        imgRotate.setPixel(h, rImage.width() - w - 1, rotColor.rgb());
      }
    }

    return imgRotate;
}

