#ifndef _PROPERTY_CPP
#define _PROPERTY_CPP
#include <string>

#include "udim2.cpp"
#include "ray.cpp"
#include "faces.cpp"
#include "axis.cpp"
#include "brickColor.cpp"
#include "color3.cpp"
#include "vector2.cpp"
#include "vector3.cpp"
#include "cframe.cpp"

using namespace std;

class Property {
  string stringValue;
  bool boolValue;
  int intValue;
  float floatValue;
  double doubleValue;
  UDim2 udim2Value;
  Ray rayValue;
  Faces facesValue;
  Axis axisValue;
  BrickColor brickColorValue;
  Color3 color3Value;
  Vector2 vector2Value;
  Vector3 vector3Value;
  CFrame cframeValue;
  int enumValue;
  int referentValue;

  int typeID;
  int propertyNameLength;
  string propertyName;
  int dataType;

  int compressedLength;
  int decompressedLength;
  string rawData;

public:
  void setCompressedLength(int);
  void setDecompressedLength(int);
  void setRawData(string);

  void setStringValue(string);
  void setBoolValue(bool);
  void setIntValue(int);
  void setFloatValue(float);
  void setDoubleValue(double);
  void setUDim2Value(UDim2);
  void setRayValue(Ray);
  void setFacesValue(Faces);
  void setAxisValue(Axis);
  void setBrickColorValue(BrickColor);
  void setColor3Value(Color3);
  void setVector2Value(Vector2);
  void setVector3Value(Vector3);
  void setCFrameValue(CFrame);
  void setEnumValue(int);
  void setReferentValue(int);

  void setTypeID(int);
  void setPropertyNameLength(int);
  void setPropertyName(string);
  void setDataType(int);

  int getCompressedLength();
  int getDecompressedLength();
  string getRawData();

  string getStringValue();
  bool getBoolValue();
  int getIntValue();
  float getFloatValue();
  double getDoubleValue();
  UDim2 getUDim2Value();
  Ray getRayValue();
  Faces getFacesValue();
  Axis getAxisValue();
  BrickColor getBrickColorValue();
  Color3 getColor3Value();
  Vector2 getVector2Value();
  Vector3 getVector3Value();
  CFrame getCFrameValue();
  int getEnumValue();
  int getReferentValue();

  int getTypeID();
  int getPropertyNameLength();
  string getPropertyName();
  int getDataType();

  Property();
  ~Property();

  Property& operator=(Property*);
};

Property::Property() {

}
Property::~Property() {

}

void Property::setCompressedLength(int n) {
  compressedLength = n;
}
void Property::setDecompressedLength(int n) {
  decompressedLength = n;
}
void Property::setRawData(string n) {
  rawData = n;
}
int Property::getCompressedLength() {
  return compressedLength;
}
int Property::getDecompressedLength() {
  return decompressedLength;
}
string Property::getRawData() {
  return rawData;
}

Property& Property::operator=(Property* n) {
  stringValue = n->getStringValue();
  boolValue = n->getBoolValue();
  intValue = n->getIntValue();
  floatValue = n->getFloatValue();
  doubleValue = n->getDoubleValue();
  udim2Value = n->getUDim2Value();
  rayValue = n->getRayValue();
  facesValue = n->getFacesValue();
  axisValue = n->getAxisValue();
  brickColorValue = n->getBrickColorValue();
  color3Value = n->getColor3Value();
  vector2Value = n->getVector2Value();
  vector3Value = n->getVector3Value();
  cframeValue = n->getCFrameValue();
  enumValue = n->getEnumValue();
  referentValue = n->getReferentValue();

  typeID = n->getTypeID();
  propertyNameLength = n->getPropertyNameLength();
  propertyName = n->getPropertyName();
  dataType = n->getDataType();

  return *this;
}

void Property::setTypeID(int n) {
  typeID = n;
}
void Property::setPropertyNameLength(int n) {
  propertyNameLength = n;
}
void Property::setPropertyName(string n) {
  propertyName = n;
}
void Property::setDataType(int n) {
  dataType = n;
}

int Property::getTypeID() {
  return typeID;
}
int Property::getPropertyNameLength() {
  return propertyNameLength;
}
string Property::getPropertyName() {
  return propertyName;
}
int Property::getDataType() {
  return dataType;
}

void Property::setStringValue(string n) {
  stringValue = n;
}
void Property::setBoolValue(bool n) {
  boolValue = n;
}
void Property::setIntValue(int n) {
  intValue = n;
}
void Property::setFloatValue(float n) {
  floatValue = n;
}
void Property::setDoubleValue(double n) {
  doubleValue = n;
}
void Property::setUDim2Value(UDim2 n) {
  udim2Value = n;
}
void Property::setRayValue(Ray n) {
  rayValue = n;
}
void Property::setFacesValue(Faces n) {
  facesValue = n;
}
void Property::setAxisValue(Axis n) {
  axisValue = n;
}
void Property::setBrickColorValue(BrickColor n) {
  brickColorValue = n;
}
void Property::setColor3Value(Color3 n) {
  color3Value = n;
}
void Property::setVector2Value(Vector2 n) {
  vector2Value = n;
}
void Property::setVector3Value(Vector3 n) {
  vector3Value = n;
}
void Property::setCFrameValue(CFrame n) {
  cframeValue = n;
}
void Property::setEnumValue(int n) {
  enumValue = n;
}
void Property::setReferentValue(int n) {
  referentValue = n;
}

string Property::getStringValue() {
  return stringValue;
}
bool Property::getBoolValue() {
  return boolValue;
}
int Property::getIntValue() {
  return intValue;
}
float Property::getFloatValue() {
  return floatValue;
}
double Property::getDoubleValue() {
  return doubleValue;
}
UDim2 Property::getUDim2Value() {
  return udim2Value;
}
Ray Property::getRayValue() {
  return rayValue;
}
Faces Property::getFacesValue() {
  return facesValue;
}
Axis Property::getAxisValue() {
  return axisValue;
}
BrickColor Property::getBrickColorValue() {
  return brickColorValue;
}
Color3 Property::getColor3Value() {
  return color3Value;
}
Vector2 Property::getVector2Value() {
  return vector2Value;
}
Vector3 Property::getVector3Value() {
  return vector3Value;
}
CFrame Property::getCFrameValue() {
  return cframeValue;
}
int Property::getEnumValue() {
  return enumValue;
}
int Property::getReferentValue() {
  return referentValue;
}

#endif
