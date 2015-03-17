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
  Instance(int, int, int, int, int, int, int, int, string, string, string);
  ~Instance();
};

Instance::Instance() {

}
Instance::~Instance() {

}

Instance::Instance(int _compressedLen, int _decompressedLen, int _typeID, int _typeNameLength, int _additionalData, int _referent, int _numReferents, int _parentReferent, string _rawData, string _typeName, string _additionalDataContent) {
    compressedLen       = _compressedLen;
    decompressedLen     = _decompressedLen;
    typeID              = _typeID;
    typeNameLength      = _typeNameLength;
    additionalData      = _additionalData;
    referent            = _referent;
    numReferents        = _numReferents;
    parentReferent      = _parentReferent;
    rawData             = _rawData;
    typeName            = _typeName;
    additionalDataContent = _additionalDataContent;
}

string Instance::getName() {
	unsigned int i;
    for (i = 0; i < properties.size(); i++) {
        if (properties[i].getPropertyName() == "Name") {
            //cout << "TYPE:" << properties[i].getTypeID() << endl;
            return properties[i].getStringValue();
        }
    }
    return "UNKNOWN";
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
  int i;
  compressedLen = n->getCompressedLen();
  decompressedLen = n->getDecompressedLen();
  typeID = n->getTypeID();
  typeNameLength = n->getTypeNameLength();
  additionalData = n->getAdditionalData();
  referent = n->getReferent();
  rawData = n->getRawData();
  typeName = n->getTypeName();
  additionalDataContent = n->getAdditionalDataContent();
  parentReferent = n->getParentReferent();
  properties.clear();
  propertyNames.clear();
  for (i = 0; i < n->getPropertyCount(); i++) {
      properties.push_back(n->getProperty(i));
      propertyNames.push_back(n->getPropertyName(i));
  }
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
