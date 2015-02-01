#ifndef _UDIM2_CPP
#define _UDIM2_CPP

class UDim2 {
  float scaleX;
  int offsetX;
  float scaleY;
  int offsetY;

public:
  void setScaleX(float);
  void setOffsetX(int);
  void setScaleY(float);
  void setOffsetY(int);

  float getScaleX();
  int getOffsetX();
  float getScaleY();
  int getOffsetY();

  UDim2& operator=(UDim2*);
};

UDim2& UDim2::operator=(UDim2* n) {
  scaleX = n->getScaleX();
  scaleY = n->getScaleY();
  offsetX = n->getOffsetX();
  offsetY = n->getOffsetY();

  return *this;
}

void UDim2::setScaleX(float n) {
  scaleX = n;
}
void UDim2::setOffsetX(int n) {
  offsetX = n;
}
void UDim2::setScaleY(float n) {
  scaleY = n;
}
void UDim2::setOffsetY(int n) {
  offsetY = n;
}

float UDim2::getScaleX() {
  return scaleX;
}
int UDim2::getOffsetX() {
  return offsetX;
}
float UDim2::getScaleY() {
  return scaleY;
}
int UDim2::getOffsetY() {
  return offsetY;
}

#endif
