#include <QApplication>
#include <QFile>

#include "hdr/mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // App style:
  QFile file(":/stylesheets/qss/Genetive.qss");
  file.open(QFile::ReadOnly);
  QString styleSheet = QLatin1String(file.readAll());
  qApp->setStyleSheet(styleSheet);
  MainWindow w;
  w.show();
  return a.exec();
}
