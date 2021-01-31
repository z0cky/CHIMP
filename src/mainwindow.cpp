#include "hdr/mainwindow.h"

#include <QColor>
#include <QColorDialog>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QPixmap>
#include <QShortcut>
#include <QString>
#include <string>

#include "hdr/content_view.h"
#include "hdr/histogram.h"
#include "hdr/imageHistory.h"
#include "hdr/parameters.hpp"
#include "hdr/tools.h"
#include "ui_mainwindow.h"
#include "ui_penwidthdialog.h"

void MainWindow::refreshState(const QImage& img) {
  item newItem;
  newItem.parms = Parameters(*this->parameters);
  newItem.img = img;
  newItem.original = this->contentView->getOriginal();
  this->contentView->setImage(img);
  this->imageHistory->newEdit(newItem);
  this->histBW->setImage(img);
  this->histRGB->setImage(img);
  this->histBW->showBWHistogram();
  this->histRGB->showRGBHistogram(histFlag);
  this->contentView->mRubberBand->hide();
}

void MainWindow::revertSliders() {
  this->ui->horizontalSliderShadows->setValue(100);
  this->ui->horizontalSliderHighlights->setValue(100);
  this->ui->horizontalSliderContrast->setValue(0);
  this->ui->horizontalSliderBrightness->setValue(0);
  this->ui->horizontalSliderSaturation->setValue(100);
  this->ui->HSliderColor->setValue(0);
  this->ui->spinRed->setValue(0);
  this->ui->spinBlue->setValue(0);
  this->ui->spinGreen->setValue(0);
}

void MainWindow::setSliders(Parameters parms) {
  this->ui->horizontalSliderShadows->setValue(parms.getShadows());
  this->ui->horizontalSliderHighlights->setValue(parms.getHighlights());
  this->ui->horizontalSliderContrast->setValue(parms.getContrast());
  this->ui->horizontalSliderBrightness->setValue(parms.getBrightness());
  this->ui->horizontalSliderSaturation->setValue(parms.getSaturation());
  this->ui->HSliderColor->setValue(parms.getHueInt());
  this->ui->spinRed->setValue(parms.getRed());
  this->ui->spinBlue->setValue(parms.getBlue());
  this->ui->spinGreen->setValue(parms.getGreen());
}

QImage MainWindow::applyAll() {
  QImage edited = this->contentView->getOriginal();

  if (parameters->getContrast() != 0) {
    edited = Tools::changeContrast(edited, parameters->getContrast());
  }
  if (parameters->getBrightness() != 0) {
    edited = Tools::changeBrightness(edited, parameters->getBrightness());
  }
  if (parameters->getShadows() != 100) {
    edited = Tools::changeShadows(edited, parameters->getShadows());
  }
  if (parameters->getHighlights() != 100) {
    edited = Tools::changeHighlights(edited, parameters->getHighlights());
  }
  if (parameters->getSaturation() != 100) {
    edited = Tools::changeSaturation(edited, parameters->getSaturation());
  }
  if (parameters->getHueInt() != 0) {
    edited =
        Tools::changeColor(edited, parameters->getHueInt(),
                           QColor(parameters->getRed(), parameters->getGreen(),
                                  parameters->getBlue()));
  }

  return edited;
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow) {
  ui->setupUi(this);
  this->setWindowTitle("CHIMP");
  ui->toolBar->setIconSize(QSize(45, 30));

  shortcutOutOfFullscreen = new QShortcut(this);
  shortcutOutOfFullscreen->setKey(Qt::Key_Escape);
  connect(shortcutOutOfFullscreen, SIGNAL(activated()), this,
          SLOT(outOfFullScreen()));

  this->contentView = new ContentView(ui->graphicsView);

  this->painter = new Painter(this->contentView);
  this->parameters = new Parameters();

  QImage newImg(10, 10, QImage::Format_ARGB32);
  this->histBW = new Histogram(ui->graphicsHistBW, newImg);
  this->histRGB = new Histogram(ui->graphicsHistRGB, newImg);
  this->histFlag = 1;
  this->parameters = new Parameters();
  this->imageHistory = new ImageHistory();

  QObject::connect(this->painter, SIGNAL(valueChanged()), this,
                   SLOT(on_toolValueChanged()));
}

MainWindow::~MainWindow() {
  delete ui;
}
void MainWindow::enableUi() {
  ui->actionUndo->setEnabled(true);
  ui->actionRedo->setEnabled(true);
  ui->actionSelection->setEnabled(true);
  ui->actionCrop->setEnabled(true);
  ui->actionResize->setEnabled(true);
  ui->actionPenWidth->setEnabled(true);
  ui->actionRotateLeft90->setEnabled(true);
  ui->actionRotateRight90->setEnabled(true);
  ui->actionMirrorX->setEnabled(true);
  ui->actionMirrorY->setEnabled(true);
  ui->actionNegative->setEnabled(true);
  ui->actionEllipse->setEnabled(true);
  ui->actionRectangle->setEnabled(true);
  ui->actionLine->setEnabled(true);
  ui->actionBrush->setEnabled(true);
  ui->sliderFrame->setEnabled(true);
  ui->colorFrame->setEnabled(true);
  ui->toolBox->setEnabled(true);
  ui->checkRed->setEnabled(true);
  ui->checkBlue->setEnabled(true);
  ui->checkGreen->setEnabled(true);
  ui->graphicsView->setEnabled(true);
  ui->pushButton->setEnabled(true);
  ui->actionSave_Project->setEnabled(true);
  ui->actionSave_As->setEnabled(true);
  ui->actionSave->setEnabled(true);

}
void MainWindow::on_actionOpen_triggered() {
  bool ind = true;
  if (!this->imageHistory->isSaved()) {
    QFileInfo fi(this->contentView->getFilePath());
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "CHIMP",
        "Unsaved changes found. Save changes to " + fi.fileName() +
            " before exiting?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (reply) {
      case QMessageBox::Cancel:
        ind = false;
        break;
      case QMessageBox::Save:
        MainWindow::on_actionSave_triggered();
        ind = true;
        break;
      case QMessageBox::Discard:
        ind = true;
        break;
      default:
        ind = true;
        break;
    }
  }

  if (ind) {
    QString filePath = QFileDialog::getOpenFileName();
    if (filePath == "") {
      QMessageBox::critical(this, "Open image", "Error opening file");
      return;
    }
    this->contentView->setFilePath(filePath);
    this->contentView->loadImage(filePath);
    revertSliders();
    QImage img = this->contentView->getImage();
    this->contentView->setOriginal(img);
    refreshState(img);
    enableUi();
  }
}
void MainWindow::outOfFullScreen() {
  if (this->isFullScreen()) {
    QWidget::showNormal();
  }
}

// Full screen mode, trenutno se zatvara na Alt+F4
void MainWindow::on_actionFull_Screen_Mode_triggered() {
  QWidget::showFullScreen();
}

void MainWindow::on_actionSave_triggered() {
  if (this->contentView->isFilePathSet()) {
    this->contentView->getImage().save(this->contentView->getFilePath());
  } else {
    QString savePath = QFileDialog::getSaveFileName(
        this, tr("Save File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
    this->contentView->getImage().save(savePath);
    this->contentView->setFilePath(savePath);
  }
}

void MainWindow::on_actionSave_As_triggered() {
  QString savePath = QFileDialog::getSaveFileName(
      this, tr("Save File"), "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
  this->contentView->getImage().save(savePath);
}

void MainWindow::on_actionQuit_triggered() {
  if (!this->imageHistory->isSaved()) {
    QFileInfo fi(this->contentView->getFilePath());
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "CHIMP",
        "Unsaved changes to " + fi.fileName() +
            ". Save changes before closing?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (reply) {
      case QMessageBox::Cancel:
        break;
      case QMessageBox::Save:
        MainWindow::on_actionSave_triggered();
        close();
        break;
      case QMessageBox::Discard:
        close();
        break;
      default:
        break;
    }
  } else {
    close();
  }
}

void setStyleSheetFile(QFile& file) {
  file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(file.readAll());
  qApp->setStyleSheet(styleSheet);
}
void MainWindow::on_actionDark_2_triggered() {
  QFile file(MATERIALDARK);
  setStyleSheetFile(file);
}

void MainWindow::on_actionAqua_triggered() {
  QFile file(AQUA);
  setStyleSheetFile(file);
}

void MainWindow::on_actionElegantNight_triggered() {
  QFile file(ELEGANTDARK);
  setStyleSheetFile(file);
}

void MainWindow::on_actionNeon_triggered() {
  QFile file(NEONBUTTONS);
  setStyleSheetFile(file);
}

void MainWindow::on_actionWanderlust_triggered() {
  QFile file(AMOLED);
  setStyleSheetFile(file);
}

void MainWindow::on_actionPicPax_triggered() {
  QFile file(PICPAX);
  setStyleSheetFile(file);
}

void MainWindow::on_actionGenetive_triggered() {
  QFile file(GENETIVE);
  setStyleSheetFile(file);
}

void MainWindow::on_actionTCobra_triggered() {
  QFile file(TCOBRA);
  setStyleSheetFile(file);
}

void MainWindow::on_actionUndo_triggered() {
  item undo = this->imageHistory->undo();
  this->contentView->setImage(undo.img);
  this->histBW->setImage(undo.img);
  this->histRGB->setImage(undo.img);
  this->histBW->showBWHistogram();
  this->histRGB->showRGBHistogram(histFlag);
  this->contentView->setOriginal(undo.original);
  this->setSliders(undo.parms);
  this->contentView->mRubberBand->hide();
}

void MainWindow::on_actionRedo_triggered() {
  item redo = this->imageHistory->redo();
  this->contentView->setImage(redo.img);
  this->histBW->setImage(redo.img);
  this->histRGB->setImage(redo.img);
  this->histBW->showBWHistogram();
  this->histRGB->showRGBHistogram(histFlag);
  this->contentView->setOriginal(redo.original);
  this->setSliders(redo.parms);
  this->contentView->mRubberBand->hide();
}

void MainWindow::on_horizontalSliderBrightness_sliderReleased() {
  int value = this->ui->horizontalSliderBrightness->value();
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setBrightness(value);
  edited = applyAll();
  this->refreshState(edited);
}

void MainWindow::on_horizontalSliderBrightness_valueChanged(int value) {
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setBrightness(value);
  edited = applyAll();
  this->contentView->setImage(edited);
}

void MainWindow::on_actionResize_triggered() {
  QImage rImage = contentView->getImage();
  QImage rImageOG = contentView->getOriginal();
  QDialog* resizeDialog = new QDialog(this);
  Ui::ResizeWindow* imgSize = new Ui::ResizeWindow;

  imgSize->setupUi(resizeDialog);
  imgSize->widthSpin->setValue(rImage.size().width());
  imgSize->heightSpin->setValue(rImage.size().height());
  this->contentView->mRubberBand->hide();

  if (resizeDialog->exec() == QDialog::Accepted) {
    this->contentView->setOriginal(rImageOG.scaled(
        QSize(imgSize->widthSpin->value(), imgSize->heightSpin->value())));
    refreshState(rImage.scaled(
        QSize(imgSize->widthSpin->value(), imgSize->heightSpin->value())));
  }
}

void MainWindow::setBrushColor() {
  QColor selectedColor =
      QColorDialog::getColor(Qt::white, this, "Color picker");
  this->painter->setColor(selectedColor);
}

void MainWindow::setBrushOpacity(int value) {
  int red = this->painter->getColor().red();
  int green = this->painter->getColor().green();
  int blue = this->painter->getColor().blue();
  this->painter->setColor(QColor(red, green, blue, value));
}

void MainWindow::on_actionPenWidth_triggered() {
  QDialog* penWidthDialog = new QDialog(this);
  Ui::PenWidthDialog* window = new Ui::PenWidthDialog;

  this->contentView->mRubberBand->hide();
  window->setupUi(penWidthDialog);
  window->spinBox->setValue(painter->getWidth());
  window->opcitySlider->setValue(this->painter->getColor().alpha());
  window->progressBar->setValue(window->opcitySlider->value());
  if (this->painter->getGradient()) {
    window->gradientBox->setCheckState(Qt::Checked);
  }
  if (this->painter->getSquare()) {
    window->squareBox->setCheckState(Qt::Checked);
  }

  QObject::connect(window->colorButton, SIGNAL(clicked()), this,
                   SLOT(setBrushColor()));
  QObject::connect(window->opcitySlider, SIGNAL(valueChanged(int)), this,
                   SLOT(setBrushOpacity(int)));
  QObject::connect(window->opcitySlider, SIGNAL(valueChanged(int)),
                   window->progressBar, SLOT(setValue(int)));

  if (penWidthDialog->exec() == QDialog::Accepted) {
    int selectedWidth = window->spinBox->value();
    this->painter->setWidth(selectedWidth);
    if (window->gradientBox->isChecked()) {
      this->painter->setGradient(true);
    } else {
      this->painter->setGradient(false);
    }
    if (window->squareBox->isChecked()) {
      this->painter->setSquare(true);
    } else {
      this->painter->setSquare(false);
    }
  }
}

void MainWindow::on_horizontalSliderContrast_sliderReleased() {
  int value = this->ui->horizontalSliderContrast->value();
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setContrast(value);
  edited = applyAll();
  this->refreshState(edited);
}

void MainWindow::on_horizontalSliderContrast_valueChanged(int value) {
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setContrast(value);
  edited = applyAll();
  this->contentView->setImage(edited);
}

void MainWindow::on_horizontalSliderShadows_sliderReleased() {
  int value = this->ui->horizontalSliderShadows->value();
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setShadows(value);
  edited = applyAll();
  this->refreshState(edited);
}

void MainWindow::on_horizontalSliderShadows_valueChanged(int value) {
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setShadows(value);
  edited = applyAll();
  this->contentView->setImage(edited);
}

void MainWindow::on_checkRed_stateChanged() {
  if (this->ui->checkRed->isChecked()) {
    this->histFlag *= DRAW_RED;
  } else {
    this->histFlag /= DRAW_RED;
  }
  histRGB->showRGBHistogram(histFlag);
}

void MainWindow::on_checkGreen_stateChanged() {
  if (this->ui->checkGreen->isChecked()) {
    this->histFlag *= DRAW_GREEN;
  } else {
    this->histFlag /= DRAW_GREEN;
  }
  histRGB->showRGBHistogram(histFlag);
}

void MainWindow::on_checkBlue_stateChanged() {
  if (this->ui->checkBlue->isChecked()) {
    this->histFlag *= DRAW_BLUE;
  } else {
    this->histFlag /= DRAW_BLUE;
  }
  histRGB->showRGBHistogram(histFlag);
}

void MainWindow::on_HSliderColor_sliderReleased() {
  int value = this->ui->HSliderColor->value();
  QImage img = this->contentView->getImage();
  QImage edited;
  int redValue = ui->spinRed->value(), greenValue = ui->spinGreen->value(),
      blueValue = ui->spinBlue->value();
  parameters->setHueInt(value);

  if (redValue < 0) {
    redValue = 0;
  }
  if (redValue > PIX_MAX) {
    redValue = PIX_MAX;
  }
  if (greenValue < 0) {
    greenValue = 0;
  }
  if (greenValue > PIX_MAX) {
    greenValue = PIX_MAX;
  }
  if (blueValue < 0) {
    blueValue = 0;
  }
  if (blueValue > PIX_MAX) {
    blueValue = PIX_MAX;
  }
  parameters->setRed(redValue);
  parameters->setGreen(greenValue);
  parameters->setBlue(blueValue);
  edited = applyAll();
  this->refreshState(edited);
}

void MainWindow::on_HSliderColor_valueChanged(int value) {
  QImage img = this->contentView->getImage();
  QImage edited;
  int redValue = ui->spinRed->value(), greenValue = ui->spinGreen->value(),
      blueValue = ui->spinBlue->value();
  parameters->setHueInt(value);

  if (redValue < 0) {
    redValue = 0;
  }
  if (redValue > PIX_MAX) {
    redValue = PIX_MAX;
  }
  if (greenValue < 0) {
    greenValue = 0;
  }
  if (greenValue > PIX_MAX) {
    greenValue = PIX_MAX;
  }
  if (blueValue < 0) {
    blueValue = 0;
  }
  if (blueValue > PIX_MAX) {
    blueValue = PIX_MAX;
  }
  parameters->setRed(redValue);
  parameters->setGreen(greenValue);
  parameters->setBlue(blueValue);
  edited = applyAll();
  this->contentView->setImage(edited);
}

void MainWindow::on_horizontalSliderHighlights_sliderReleased() {
  int value = this->ui->horizontalSliderHighlights->value();
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setHighlights(value);
  edited = applyAll();
  this->refreshState(edited);
}

void MainWindow::on_horizontalSliderHighlights_valueChanged(int value) {
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setHighlights(value);
  edited = applyAll();
  this->contentView->setImage(edited);
}

void MainWindow::on_actionBrush_triggered() {
  this->contentView->mRubberBand->hide();
  this->painter->triggerBrushDrawing();
}

void MainWindow::on_actionEllipse_triggered() {
  this->contentView->mRubberBand->hide();
  this->painter->triggerEllipseDrawing();
}

void MainWindow::on_actionRectangle_triggered() {
  this->contentView->mRubberBand->hide();
  this->painter->triggerRectangleDrawing();
}

void MainWindow::on_actionLine_triggered() {
  this->contentView->mRubberBand->hide();
  this->painter->triggerLineDrawing();
}

void MainWindow::on_actionNew_triggered() {
  QDialog* resizeDialog = new QDialog(this);
  Ui::ResizeWindow* resize = new Ui::ResizeWindow;
  resize->setupUi(resizeDialog);
  if (resizeDialog->exec() == QDialog::Accepted) {
    QPixmap newImage =
        QPixmap(resize->widthSpin->value(), resize->heightSpin->value());
    newImage.fill();
    QImage image = newImage.toImage();
    contentView->setOriginal(image);
    refreshState(image);
    enableUi();
  }
}

void MainWindow::on_actionNegative_triggered() { 
    QImage img = this->contentView->getImage();
    QImage edited;
    edited = Tools::negative(img);
    this->contentView->setOriginal(Tools::negative(this->contentView->getOriginal()));
    edited = applyAll();
    this->refreshState(edited);
    this->contentView->setImage(edited);
}

void MainWindow::on_actionMirrorX_triggered() {
    QImage img = this->contentView->getImage();
    QImage edited;
    edited = Tools::mirrorX(img);
    this->contentView->setOriginal(Tools::mirrorX(this->contentView->getOriginal()));
    edited = applyAll();
    this->refreshState(edited);
    this->contentView->setImage(edited);
}

void MainWindow::on_actionMirrorY_triggered() {
    QImage img = this->contentView->getImage();
    QImage edited;
    edited = Tools::mirrorY(img);
    this->contentView->setOriginal(Tools::mirrorY(this->contentView->getOriginal()));
    edited = applyAll();
    this->refreshState(edited);
    this->contentView->setImage(edited);
}

void MainWindow::on_actionShortcuts_triggered() {
  this->contentView->mRubberBand->hide();
  QMessageBox msgBox;
  msgBox.setText(SHORTCUTS_HTML);
  msgBox.exec();
}

void MainWindow::on_actionAbout_CHIMP_triggered() {
  this->contentView->mRubberBand->hide();
  QMessageBox msgBox;
  QPixmap img(LOGO_PATH);
  msgBox.setIconPixmap(
      img.scaled(400, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
  msgBox.setWindowTitle("About CHIMP");
  msgBox.setText("<b>CHIMP IMAGE EDITOR</b>");
  msgBox.setInformativeText(ABOUT_CHIMP_TEXT);
  msgBox.exec();
}

void MainWindow::on_actionNew_Window_triggered() {
  MainWindow* mw = new MainWindow();
  mw->show();
}

void MainWindow::on_actionRotateRight90_triggered() {
    QImage img = this->contentView->getImage();
    QImage edited;
    edited = Tools::rotateRight90(img);
    this->contentView->setOriginal(Tools::rotateRight90(this->contentView->getOriginal()));
    edited = applyAll();
    this->refreshState(edited);
    this->contentView->setImage(edited);
}

void MainWindow::on_actionRotateLeft90_triggered() {
    QImage img = this->contentView->getImage();
    QImage edited;
    edited = Tools::rotateLeft90(img);
    this->contentView->setOriginal(Tools::rotateLeft90(this->contentView->getOriginal()));
    edited = applyAll();
    this->refreshState(edited);
    this->contentView->setImage(edited);

}

void MainWindow::on_horizontalSliderSaturation_sliderReleased() {
  int value = this->ui->horizontalSliderSaturation->value();
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setSaturation(value);
  edited = applyAll();
  this->refreshState(edited);
}

void MainWindow::on_horizontalSliderSaturation_valueChanged(int value) {
  QImage img = this->contentView->getImage();
  QImage edited;
  parameters->setSaturation(value);
  edited = applyAll();
  this->contentView->setImage(edited);
}

void MainWindow::on_actionSelection_triggered() {
  this->contentView->setSelection(true);
  this->painter->setCurrentTool(ContentView::NO_TOOL);
}

void MainWindow::on_actionCrop_triggered() {
  if (contentView->getSelectionRect().isEmpty()) {
    QMessageBox::warning(this, "Crop",
                         "Please select area by pressing selection tool",
                         QMessageBox::Ok);
    return;
  }

  this->contentView->setOriginal(Tools::crop(
      this->contentView->getOriginal(), this->contentView->getSelectionRect()));
  refreshState(Tools::crop(this->contentView->getImage(),
                           this->contentView->getSelectionRect()));
}

void MainWindow::on_actionOpen_Project_triggered() {
  bool ind = true;
  if (!this->imageHistory->isSaved()) {
    QFileInfo fi(this->contentView->getFilePath());
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "CHIMP",
        "Unsaved changes found. Save changes to " + fi.fileName() +
            " before exiting?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (reply) {
      case QMessageBox::Cancel:
        ind = false;
        break;
      case QMessageBox::Save:
        MainWindow::on_actionSave_triggered();
        ind = true;
        break;
      case QMessageBox::Discard:
        ind = true;
        break;
      default:
        ind = true;
        break;
    }
  }
  if (ind) {
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Chim projects ( *.chimp)"));
    dialog.setViewMode(QFileDialog::Detail);
    QStringList fileName;
    if (dialog.exec()) {
      fileName = dialog.selectedFiles();
      QFile file(fileName.first());
      file.open(QIODevice::ReadWrite);
      QDataStream in(&file);

      delete this->imageHistory;
      this->imageHistory = new ImageHistory(in);

      on_actionUndo_triggered();
      on_actionRedo_triggered();
      on_actionRedo_triggered();
      on_actionUndo_triggered();
      enableUi();
    }
  }
}

void MainWindow::on_actionSave_Project_triggered() {
  QString savePath = QFileDialog::getSaveFileName(
      this, tr("Save File"), "", tr("CHIMP project file (*.CHIMP *.chimp)"));
  QFile file(savePath);
  file.open(QIODevice::ReadWrite);
  QDataStream out(&file);
  this->imageHistory->serialize(out);
  file.close();
}

void MainWindow::on_toolValueChanged() {
  refreshState(this->contentView->getImage());
  // contentView->setImage(contentView->getOriginal());
}

void MainWindow::on_actionClose_2_triggered() {
  if (!this->imageHistory->isSaved()) {
    QFileInfo fi(this->contentView->getFilePath());
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "CHIMP",
        "Unsaved changes to:" + fi.fileName() +
            ". Save changes before closing?",
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (reply) {
      case QMessageBox::Cancel:
        break;
      case QMessageBox::Save:
        MainWindow::on_actionSave_triggered();
        close();
        break;
      case QMessageBox::Discard:
        close();
        break;
      default:
        break;
    }
  } else {
    close();
  }
}

void MainWindow::on_pushButton_pressed() {
  QColor selectedC = QColorDialog::getColor(Qt::white, this, "Color picker");
  this->ui->spinRed->setValue(selectedC.red());
  this->ui->spinGreen->setValue(selectedC.green());
  this->ui->spinBlue->setValue(selectedC.blue());
  this->ui->HSliderColor->setValue(0);
}
