#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <QDataStream>

class Parameters {
 public:
  Parameters();
  Parameters(const Parameters &copyParm) = default;
  Parameters(QDataStream &in);
  void serialize(QDataStream &out);

  void setBrightness(int);
  void setContrast(int);
  void setShadows(int);
  void setRed(int);
  void setGreen(int);
  void setBlue(int);
  void setHueInt(int);
  void setHighlights(int);
  void setSaturation(int);

  int getBrightness() const;
  int getContrast() const;
  int getShadows() const;
  int getRed() const;
  int getGreen() const;
  int getBlue() const;
  int getHueInt() const;
  int getHighlights() const;
  int getSaturation() const;

 private:
  int brightness;
  int contrast;
  int shadows;
  int red, green, blue;
  int hueIntensity;
  int highlights;
  int saturation;
};

#endif  // PARAMETERS_HPP
