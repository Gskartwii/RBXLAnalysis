#ifndef _VECTOR2_CPP
#define _VECTOR2_CPP

class Vector2 {
  float x, y;

public:
  void setX(float);
  void setY(float);

  float getX();
  float getY();

  Vector2& operator=(Vector2*);
};

Vector2& Vector2::operator=(Vector2* n) {
  x = n->getX();
  y = n->getY();

  return *this;
}

void Vector2::setX(float n) {
  x = n;
}
void Vector2::setY(float n) {
  y = n;
}

float Vector2::getX() {
  return x;
}
float Vector2::getY() {
  return y;
}

#endif
