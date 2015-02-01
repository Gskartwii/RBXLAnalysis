#ifndef _COLOR3_CPP
#define _COLOR3_CPP

class Color3 {
  float r, g, b;

public:
  void setR(float);
  void setG(float);
  void setB(float);

  float getR();
  float getG();
  float getB();
  int   getRInt();
  int   getGInt();
  int   getBInt();

  Color3& operator=(Color3*);
};

Color3& Color3::operator=(Color3* n) {
  r = n->getR();
  g = n->getG();
  b = n->getB();

  return *this;
}

void Color3::setR(float n) {
  r = n;
}
void Color3::setG(float n) {
  g = n;
}
void Color3::setB(float n) {
  b = n;
}

float Color3::getR() {
  return r;
}
float Color3::getG() {
  return g;
}
float Color3::getB() {
  return b;
}
int Color3::getRInt() {
  return (int) r * 255;
}
int Color3::getGInt() {
  return (int) r * 255;
}
int Color3::getBInt() {
  return (int) r * 255;
}

#endif
