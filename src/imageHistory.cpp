#include "hdr/imageHistory.h"

#include "hdr/constants.h"

void ImageHistory::newEdit(const item &newImage) {
  this->undoHistory->push_back(newImage);
  this->redoHistory->clear();
  if (undoHistory->size() == MAX_HISTORY) {
    undoHistory->pop_front();
  }
  this->saved = false;
}
item ImageHistory::undo() {
  if (undoHistory->size() == 1) {
    return undoHistory->back();
  }
  this->redoHistory->append(undoHistory->back());
  undoHistory->pop_back();
  return (undoHistory->back());
}
item ImageHistory::redo() {
  if (this->redoHistory->empty()) {
    return undoHistory->back();
  }
  if (this->redoHistory->size() == 1) {
    return redoHistory->back();
  }

  item ret = redoHistory->back();
  undoHistory->append(ret);
  redoHistory->pop_back();
  return ret;

}

void ImageHistory::serialize(QDataStream &out) {
  out << (qint32)undoHistory->length();
  QList<item>::iterator i;
  for (i = undoHistory->begin(); i != undoHistory->end(); ++i) {
    i->serialize(out);
  }
  out << (qint32)redoHistory->length();
  for (i = redoHistory->begin(); i != redoHistory->end(); ++i) {
    i->serialize(out);
  }
}

ImageHistory::ImageHistory(QDataStream &in) {
  qint32 undoLength;
  qint32 redoLength;
  in >> undoLength;
  this->undoHistory = new QList<item>();
  this->redoHistory = new QList<item>();

  for (int i = 0; i < undoLength; i++) {
    item newItem = item(in);
    undoHistory->append(newItem);
  }

  in >> redoLength;
  for (int i = 0; i < redoLength; i++) {
    redoHistory->append(item(in));
  }
}
