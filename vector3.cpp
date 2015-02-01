#ifndef _VECTOR3_CPP
#define _VECTOR3_CPP

class Vector3 {
  float x, y, z;

public:
  void setX(float);
  void setY(float);
  void setZ(float);

  float getX();
  float getY();
  float getZ();

  Vector3& operator=(Vector3*);
};

Vector3& Vector3::operator=(Vector3* n) {
  x = n->getX();
  y = n->getY();
  z = n->getZ();

  return *this;
}

void Vector3::setX(float n) {
  x = n;
}
void Vector3::setY(float n) {
  y = n;
}
void Vector3::setZ(float n) {
  z = n;
}

float Vector3::getX() {
  return x;
}
float Vector3::getY() {
  return y;
}
float Vector3::getZ() {
  return z;
}

#endif
