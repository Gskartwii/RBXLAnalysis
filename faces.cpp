#ifndef _FACES_CPP
#define _FACES_CPP

class Faces {
  bool right, top, back, left, bottom, front;

public:
  void setRight(bool);
  void setTop(bool);
  void setBack(bool);
  void setLeft(bool);
  void setBottom(bool);
  void setFront(bool);

  bool getRight();
  bool getTop();
  bool getBack();
  bool getLeft();
  bool getBottom();
  bool getFront();

  Faces& operator=(Faces*);
};

Faces& Faces::operator=(Faces* n) {
  right = n->getRight();
  top = n->getTop();
  back = n->getBack();
  left = n->getLeft();
  bottom = n->getBottom();
  front = n->getFront();

  return *this;
}

void Faces::setRight(bool n) {
  right = n;
}
void Faces::setTop(bool n) {
  top = n;
}
void Faces::setBack(bool n) {
  back = n;
}
void Faces::setLeft(bool n) {
  left = n;
}
void Faces::setBottom(bool n) {
  bottom = n;
}
void Faces::setFront(bool n) {
  front = n;
}

bool Faces::getRight() {
  return right;
}
bool Faces::getTop() {
  return top;
}
bool Faces::getBack() {
  return back;
}
bool Faces::getLeft() {
  return left;
}
bool Faces::getBottom() {
  return bottom;
}
bool Faces::getFront() {
  return front;
}

#endif
