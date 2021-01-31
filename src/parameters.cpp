#include "hdr/parameters.hpp"

Parameters::Parameters()
    : brightness(0),
      contrast(0),
      shadows(100),
      red(0),
      green(0),
      blue(0),
      hueIntensity(0),
      highlights(100),
      saturation(100) {
}

int Parameters::getBrightness() const {
  return Parameters::brightness;
}

void Parameters::setBrightness(int value) {
  Parameters::brightness = value;
}

int Parameters::getContrast() const {
  return Parameters::contrast;
}

void Parameters::setContrast(int value) {
  Parameters::contrast = value;
}

int Parameters::getShadows() const {
  return Parameters::shadows;
}

void Parameters::setShadows(int value) {
  Parameters::shadows = value;
}

int Parameters::getRed() const {
  return Parameters::red;
}

void Parameters::setRed(int value) {
  Parameters::red = value;
}
int Parameters::getGreen() const {
  return Parameters::green;
}

void Parameters::setGreen(int value) {
  Parameters::green = value;
}
int Parameters::getBlue() const {
  return Parameters::blue;
}

void Parameters::setBlue(int value) {
  Parameters::blue = value;
}

void Parameters::setHueInt(int value) {
  Parameters::hueIntensity = value;
}

int Parameters::getHueInt() const {
  return Parameters::hueIntensity;
}

int Parameters::getHighlights() const {
  return Parameters::highlights;
}

void Parameters::setHighlights(int value) {
  Parameters::highlights = value;
}

int Parameters::getSaturation() const {
  return Parameters::saturation;
}

void Parameters::setSaturation(int value) {
  Parameters::saturation = value;
}
Parameters::Parameters(QDataStream &in) {
  qint32 qbrightness;
  qint32 qcontrast;
  qint32 qshadows;
  qint32 qred;
  qint32 qgreen;
  qint32 qblue;
  qint32 qhueIntensity;
  qint32 qhighlights;
  qint32 qsaturation;
  in >> qbrightness >> qcontrast >> qshadows >> qred >> qgreen >> qblue >>
      qhueIntensity >> qhighlights >> qsaturation;
  brightness = (int)qbrightness;
  contrast = (int)qcontrast;
  shadows = (int)qshadows;
  red = (int)qred;
  green = (int)qgreen;
  blue = (int)qblue;
  hueIntensity = (int)qhueIntensity;
  highlights = (int)qhighlights;
  saturation = (int)qsaturation;
}
void Parameters::serialize(QDataStream &out) {
  out << brightness << contrast << shadows << red << green << blue
      << hueIntensity << highlights << saturation;
}
