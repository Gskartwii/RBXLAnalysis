#ifndef _CFRAME_CPP
#define _CFRAME_CPP
#include <cmath>

class CFrame {
  float x, y, z;
  float r00, r01, r02, r10, r11, r12, r20, r21, r22;

public:
  void setX(float);
  void setY(float);
  void setZ(float);

  void setR00(float);
  void setR01(float);
  void setR02(float);
  void setR10(float);
  void setR11(float);
  void setR12(float);
  void setR20(float);
  void setR21(float);
  void setR22(float);

  float getX();
  float getY();
  float getZ();

  float getR00();
  float getR01();
  float getR02();
  float getR10();
  float getR11();
  float getR12();
  float getR20();
  float getR21();
  float getR22();

  float getXRot();
  float getYRot();
  float getZRot();

  CFrame& operator=(CFrame*);
};

CFrame& CFrame::operator=(CFrame* n) {
  x = n->getX();
  y = n->getY();
  z = n->getZ();

  r00 = n->getR00();
  r01 = n->getR01();
  r02 = n->getR02();
  r10 = n->getR10();
  r11 = n->getR11();
  r12 = n->getR12();
  r20 = n->getR20();
  r21 = n->getR21();
  r22 = n->getR22();

  return *this;
}

// The following three functions are based on http://nghiaho.com/uploads/code/rotation_matrix_demo.m
float CFrame::getXRot() {
  return (float) atan2(r21, r22);
}
float CFrame::getYRot() {
  return (float) atan2(0 - r20, sqrt(r21 * r21 + r22 * r22));
}
float CFrame::getZRot() {
  return (float) atan2(r10, r00);
}

void CFrame::setX(float n) {
  x = n;
}
void CFrame::setY(float n) {
  y = n;
}
void CFrame::setZ(float n) {
  z = n;
}

void CFrame::setR00(float n) {
  r00 = n;
}
void CFrame::setR01(float n) {
  r01 = n;
}
void CFrame::setR02(float n) {
  r02 = n;
}
void CFrame::setR10(float n) {
  r10 = n;
}
void CFrame::setR11(float n) {
  r11 = n;
}
void CFrame::setR12(float n) {
  r12 = n;
}
void CFrame::setR20(float n) {
  r20 = n;
}
void CFrame::setR21(float n) {
  r21 = n;
}
void CFrame::setR22(float n) {
  r22 = n;
}

float CFrame::getX() {
  return x;
}
float CFrame::getY() {
  return y;
}
float CFrame::getZ() {
  return z;
}

float CFrame::getR00() {
  return r00;
}
float CFrame::getR01() {
  return r01;
}
float CFrame::getR02() {
  return r02;
}
float CFrame::getR10() {
  return r10;
}
float CFrame::getR11() {
  return r11;
}
float CFrame::getR12() {
  return r12;
}
float CFrame::getR20() {
  return r20;
}
float CFrame::getR21() {
  return r21;
}
float CFrame::getR22() {
  return r22;
}

#endif
