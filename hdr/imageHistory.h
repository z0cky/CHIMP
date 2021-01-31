#ifndef IMAGEHISTORY_H
#define IMAGEHISTORY_H

#include <QDataStream>
#include <QImage>
#include <QList>

#include "hdr/parameters.hpp"

using namespace std;
struct item {
  Parameters parms;
  QImage img;
  QImage original;

  void serialize(QDataStream &out) {
    parms.serialize(out);
    out << img;
    out << original;
  }
  item(QDataStream &in) {
    parms = Parameters(in);
    in >> img;
    in >> original;
  }
  item() {
  }
};

class ImageHistory {
 public:
  ImageHistory() {
    this->undoHistory = new QList<item>();
    this->redoHistory = new QList<item>();
    this->saved = true;
  }
  ~ImageHistory() {
    delete undoHistory;
    delete redoHistory;
  }

  ImageHistory(QDataStream &in);
  void serialize(QDataStream &in);

  item undo();  // returns the previous image, adds current to redo list
  item redo();
  void newEdit(const item &);
  bool isSaved() {
    return this->saved;
  }

 private:
  QList<item> *undoHistory;
  QList<item> *redoHistory;
  bool saved;
};

#endif  // IMAGEHISTORY_H
