#ifndef _INSTANCE_CPP
#define _INSTANCE_CPP
#include <vector>
#include <string>
#include <algorithm>
#include "property.cpp"

using namespace std;

class Instance {
  vector<Property> properties;
  vector<string>   propertyNames;
  int compressedLen, decompressedLen, typeID, typeNameLength, additionalData, referent, numReferents, parentReferent;
  string rawData, typeName, additionalDataContent;

public:
  void setCompressedLen(int);
  void setDecompressedLen(int);
  void setTypeID(int);
  void setTypeNameLength(int);
  void setAdditionalData(int);
  void setReferent(int);
  void setNumReferents(int);

  void setRawData(string);
  void setTypeName(string);
  void setAdditionalDataContent(string);

  int getCompressedLen();
  int getDecompressedLen();
  int getTypeID();
  int getTypeNameLength();
  int getAdditionalData();
  int getReferent();
  int getNumReferents();
  string getRawData();
  string getTypeName();
  string getAdditionalDataContent();
  string getName();

  //void setProperty(int, Property);
  void setPropertyName(int, string);
  void addProperty(Property);
  void addPropertyName(string);

  Instance& operator=(Instance*);

  Property getProperty(int);
  string getPropertyName(int);
  int getPropertyCount();

  void setParentReferent(int);
  int getParentReferent();

  Instance();
  ~Instance();
};

Instance::Instance() {

}

Instance::~Instance() {

}

string Instance::getName() {
	return properties[propertyNames.begin() - find(propertyNames.begin(), propertyNames.end(), "Name")].getStringValue();
}

void Instance::setParentReferent(int n) {
  parentReferent = n;
}
int Instance::getParentReferent() {
  return parentReferent;
}

int Instance::getPropertyCount() {
  return propertyNames.size();
}

Instance& Instance::operator=(Instance* n) {
  compressedLen = n->getCompressedLen();
  decompressedLen = n->getDecompressedLen();
  typeID = n->getTypeID();
  typeNameLength = n->getTypeNameLength();
  additionalData = n->getAdditionalData();
  referent = n->getReferent();
  rawData = n->getRawData();
  typeName = n->getTypeName();
  additionalDataContent = n->getAdditionalDataContent();
  return *this;
}

void Instance::setCompressedLen(int n) {
  compressedLen = n;
}
void Instance::setDecompressedLen(int n) {
  decompressedLen = n;
}
void Instance::setTypeID(int n) {
  typeID = n;
}
void Instance::setTypeNameLength(int n) {
  typeNameLength = n;
}
void Instance::setAdditionalData(int n) {
  additionalData = n;
}
void Instance::setReferent(int n) {
  referent = n;
}
void Instance::setNumReferents(int n) {
  numReferents = n;
}

void Instance::setRawData(string n) {
  rawData = n;
}
void Instance::setTypeName(string n) {
  typeName = n;
}
void Instance::setAdditionalDataContent(string n) {
  additionalDataContent = n;
}

int Instance::getCompressedLen() {
  return compressedLen;
}
int Instance::getDecompressedLen() {
  return decompressedLen;
}
int Instance::getTypeID() {
  return typeID;
}
int Instance::getTypeNameLength() {
  return typeNameLength;
}
int Instance::getAdditionalData() {
  return additionalData;
}
int Instance::getReferent() {
  return referent;
}
int Instance::getNumReferents() {
  return numReferents;
}

string Instance::getRawData() {
  return rawData;
}
string Instance::getTypeName() {
  return typeName;
}
string Instance::getAdditionalDataContent() {
  return additionalDataContent;
}

void Instance::setPropertyName(int i, string n) {
  propertyNames[i] = n;
}
void Instance::addProperty(Property n) {
  properties.push_back(n);
}
void Instance::addPropertyName(string n) {
  propertyNames.push_back(n);
}

Property Instance::getProperty(int i) {
  return properties[i];
}
string Instance::getPropertyName(int i) {
  return propertyNames[i];
}
#endif
