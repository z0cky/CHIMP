#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "hdr/constants.h"
#include "hdr/content_view.h"
#include "hdr/histogram.h"
#include "hdr/imageHistory.h"
#include "hdr/painter.h"
#include "hdr/parameters.hpp"
#include "hdr/tools.h"
#include "ui_mainwindow.h"
#include "ui_resizewindow.h"

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QImage>
#include <QMainWindow>
#include <QShortcut>
#include <QString>
#include <QtGui>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class ResizeWindow;
class PenWidthDialog;
}  // namespace Ui

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void actionOpen();
  void revertSliders();
  void setSliders(Parameters parms);
  void refreshState(const QImage &img);
  QImage applyAll();

 private slots:
  void on_actionOpen_triggered();
  void outOfFullScreen();
  void on_actionFull_Screen_Mode_triggered();
  void on_actionQuit_triggered();
  void on_actionSave_triggered();
  void on_actionSave_As_triggered();
  void on_actionDark_2_triggered();
  void on_actionAqua_triggered();
  void on_actionElegantNight_triggered();
  void on_actionNeon_triggered();
  void on_actionWanderlust_triggered();
  void on_actionPicPax_triggered();
  void on_actionGenetive_triggered();
  void on_actionTCobra_triggered();
  void on_actionUndo_triggered();
  void on_actionRedo_triggered();
  void on_actionShortcuts_triggered();
  void on_actionAbout_CHIMP_triggered();
  void on_actionResize_triggered();
  void on_checkRed_stateChanged();
  void on_checkGreen_stateChanged();
  void on_checkBlue_stateChanged();
  void on_horizontalSliderBrightness_sliderReleased();
  void on_horizontalSliderBrightness_valueChanged(int value);
  void on_horizontalSliderContrast_sliderReleased();
  void on_horizontalSliderContrast_valueChanged(int value);
  void on_horizontalSliderShadows_sliderReleased();
  void on_horizontalSliderShadows_valueChanged(int value);
  void on_HSliderColor_sliderReleased();
  void on_HSliderColor_valueChanged(int value);
  void on_horizontalSliderHighlights_sliderReleased();
  void on_horizontalSliderHighlights_valueChanged(int value);
  void on_actionBrush_triggered();
  void on_actionNew_triggered();
  void on_actionEllipse_triggered();
  void on_actionRectangle_triggered();
  void on_actionLine_triggered();
  void on_actionNegative_triggered();
  void on_actionMirrorX_triggered();
  void on_actionMirrorY_triggered();
  void on_actionNew_Window_triggered();
  void on_actionRotateRight90_triggered();
  void on_actionRotateLeft90_triggered();
  void on_horizontalSliderSaturation_sliderReleased();
  void on_horizontalSliderSaturation_valueChanged(int value);
  void on_actionSelection_triggered();
  void on_actionCrop_triggered();
  void on_actionOpen_Project_triggered();
  void on_actionSave_Project_triggered();
  void on_toolValueChanged();
  void setBrushColor();
  void setBrushOpacity(int value);
  void on_actionClose_2_triggered();
  void on_actionPenWidth_triggered();
  void enableUi();
  void on_pushButton_pressed();

 private:
  Ui::MainWindow *ui;
  ContentView *contentView;
  QShortcut *shortcutOutOfFullscreen;
  Histogram *histBW;
  Histogram *histRGB;
  ImageHistory *imageHistory;
  Painter *painter;
  int histFlag;
  Parameters *parameters;
};
#endif  // MAINWINDOW_H
