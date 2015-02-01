#ifndef _AXIS_CPP
#define _AXIS_CPP

class Axis {
  bool x, y, z;


public:
  void setX(bool);
  void setY(bool);
  void setZ(bool);

  bool getX();
  bool getY();
  bool getZ();

  Axis& operator=(Axis*);
};

Axis& Axis::operator=(Axis* n) {
  x = n->getX();
  y = n->getY();
  z = n->getZ();

  return *this;
}

void Axis::setX(bool n) {
  x = n;
}
void Axis::setY(bool n) {
  y = n;
}
void Axis::setZ(bool n) {
  z = n;
}

bool Axis::getX() {
  return x;
}
bool Axis::getY() {
  return y;
}
bool Axis::getZ() {
  return z;
}

#endif
