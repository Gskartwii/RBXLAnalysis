#ifndef _RAY_CPP
#define _RAY_CPP

class Ray {
  float originX, originY, originZ;
  float directionX, directionY, directionZ;

public:
  void setOriginX(float);
  void setOriginY(float);
  void setOriginZ(float);

  void setDirectionX(float);
  void setDirectionY(float);
  void setDirectionZ(float);

  float getOriginX();
  float getOriginY();
  float getOriginZ();

  float getDirectionX();
  float getDirectionY();
  float getDirectionZ();

  Ray& operator=(Ray*);
};

Ray& Ray::operator=(Ray* n) {
  originX = n->getOriginX();
  originY = n->getOriginY();
  originZ = n->getOriginZ();
  directionX = n->getDirectionX();
  directionY = n->getDirectionY();
  directionZ = n->getDirectionZ();

  return *this;
}

void Ray::setOriginX(float n) {
  originX = n;
}

void Ray::setOriginY(float n) {
  originY = n;
}

void Ray::setOriginZ(float n) {
  originZ = n;
}

void Ray::setDirectionX(float n) {
  directionX = n;
}
void Ray::setDirectionY(float n) {
  directionY = n;
}
void Ray::setDirectionZ(float n) {
  directionZ = n;
}

float Ray::getOriginX() {
  return originX;
}
float Ray::getOriginY() {
  return originY;
}
float Ray::getOriginZ() {
  return originZ;
}
float Ray::getDirectionX() {
  return directionX;
}
float Ray::getDirectionY() {
  return directionY;
}
float Ray::getDirectionZ() {
  return directionZ;
}
#endif
