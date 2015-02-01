#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sys/stat.h>
#include <iomanip>
#include <sstream>
#include "instance.cpp"

static ofstream currObjectStream;

static vector<Instance> gameObjects;
static vector<int> usedspots;
int voidFreadValue = 0; // To silence g++ warnings about ignoring fread return values

union doubleConverter {
  double d;
  char bytes[8 * sizeof(char)];
} u;

union floatConverter {
  float f;
  unsigned int hexval;
} f;

using namespace std;

int countLines(string str) {
    return count(str.begin(), str.end(), '\n');
}

void dumpSource(string path, string source) {
    ofstream sourceStream;
    sourceStream.open((path + "source.lua").c_str());
    sourceStream << "-- RBXL Analysis Object Property Extraction" << endl;
    sourceStream << "-- This file is not meant to be edited manually" << endl;
    sourceStream << "-- Source dump: (" << countLines(source) << " Lines)" << endl;
    sourceStream << source << endl;
    sourceStream.close();
}

string hexVal(int a) {
    stringstream ret;
    ret << "0x" << hex << setiosflags(ios::uppercase);
    ret << setw(8) << setfill('0') << a;
    return ret.str();
}

string hexValFloat(float a) {
    f.f = a;
    stringstream ret;
    ret << "0x" << hex << setiosflags(ios::uppercase);
    ret << setw(8) << setfill('0') << f.hexval;
    return ret.str();
}

string hexValDouble(double a) {
    u.d = a;
    stringstream ret;
    ret << "0x";
    int i;
    for (i = 0; i < 8; i++) {
        ret << hex << setiosflags(ios::uppercase) << setw(2) << setfill('0') << (int)u.bytes[i];
    }
    return ret.str();
}

string dumpFaces(Faces a) {
    string ret;
    if (a.getRight()) {
        ret += ", Right";
    }
    if (a.getTop()) {
        ret += ", Top";
    }
    if (a.getBack()) {
        ret += ", Back";
    }
    if (a.getLeft()) {
        ret += ", Left";
    }
    if (a.getBottom()) {
        ret += ", Bottom";
    }
    if (a.getFront()) {
        ret += ", Front";
    }
    if (!ret.length()) {
        return "Empty";
    }
    else {
        return ret.substr(2);
    }
}

string dumpAxis(Axis a) {
    string ret;
    if (a.getZ()) {
        ret += ", Z";
    }
    if (a.getY()) {
        ret += ", Y";
    }
    if (a.getX()) {
        ret += ", X";
    }
    if (!ret.length()) {
        return "Empty";
    }
    else {
        return ret.substr(2);
    }
}

string dumpCFrame(CFrame a) {
    stringstream ret;
    ret << string("X: ") << a.getX() << string(" (") << hexValFloat(a.getX()) << "), ";
    ret << string("Y: ") << a.getY() << string(" (") << hexValFloat(a.getY()) << "), ";
    ret << string("Z: ") << a.getZ() << string(" (") << hexValFloat(a.getZ()) << "), ";
    ret << string("Rotation Angles: {") << a.getXRot() << string(", ") << a.getYRot() << string(", ") << a.getZRot() << "}, ";
    ret << string("Rotation Matrix: {") << a.getR00() << string(", ") << a.getR01() << string(", ") << a.getR02() << string(", ") << a.getR10() << string(", ") << a.getR11() << string(", ") << a.getR12() << string(", ") << a.getR20() << string(", ") << a.getR21() << string(", ") << a.getR22() << "}, ";
    ret << string("Rotation Angles Hex: {") << hexValFloat(a.getXRot()) << string(", ") << hexValFloat(a.getYRot()) << string(", ") << hexValFloat(a.getZRot()) << "}, ";
    ret << string("Rotation Matrix Hex: {") << hexValFloat(a.getR00()) << string(", ") << hexValFloat(a.getR01()) << string(", ") << hexValFloat(a.getR02()) << string(", ") << hexValFloat(a.getR10()) << string(", ") << hexValFloat(a.getR11()) << string(", ") << hexValFloat(a.getR12()) << string(", ") << hexValFloat(a.getR20()) << string(", ") << hexValFloat(a.getR21()) << string(", ") << hexValFloat(a.getR22()) << "}";
    return ret.str();
}

string getExtractedPath(int inst) {
  string ret = gameObjects[inst].getName() + string(".") + gameObjects[inst].getTypeName() + string("/");
  int currinst = inst;
  while (gameObjects[currinst].getParentReferent() != -1) {
    currinst = gameObjects[currinst].getParentReferent();
    ret = gameObjects[currinst].getName() + string(".") + gameObjects[currinst].getTypeName() + string("/") + ret;
  }
  ret = "extract/" + ret;
  voidFreadValue = system(("mkdir -p \"" + ret + "\"").c_str());

  return ret;
}

int getInstanceIndex(int type, int typeIndex) {
  unsigned int i = 0;
  int j = 0;

  for (i = 0; i < gameObjects.size(); i++) {
    if (gameObjects[i].getTypeID() == type) {
      if (typeIndex == j) {
        return i;
      }
      j++;
    }
  }
  return 0;
}

vector<float> computeAngles(float r00, float r01, float r02, float r10, float r11, float r12, float r20, float r21, float r22) {
  vector<float> retvec (3);
  retvec[0] = atan2(r21, r22);
  retvec[1] = atan2(0 - r20, sqrt(r21 * r21 + r22 * r22));
  retvec[2] = atan2(r10, r00);

  return retvec;
}

vector<float> computeRotMatrix(float x, float y, float z) {
  float mx[3][3] = {{1,1,1},
                   {1,1,1},
                   {1,1,1}};
  float my[3][3] = {{1,1,1},
                   {1,1,1},
                   {1,1,1}};
  float mz[3][3] = {{1,1,1},
                   {1,1,1},
                   {1,1,1}};

  mx[1][1] = cos(x);
  mx[1][2] = 0 - sin(x);
  mx[2][1] = sin(x);
  mx[2][2] = cos(x);

  my[0][0] = cos(y);
  my[0][2] = sin(y);
  my[2][0] = 0 - sin(y);
  my[2][2] = cos(y);

  mz[0][0] = cos(z);
  mz[0][1] = 0 - sin(z);
  mz[1][0] = sin(z);
  mz[1][1] = cos(z);

  vector<float> retvec;
  retvec.push_back(mz[0][0] * my[0][0] * mx[0][0]);
  retvec.push_back(mz[0][1] * my[0][1] * mx[0][1]);
  retvec.push_back(mz[0][2] * my[0][2] * mx[0][2]);
  retvec.push_back(mz[1][0] * my[1][0] * mx[1][0]);
  retvec.push_back(mz[1][1] * my[1][1] * mx[1][1]);
  retvec.push_back(mz[1][2] * my[1][2] * mx[1][2]);
  retvec.push_back(mz[2][0] * my[2][0] * mx[2][0]);
  retvec.push_back(mz[2][1] * my[2][1] * mx[2][1]);
  retvec.push_back(mz[2][2] * my[2][2] * mx[2][2]);

  return retvec;
}


string dataTypeToString(int dataType) {
  switch (dataType) {
    case 0x1:
      return string("String");
      break;
    case 0x2:
      return string("Boolean");
      break;
    case 0x3:
      return string("Int32");
      break;
    case 0x4:
      return string("Float");
      break;
    case 0x5:
      return string("Double");
      break;
    case 0x7:
      return string("UDim2");
      break;
    case 0x8:
      return string("Ray");
      break;
    case 0x9:
      return string("Faces");
      break;
    case 0xA:
      return string("Axis");
      break;
    case 0xB:
      return string("BrickColor");
      break;
    case 0xC:
      return string("Color3");
      break;
    case 0xD:
      return string("Vector2");
      break;
    case 0xE:
      return string("Vector3");
      break;
    case 0x10:
      return string("CFrame");
      break;
    case 0x12:
      return string("Enumeration/Token");
      break;
    case 0x13:
      return string("Referent");
      break;
    case 0x14:
      return string("CSG Data");
      break;
    default:
      return string("Unknown type");
  }
}

int getDetransformedInteger(unsigned int transformed) {
  if (transformed % 2 != 0) {
    return 0 - (transformed + 1) / 2;
  }
  else {
    return transformed / 2;
  }
}

void dumpString(char* str) {
  int len = strlen(str), i;
  printf("%X", str[0]);
  for (i = 1; i < len; i++) {
    printf(":%X", str[i]);
  }
}

void dumpStdString(string dumpthis) {
  int len = dumpthis.length(), i;
  if ((unsigned) dumpthis.at(0) < 0x10) {
    printf("0%X", (unsigned char)dumpthis.at(0));
  }
  else {
    printf("%X", (unsigned char)dumpthis.at(0));
  }
  for (i = 1; i < len; i++) {
    if ((unsigned) dumpthis.at(i) < 0x10) {
      printf(" 0%X", (unsigned char)dumpthis.at(i));
    }
    else {
      printf(" %X", (unsigned char)dumpthis.at(i));
    }
  }
}

string decompressData(FILE* decompress, int compressedLen, int decompressedLen) {
  // This function is based on the pseudocode by Gregory of Team Havemeat
  // This function expects the file position indicator to be set at a position AFTER the 0xC-byte header
  string decompressed = "";
  string compressedbuf= "";
  string substr       = "";
  int totalProcessed  = 0;
  int tokenByte       = 0;
  int literalLen      = 0;
  int matchLen        = 0;
  int lastByte        = 0;
  int offset          = 0;
  int i;
  int origLen         = 0;
  while (totalProcessed < decompressedLen) {
    //printf("%d. %d\n", strlen(decompressed), decompressedLen);
    tokenByte  = (int) fgetc(decompress);
    literalLen = tokenByte >> 4;
    matchLen   = tokenByte & 0xF;

    if (literalLen == 0xF) {
      do {
        lastByte    = (int) fgetc(decompress);
        literalLen += lastByte;
      }
      while (lastByte == 0xFF);
    }

    // Move <literalLen> bytes from compressed stream to output
    compressedbuf = string(literalLen, '\0');
    voidFreadValue = fread(&compressedbuf[0], 1, literalLen, decompress);
    decompressed += compressedbuf;
    totalProcessed += literalLen;

    if (totalProcessed < decompressedLen) {
    //if (matchLen > 0x0) {
      offset = ((int) fgetc(decompress)) + ((int) fgetc(decompress) << 8);

      if (matchLen == 0xF) {
        do {
          lastByte    = (int) fgetc(decompress);
          matchLen   += lastByte;
        }
        while (lastByte == 0xFF);
      }
      matchLen += 0x4;

      // Copy <matchLen> bytes from decompressed stream (starting at <offset> bytes from end)
      // to end of decompressed stream
      /*substr = malloc(sizeof(char) * matchLen);
      strncpy(substr, decompressed + strlen(decompressed) - offset, matchLen);
      strcat(decompressed, substr);*/
      //substr = decompressed.substr(decompressed.length() - offset, matchLen);
      origLen = decompressed.length() - offset;
      for (i = 0; i < matchLen; i++) {
        //cout << offset << "," << i << "," << decompressed.length() << endl;
        decompressed += decompressed.at(origLen + i);
      }
      //decompressed += substr;
      substr = string("");
      totalProcessed += matchLen;
    }
  }

  return decompressed;
}
/*string decompressData(FILE* decompress, int compressedLen, int decompressedLen) {
  string buf = string(decompressedLen + 5, '\0');
  blosc_init();
  blosc_set_compressor("lz4");
  string input = string(compressedLen + 5, '\0');
  fread(&input[0], 1, compressedLen, decompress);
  cout << "IN" << input << endl;
  cout << "DECOM" << blosc_decompress(input.c_str(), &buf[0], decompressedLen) << endl;
  blosc_destroy();
  return buf;
}*/

int readLittleEndian(FILE* stream) {
  return (int)(unsigned char) fgetc(stream) + ((int)(unsigned char) fgetc(stream) << 8) + ((int)(unsigned char) fgetc(stream) << 16) + ((int)(unsigned char) fgetc(stream) << 24);
}

int readLittleEndianString(string str, int offset) {
  /*char* tmp = (char*) (*string + offset);
  return (int) *tmp + ((int) *(tmp + 1) << 8) + ((int) *(tmp + 2) << 16) + ((int) *(tmp + 3) << 24);*/
  return (int)(unsigned char) str[offset + 0] + ((int)(unsigned char) str[offset + 1] << 8) + ((int)(unsigned char) str[offset + 2] << 16) + ((int)(unsigned char) str[offset + 3] << 24);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <path to .rbxl file>\n", argv[0]);
    return 1;
  }

  FILE* rbxl = fopen(argv[1], "rb");
  if (rbxl == NULL) {
    perror("fopen: unable to open file");
    return 2;
  }

  char* buf = (char*)malloc(sizeof(char) * 16);
  voidFreadValue = fread(buf, 1, 16, rbxl);
  if (strcmp(buf, "<roblox!\x89\xFF\r\n\x1A\n\0\0") != 0) {
    fprintf(stderr, "\e[31mInvalid header\e[0m\n");
    return 3;
  }
  printf("Valid header found.\n");

  int uniqueTypes = readLittleEndian(rbxl);
  printf("Unique types: %d\n", uniqueTypes);
  int totalObjects = readLittleEndian(rbxl);
  gameObjects.resize(totalObjects);
  printf("Total objects: %d\n", totalObjects);
  Instance currInstance;

  fseek(rbxl, 8, SEEK_CUR); // Skip padding

  free(buf);

  char* instancebuf = (char*)malloc(sizeof(char) * 5);

  voidFreadValue = fread(instancebuf, 1, 4, rbxl);
  instancebuf[4] = '\0';

  string decompressed, typeName;
  unsigned int i = 0, numInstances = 0, j = 0;
  int compressedLength, decompressedLength, typeID, typeNameLength, additionalData, k, shiftBy, referentIndex, lastInteger, refcounter = 0, dataType, stringLen;
  int* referents = NULL;
  string content;
  vector<string> typeNames;
  while (strcmp(instancebuf, "INST") == 0) {
    compressedLength = readLittleEndian(rbxl);
    decompressedLength = readLittleEndian(rbxl);
    fseek(rbxl, 4, SEEK_CUR); // Skip padding
    decompressed = decompressData(rbxl, compressedLength, decompressedLength);
    printf("\e[32m==== BEGIN INST DUMP ====\e[0m\n");
    typeID = readLittleEndianString(decompressed, 0);
    typeNameLength = readLittleEndianString(decompressed, 4);
    typeName = decompressed.substr(8, typeNameLength);
    typeNames.push_back(typeName);
    additionalData = (int) decompressed.at(8 + typeNameLength);
    numInstances = readLittleEndianString(decompressed, 9 + typeNameLength);
    printf("Compressed Length: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", compressedLength, compressedLength);
    printf("Decompressed Length: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", decompressedLength, decompressedLength);
    printf("Type ID: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", typeID, typeID);
    printf("Type Name Length: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", typeNameLength, typeNameLength);
    printf("Type Name: \e[36;1m%s\e[0m\n", typeName.c_str());
    printf("Additional data: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", additionalData, additionalData);
    printf("# Instances: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", numInstances, numInstances);

    referents = (int*) realloc(referents, sizeof(int) * numInstances);

    for (i = 0; i < numInstances; i++) {
      referents[i] = 0; // Damn you, realloc!
    }


    for (i = 0; i < numInstances * 4; i++) {
      //shiftBy = (numInstances - i / numInstances) * 8;
      referentIndex = i % numInstances;
      shiftBy = (3 - i / numInstances) * 8;
      //cout << shiftBy << "," << referentIndex << "," << (int)decompressed.at(13 + typeNameLength + i) << endl;
      referents[referentIndex] += (int)(unsigned char) decompressed.at(13 + typeNameLength + i) << shiftBy;
    }

    lastInteger = 0;
    for (i = 0; i < numInstances; i++) {
      lastInteger += getDetransformedInteger(referents[i]);
      refcounter++;
      printf("Referent #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m)\n", i, i, lastInteger, lastInteger);
      currInstance = new Instance();
      currInstance.setCompressedLen(compressedLength);
      currInstance.setDecompressedLen(decompressedLength);
      currInstance.setTypeID(typeID);
      currInstance.setTypeNameLength(typeNameLength);
      currInstance.setAdditionalData(additionalData);
      currInstance.setReferent(lastInteger);
      currInstance.setNumReferents(numInstances);
      currInstance.setRawData(decompressed);
      currInstance.setTypeName(typeName);
      currInstance.setAdditionalDataContent("");

      gameObjects[lastInteger] = currInstance;
    }

    printf("Raw data: \e[34;1m");
    dumpStdString(decompressed);
    printf("\e[0m\n");

    voidFreadValue = fread(instancebuf, 1, 4, rbxl);
  }

  int* ints32 = NULL;
  int* floats = NULL;
  float currFloat = 0.0;
  unsigned int currFloatTemp = 0, isSigned;
  char currDoubleTemp[sizeof(double)];

  int* offsetsX = NULL;
  int* offsetsY = NULL;
  int* scalesX  = NULL;
  int* scalesY  = NULL;
  float currScaleX = 0.0;
  float currScaleY = 0.0;
  int currOffsetX  = 0;
  int currOffsetY  = 0;

  float origin[3];
  float direction[3];

  int *r = NULL, *g = NULL, *b = NULL;

  int *vecx = NULL, *vecy = NULL, *vecz = NULL;
  float *r00 = NULL, *r01 = NULL, *r02 = NULL, *r10 = NULL, *r11 = NULL, *r12 = NULL, *r20 = NULL, *r21 = NULL, *r22 = NULL;
  int stringpos;
  vector<float> matrix(9);
  vector<float> angles(3);

  Property currProperty;
  Axis currAxis;
  BrickColor currBrickColor;
  CFrame currCFrame;
  Color3 currColor3;
  Faces currFaces;
  Ray currRay;
  UDim2 currUDim2;
  Vector2 currVector2;
  Vector3 currVector3;

  char currChar;

  while (strcmp(instancebuf, "PROP") == 0) {
    compressedLength = readLittleEndian(rbxl);
    decompressedLength = readLittleEndian(rbxl);
    fseek(rbxl, 4, SEEK_CUR); // Skip padding
    decompressed = decompressData(rbxl, compressedLength, decompressedLength);
    printf("\e[32m==== BEGIN PROP DUMP ====\e[0m\n");
    typeID = readLittleEndianString(decompressed, 0);
    typeNameLength = readLittleEndianString(decompressed, 4);
    typeName = decompressed.substr(8, typeNameLength);
    dataType = (int) decompressed.at(8 + typeNameLength);

    printf("Compressed Length: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", compressedLength, compressedLength);
    printf("Decompressed Length: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", decompressedLength, decompressedLength);
    printf("Type ID: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m - \e[36;1m%s\e[0m)\n", typeID, typeID, typeNames[typeID].c_str());
    printf("Type Name Length: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m)\n", typeNameLength, typeNameLength);
    printf("Type Name: \e[36;1m%s\e[0m\n", typeName.c_str());
    printf("Data type: \e[36;1m%d\e[0m (\e[36;1m0x%X\e[0m - \e[36;1m%s\e[0m)\n", dataType, dataType, dataTypeToString(dataType).c_str());
    printf("Raw data: \e[34;1m");
    dumpStdString(decompressed);
    printf("\e[0m\n");

    if (dataType == 0x1) {
      i = 9 + typeNameLength;
      j = 0;
      while (decompressed.substr(i, 4).length()) {
        stringLen = readLittleEndianString(decompressed, i);
        i += 4;
        printf("String entry %d (%X): \e[36;1m%s\e[0m\n", j, j, decompressed.substr(i, stringLen).c_str());
        i += stringLen;
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currProperty.setStringValue(decompressed.substr(i - stringLen, stringLen));
        gameObjects[getInstanceIndex(typeID, j)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, j)].addPropertyName(typeName);
        cout << getInstanceIndex(typeID, j) << endl;
        j++;
      }
    }
    else if (dataType == 0x2) {
      j = 0;
      for (i = 9 + typeNameLength; i < decompressed.length(); i++) {
        if ((int)decompressed[i] == 0x1) {
          printf("Boolean entry %d (%X): \e[36;1mtrue\e[0m\n", j, j);
        }
        else {
          printf("Boolean entry %d (0x%X): \e[36;1mfalse\e[0m\n", j, j);
        }
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currProperty.setBoolValue(((int)decompressed[i] == 0x1) ? true : false);
        gameObjects[getInstanceIndex(typeID, j)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, j)].addPropertyName(typeName);
        cout << getInstanceIndex(typeID, j) << endl;
        j++;
      }
    }
    else if (dataType == 0x3 || dataType == 0x12) {
      numInstances = (decompressed.length() - typeNameLength - 9) / 4;
      ints32 = (int*) realloc(ints32, sizeof(int) * numInstances);
      for (i = 0; i < numInstances; i++) {
        ints32[i] = 0;
      }

      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        ints32[referentIndex] += ((int)(unsigned char) decompressed.at(9 + typeNameLength + i)) << shiftBy;
      }

      for (i = 0; i < numInstances; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        if (dataType == 0x3) {
          currProperty.setIntValue(getDetransformedInteger(ints32[i]));
          printf("Int32");
        }
        else {
          currProperty.setEnumValue(getDetransformedInteger(ints32[i]));
          printf("Enum/Token");
        }
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
        printf(" Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m)\n", i, i, getDetransformedInteger(ints32[i]), getDetransformedInteger(ints32[i]));
      }
    }
    else if (dataType == 0x4) {
      numInstances = (decompressed.length() - typeNameLength - 9) / 4;
      floats = (int*) realloc(floats, sizeof(int) * numInstances);
      for (i = 0; i < numInstances; i++) {
        floats[i] = 0;
      }

      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        floats[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i) << shiftBy;
      }

      for (i = 0; i < numInstances; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        isSigned = floats[i] << 31;
        currFloatTemp = floats[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        memcpy(&currFloat, &currFloatTemp, sizeof(currFloat));
        currProperty.setFloatValue(currFloat);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);

        printf("Float Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): \e[33;1m%f\e[0m (\e[33;1m0x%X\e[0m)\n", i, i, currFloat, currFloatTemp);
      }
    }
    else if (dataType == 0x5) {
      numInstances = (decompressed.length() - typeNameLength - 9) / 8;
      for (i = 0; i < numInstances; i++) {
        k = 0;
        for (j = typeNameLength + 9 + i; j < typeNameLength + 17 + i; j++) {
          currDoubleTemp[k] = decompressed[j];
          k++;
        }
        k = 0;
        for (j = sizeof(currDoubleTemp) / sizeof(currDoubleTemp[0]); j > 0; j--) {
          u.bytes[j] = currDoubleTemp[j]; // Quick hax because I noticed that it's little-endian
          k++;
        }
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currProperty.setDoubleValue(u.d);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);

        printf("Double Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): \e[33;1m%g\e[0m (Little Endian: \e[33;1m0x%X%X%X%X%X%X%X%X\e[0m)\n", i, i, u.d, u.bytes[0], u.bytes[1], u.bytes[2], u.bytes[3], u.bytes[4], u.bytes[5], u.bytes[6], u.bytes[7]);
      }
    }
    else if (dataType == 0x7) {
      numInstances = (decompressed.length() - typeNameLength - 9) / 16;
      offsetsX = (int*)  realloc(offsetsX, sizeof(int) * numInstances);
      offsetsY = (int*)  realloc(offsetsY, sizeof(int) * numInstances);
      scalesX  = (int*)  realloc(scalesX,  sizeof(int) * numInstances);
      scalesY  = (int*)  realloc(scalesY,  sizeof(int) * numInstances);
      for (i = 0; i < numInstances; i++) {
        offsetsX[i] = 0;
        offsetsY[i] = 0;
        scalesX[i]  = 0;
        scalesY[i]  = 0;
      }

      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        scalesX[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i) << shiftBy;
        scalesY[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 4) << shiftBy;
        offsetsX[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 8) << shiftBy;
        offsetsY[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 12) << shiftBy;
        //printf("ADDING TO OFFSETX: %X,%X\n", (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 8), (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 12));
      }

      for (i = 0; i < numInstances; i++) {
        isSigned = scalesX[i] << 31;
        currFloatTemp = scalesX[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        currScaleX = 0.0;
        currScaleY = 0.0;
        currOffsetX = 0;
        currOffsetY = 0;
        memcpy(&currScaleX, &currFloatTemp, sizeof(currScaleX));

        //printf("UDim2 ScaleX Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m)\n", i, i, currFloat, currFloatTemp);

        isSigned = scalesY[i] << 31;
        currFloatTemp = scalesY[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        memcpy(&currScaleY, &currFloatTemp, sizeof(currScaleY));

        //printf("UDim2 ScaleY Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m)\n", i, i, currFloat, currFloatTemp);
        currOffsetX = getDetransformedInteger(offsetsX[i]);
        //cout << "Transformed " << offsetsX[i] << " to " << currOffsetX;
        currOffsetY = getDetransformedInteger(offsetsY[i]);
        //cout << "Transformed " << offsetsY[i] << " to " << currOffsetY;
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);

        currUDim2 = new UDim2();
        printf("UDim2 Entry #\e[33;1m%d\e[0m (\e[33:1m0x%X\e[0m): ", i, i);
        f.f = currScaleX;
        currUDim2.setScaleX(currScaleX);
        printf("{ScaleX: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), ", currScaleX, f.hexval);
        currUDim2.setOffsetX(currOffsetX);
        printf("OffsetX: \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m), ", currOffsetX, currOffsetX);
        f.f = currScaleY;
        currUDim2.setScaleY(currScaleY);
        printf("ScaleY: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), ", currScaleY, f.hexval);
        currUDim2.setOffsetY(currOffsetY);
        printf("OffsetY: \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m)}\n", currOffsetY, currOffsetY);
        //printf("UDim2 Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): {ScaleX: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), OffsetX: \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m), ScaleY: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), OffsetY: \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m)}\n", i, i, currScaleX, currScaleX, currOffsetX, currOffsetX, currScaleY, currScaleY, currOffsetY, currOffsetY);
        currProperty.setUDim2Value(currUDim2);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
      }
    }
    else if (dataType == 0x8) {
      numInstances = (decompressed.length() - typeNameLength - 9) / 0x18;

      for (i = 0; i < decompressed.length() - typeNameLength - 9; i += 0x18) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currRay = new Ray();
        currFloatTemp  = ((int)(unsigned char) decompressed.at(i + typeNameLength + 9))  << 0;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 10)) << 8;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 11)) << 16;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 12)) << 24;
        f.hexval = currFloatTemp;
        currRay.setOriginX(f.f);
        printf("Ray Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): {OriginX: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), ", i, i, f.f, f.hexval);
        memcpy(&(origin[0]), &currFloatTemp, sizeof(float));

        currFloatTemp  = ((int)(unsigned char) decompressed.at(i + typeNameLength + 13)) << 0;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 14)) << 8;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 15)) << 16;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 16)) << 24;
        f.hexval = currFloatTemp;
        currRay.setOriginY(f.f);
        printf("OriginY: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), ", f.f, f.hexval);
        memcpy(&(origin[1]), &currFloatTemp, sizeof(float));

        currFloatTemp  = ((int)(unsigned char) decompressed.at(i + typeNameLength + 17)) << 0;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 18)) << 8;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 19)) << 16;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 20)) << 24;
        f.hexval = currFloatTemp;
        currRay.setOriginZ(f.f);
        printf("OriginZ: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), ", f.f, f.hexval);
        memcpy(&(origin[2]), &currFloatTemp, sizeof(float));

        currFloatTemp  = ((int)(unsigned char) decompressed.at(i + typeNameLength + 21)) << 0;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 22)) << 8;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 23)) << 16;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 24)) << 24;
        f.hexval = currFloatTemp;
        currRay.setDirectionX(f.f);
        printf("DirectionX: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), ", f.f, f.hexval);
        memcpy(&(direction[0]), &currFloatTemp, sizeof(float));

        currFloatTemp  = ((int)(unsigned char) decompressed.at(i + typeNameLength + 25)) << 0;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 26)) << 8;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 27)) << 16;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 28)) << 24;
        f.hexval = currFloatTemp;
        currRay.setDirectionY(f.f);
        printf("DirectionY: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), ", f.f, f.hexval);
        memcpy(&(direction[1]), &currFloatTemp, sizeof(float));

        currFloatTemp  = ((int)(unsigned char) decompressed.at(i + typeNameLength + 29)) << 0;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 30)) << 8;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 31)) << 16;
        currFloatTemp += ((int)(unsigned char) decompressed.at(i + typeNameLength + 32)) << 24;
        f.hexval = currFloatTemp;
        currRay.setDirectionZ(f.f);
        printf("DirectionZ: \e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m)}\n", f.f, f.hexval);
        memcpy(&(direction[2]), &currFloatTemp, sizeof(float));

        currProperty.setRayValue(currRay);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
      }
    }
    else if (dataType == 0x9) {
      numInstances = decompressed.length() - typeNameLength - 9;

      for (i = 0; i < decompressed.length() - typeNameLength - 9; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currFaces = new Faces();
        printf("Faces Entry #\e[33;1m%d\e[0m (\e[33;1m%X\e[0m): {", i, i);

        currChar = decompressed.at(typeNameLength + 9 + i);

        if (currChar & 0x20) {
          printf("\e[36;1mRight\e[0m");
          currFaces.setRight(true);
          if (currChar & 0x1F) {
            printf(", ");
          }
        }
        else {
          currFaces.setRight(false);
        }
        if (currChar & 0x10) {
          printf("\e[36;1mTop\e[0m");
          currFaces.setTop(true);
          if (currChar & 0xF) {
            printf(", ");
          }
        }
        else {
          currFaces.setTop(false);
        }
        if (currChar & 0x8) {
          printf("\e[36;1mBack\e[0m");
          currFaces.setBack(true);
          if (currChar & 0x7) {
            printf(", ");
          }
        }
        else {
          currFaces.setBack(false);
        }
        if (currChar & 0x4) {
          printf("\e[36;1mLeft\e[0m");
          currFaces.setLeft(true);
          if (currChar & 0x3) {
            printf(", ");
          }
        }
        else {
          currFaces.setLeft(false);
        }
        if (currChar & 0x2) {
          printf("\e[36;1mBottom\e[0m");
          currFaces.setBottom(true);
          if (currChar & 0x1) {
            printf(", ");
          }
        }
        else {
          currFaces.setBottom(false);
        }
        if (currChar & 0x1) {
          printf("\e[36;1mFront\e[0m");
          currFaces.setFront(true);
        }
        else {
          currFaces.setFront(false);
        }

        printf("}\n");
        currProperty.setFacesValue(currFaces);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
      }
    }
    else if (dataType == 0xA) {
      numInstances = decompressed.length() - 9 - typeNameLength;
      for (i = 0; i < decompressed.length() - 9 - typeNameLength; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currAxis = new Axis();
        currChar = decompressed.at(typeNameLength + 9 + i);
        printf("Axis Entry #\e[33;1m%d\e[0m (\e[33;1m%X\e[0m): {", i, i);
        if (currChar & 0x4) {
          printf("\e[36;1mZ\e[0m");
          currAxis.setZ(true);
          if (currChar & 0x3) {
            printf(", ");
          }
        }
        else {
          currAxis.setZ(false);
        }
        if (currChar & 0x2) {
          printf("\e[36;1mY\e[0m");
          currAxis.setY(true);
          if (currChar & 0x1) {
            printf(", ");
          }
        }
        else {
          currAxis.setY(false);
        }
        if (currChar & 0x1) {
          currAxis.setX(true);
          printf("\e[36;1mX\e[0m");
        }
        else {
          currAxis.setX(false);
        }
        printf("}\n");
        currProperty.setAxisValue(currAxis);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
      }
    }
    else if (dataType == 0xB) {
      numInstances = (decompressed.length() - 9 - typeNameLength) / 4;
      ints32 = (int*) realloc(ints32, sizeof(int) * numInstances);
      for (i = 0; i < numInstances; i++) {
        ints32[i] = 0;
      }

      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        ints32[referentIndex] += ((int)(unsigned char) decompressed.at(9 + typeNameLength + i)) << shiftBy;
      }

      for (i = 0; i < numInstances; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currBrickColor = new BrickColor();
        currBrickColor.setColor(ints32[i]);
        currProperty.setBrickColorValue(currBrickColor);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
        printf("BrickColor Entry #\e[36;1m%d\e[0m (\e[36;1m%X\e[0m): \e[36;1m%d\e[0m (\e[36;1m%X\e[0m)\n", i, i, ints32[i], ints32[i]);
      }
    }
    else if (dataType == 0xC) {
      numInstances = (decompressed.length() - 9 - typeNameLength) / 12;
      r = (int*) realloc(r, sizeof(int) * numInstances);
      g = (int*) realloc(g, sizeof(int) * numInstances);
      b = (int*) realloc(b, sizeof(int) * numInstances);
      for (i = 0; i < numInstances; i++) {
        r[i] = 0;
        g[i] = 0;
        b[i] = 0;
      }
      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        r[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i) << shiftBy;
        g[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 4) << shiftBy;
        b[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 8) << shiftBy;
      }
      for (i = 0; i < numInstances; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currColor3 = new Color3();
        printf("Color3 Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): {R: ", i, i);
        isSigned = r[i] << 31;
        currFloatTemp = r[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        f.hexval = currFloatTemp;
        currColor3.setR(f.f);
        printf("\e[33;1m%d\e[0m (Stored value: \e[33;1m%.3f\e[0m, Stored hex value: \e[33;1m%X\e[0m, Actual hex value: \e[33;1m%X\e[0m), G:", (int) (f.f * 255), f.f, f.hexval, (int) (f.f * 255));
        isSigned = g[i] << 31;
        currFloatTemp = g[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        f.hexval = currFloatTemp;
        currColor3.setG(f.f);
        printf("\e[33;1m%d\e[0m (Stored value: \e[33;1m%.3f\e[0m, Stored hex value: \e[33;1m%X\e[0m, Actual hex value: \e[33;1m%X\e[0m), B:", (int) (f.f * 255), f.f, f.hexval, (int) (f.f * 255));
        isSigned = b[i] << 31;
        currFloatTemp = b[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        f.hexval = currFloatTemp;
        currColor3.setB(f.f);
        printf("\e[33;1m%d\e[0m (Stored value: \e[33;1m%.3f\e[0m, Stored hex value: \e[33;1m%X\e[0m, Actual hex value: \e[33;1m%X\e[0m)}\n", (int) (f.f * 255), f.f, f.hexval, (int) (f.f * 255));
        currProperty.setColor3Value(currColor3);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
      }
    }
    else if (dataType == 0xD) {
      numInstances = (decompressed.length() - 9 - typeNameLength) / 8;
      vecx = (int*)realloc(vecx, numInstances * sizeof(int));
      vecy = (int*)realloc(vecy, numInstances * sizeof(int));

      for (i = 0; i < numInstances; i++) {
        vecx[i] = 0;
        vecy[i] = 0;
      }
      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        cout << shiftBy << "," << i << "," << numInstances << endl;
        vecx[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i) << shiftBy;
        vecy[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 4) << shiftBy;
      }
      for (i = 0; i < numInstances; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currVector2 = new Vector2();
        printf("Vector2 Entry #\e[33;1m%d\e[0m (\e[33;1m%X\e[0m): {X: ", i, i);
        isSigned = vecx[i] << 31;
        currFloatTemp = vecx[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        f.hexval = currFloatTemp;
        currVector2.setX(f.f);
        printf("\e[33;1m%.3f\e[0m (\e[33;1m%X\e[0m), Y: ", f.f, f.hexval);
        isSigned = vecy[i] << 31;
        currFloatTemp = vecy[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        f.hexval = currFloatTemp;
        currVector2.setY(f.f);
        printf("\e[33;1m%.3f\e[0m (\e[33;1m%X\e[0m)}\n", f.f, f.hexval);
        currProperty.setVector2Value(currVector2);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
      }
    }
    else if (dataType == 0xE) {
      numInstances = (decompressed.length() - 9 - typeNameLength) / 12;
      vecx = (int*)realloc(vecx, numInstances * sizeof(int));
      vecy = (int*)realloc(vecy, numInstances * sizeof(int));
      vecz = (int*)realloc(vecz, numInstances * sizeof(int));

      for (i = 0; i < numInstances; i++) {
        vecx[i] = 0;
        vecy[i] = 0;
        vecz[i] = 0;
      }
      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        //cout << shiftBy << "," << i << "," << numInstances << endl;
        vecx[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i) << shiftBy;
        vecy[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 4) << shiftBy;
        vecz[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i + numInstances * 8) << shiftBy;
      }
      for (i = 0; i < numInstances; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currVector3 = new Vector3();
        printf("Vector3 Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): {X: ", i, i);
        isSigned = vecx[i] << 31;
        currFloatTemp = vecx[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        f.hexval = currFloatTemp;
        currVector3.setX(f.f);
        printf("\e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), Y: ", f.f, f.hexval);
        isSigned = vecy[i] << 31;
        currFloatTemp = vecy[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        f.hexval = currFloatTemp;
        currVector3.setY(f.f);
        printf("\e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), Z:", f.f, f.hexval);
        isSigned = vecz[i] << 31;
        currFloatTemp = vecz[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x10000000;
        }
        f.hexval = currFloatTemp;
        currVector3.setZ(f.f);
        printf("\e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m)}\n", f.f, f.hexval);
        currProperty.setVector3Value(currVector3);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
      }
    }
    else if (dataType == 0x10) { // CFrame time! Stuff's gonna get interesting here.
      stringpos = 9 + typeNameLength;
      numInstances = 0;

      // Count number of instances
      while (numInstances != (decompressed.length() - stringpos) / 12) {
        if (decompressed.at(stringpos) == '\0') {
          stringpos += 0x24;
          numInstances++;
        }
        else {
          stringpos++;
          numInstances++;
        }
      }

      vecx = (int*)realloc(vecx, numInstances * sizeof(int));
      vecy = (int*)realloc(vecy, numInstances * sizeof(int));
      vecz = (int*)realloc(vecz, numInstances * sizeof(int));
      r00  = (float*)realloc(r00,  numInstances * sizeof(float));
      r01  = (float*)realloc(r01,  numInstances * sizeof(float));
      r02  = (float*)realloc(r02,  numInstances * sizeof(float));
      r10  = (float*)realloc(r10,  numInstances * sizeof(float));
      r11  = (float*)realloc(r11,  numInstances * sizeof(float));
      r12  = (float*)realloc(r12,  numInstances * sizeof(float));
      r20  = (float*)realloc(r20,  numInstances * sizeof(float));
      r21  = (float*)realloc(r21,  numInstances * sizeof(float));
      r22  = (float*)realloc(r22,  numInstances * sizeof(float));

      for (i = 0; i < numInstances; i++) {
        vecx[i] = 0;
        vecy[i] = 0;
        vecz[i] = 0;
        r00[i]  = 0;
        r01[i]  = 0;
        r02[i]  = 0;
        r10[i]  = 0;
        r11[i]  = 0;
        r12[i]  = 0;
        r20[i]  = 0;
        r21[i]  = 0;
        r22[i]  = 0;
      }

      stringpos = 9 + typeNameLength;
      // Parse the rotation matrix
      i = 0;
      while (i != (decompressed.length() - stringpos) / 12) {
        // Long values
        if (decompressed.at(stringpos) == '\0') {
          for (j = 0; j < 9; j++) {
            currFloatTemp = (int)(unsigned char) decompressed.at(++stringpos);
            currFloatTemp += ((int)(unsigned char) decompressed.at(++stringpos)) << 8;
            currFloatTemp += ((int)(unsigned char) decompressed.at(++stringpos)) << 16;
            currFloatTemp += ((int)(unsigned char) decompressed.at(++stringpos)) << 24;
            f.hexval = currFloatTemp;
            matrix[j] = f.f;
          }
        }
        else { // Short values
          switch (decompressed.at(stringpos)) {
            case '\x02': // 0,0,0
              matrix = computeRotMatrix(0,0,0);
              break;
            case '\x03': // 90,0,0
              matrix = computeRotMatrix(90,0,0);
              break;
            case '\x05': // -180,0,0
              matrix = computeRotMatrix(-180,0,0);
              break;
            case '\x06': // -90,0,0
              matrix = computeRotMatrix(-90,0,0);
              break;
            case '\x07': // -180,0,-90
              matrix = computeRotMatrix(-180,0,-90);
              break;
            case '\x09': // 90,90,0
              matrix = computeRotMatrix(90,90,0);
              break;
            case '\x0A': // 0,0,90
              matrix = computeRotMatrix(0,0,90);
              break;
            case '\x0C': // -90,-90,0
              matrix = computeRotMatrix(-90,-90,0);
              break;
            case '\x0D': // -90,0,-90
              matrix = computeRotMatrix(-90,0,-90);
              break;
            case '\x0E': // 0,-90,0
              matrix = computeRotMatrix(0,-90,0);
              break;
            case '\x10': // 90,0,90
              matrix = computeRotMatrix(90,0,90);
              break;
            case '\x11': // 180,90,0
              matrix = computeRotMatrix(180,90,0);
              break;
            case '\x14': // -180,0,-180
              matrix = computeRotMatrix(-180,0,-180);
              break;
            case '\x15': // -90,0,-180
              matrix = computeRotMatrix(-90,0,-180);
              break;
            case '\x17': // 0,0,-180
              matrix = computeRotMatrix(0,0,-180);
              break;
            case '\x18': // 90,0,-180
              matrix = computeRotMatrix(90,0,-180);
              break;
            case '\x19': // 0,0,-90
              matrix = computeRotMatrix(0,0,-90);
              break;
            case '\x1B': // 90,-90,0
              matrix = computeRotMatrix(90,-90,0);
              break;
            case '\x1C': // 180,0,90
              matrix = computeRotMatrix(180,0,90);
              break;
            case '\x1E': // -90,90,0
              matrix = computeRotMatrix(-90,90,0);
              break;
            case '\x1F': // 90,0,-90
              matrix = computeRotMatrix(90,0,-90);
              break;
            case '\x20': // 0,90,0
              matrix = computeRotMatrix(0,90,0);
              break;
            case '\x22': // -90,0,90
              matrix = computeRotMatrix(-90,0,90);
              break;
            case '\x23': // -180,-90,0
              matrix = computeRotMatrix(-180,-90,0);
              break;
          }
        }
        r00[i] = matrix[0];
        r01[i] = matrix[1];
        r02[i] = matrix[2];
        r10[i] = matrix[3];
        r11[i] = matrix[4];
        r12[i] = matrix[5];
        r20[i] = matrix[6];
        r21[i] = matrix[7];
        r22[i] = matrix[8];
        i++;
      }

      stringpos++;

      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        //cout << shiftBy << "," << i << "," << numInstances << "," << stringpos << "," << (int)decompressed.at(stringpos + i + numInstances * 4) << endl;
        vecx[referentIndex] += (int)(unsigned char) decompressed.at(stringpos + i) << shiftBy;
        vecy[referentIndex] += (int)(unsigned char) decompressed.at(stringpos + i + numInstances * 4) << shiftBy;
        vecz[referentIndex] += (int)(unsigned char) decompressed.at(stringpos + i + numInstances * 8) << shiftBy;
      }

      for (i = 0; i < numInstances; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        currCFrame = new CFrame();
        printf("CFrame Entry #\e[33;1m%d\e[0m (\e[33;1m%X\e[0m): {X: ", i, i);
        isSigned = vecx[i] << 31;
        currFloatTemp = vecx[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          //currFloatTemp &= 0x7FFFFFFF;
          currFloatTemp |= 0x80000000;
        }
        f.hexval = currFloatTemp;
        currCFrame.setX(f.f);
        printf("\e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), Y: ", f.f, f.hexval);
        isSigned = vecy[i] << 31;
        currFloatTemp = vecy[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x80000000;
        }
        f.hexval = currFloatTemp;
        currCFrame.setY(f.f);
        printf("\e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), Z: ", f.f, f.hexval);
        isSigned = vecz[i] << 31;
        currFloatTemp = vecz[i] >> 1;
        if (!isSigned) {
          currFloatTemp &= 0x7FFFFFFF;
        }
        else {
          currFloatTemp |= 0x80000000;
        }
        f.hexval = currFloatTemp;
        currCFrame.setZ(f.f);
        printf("\e[33;1m%.3f\e[0m (\e[33;1m0x%X\e[0m), Rotation Matrix: ", f.f, f.hexval);
        currCFrame.setR00(r00[i]);
        currCFrame.setR01(r01[i]);
        currCFrame.setR02(r02[i]);
        currCFrame.setR10(r10[i]);
        currCFrame.setR11(r11[i]);
        currCFrame.setR12(r12[i]);
        currCFrame.setR20(r20[i]);
        currCFrame.setR21(r21[i]);
        currCFrame.setR22(r22[i]);
        printf("\e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, Euler Angles (approx.): ", r00[i], r01[i], r02[i], r10[i], r11[i], r12[i], r20[i], r21[i], r22[i]);
        angles = computeAngles(r00[i], r01[i], r02[i], r10[i], r11[i], r12[i], r20[i], r21[i], r22[i]);
        printf("\e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m, \e[33;1m%.3f\e[0m}\n", angles[0], angles[1], angles[2]);
        currProperty.setCFrameValue(currCFrame);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
      }
    }
    else if (dataType == 0x12) { // See data type 0x3

    }
    else if (dataType == 0x13) { // Referents
      numInstances = (decompressed.length() - 9 - typeNameLength) / 4;
      referents = (int*) realloc(referents, sizeof(int) * numInstances);

      for (i = 0; i < numInstances; i++) {
        referents[i] = 0;
      }

      for (i = 0; i < numInstances * 4; i++) {
        referentIndex = i % numInstances;
        shiftBy = (3 - i / numInstances) * 8;
        referents[referentIndex] += (int)(unsigned char) decompressed.at(9 + typeNameLength + i) << shiftBy;
      }

      lastInteger = 0;
      for (i = 0; i < numInstances; i++) {
        currProperty = new Property();
        currProperty.setCompressedLength(compressedLength);
        currProperty.setDecompressedLength(decompressedLength);
        currProperty.setRawData(decompressed);
        currProperty.setTypeID(typeID);
        currProperty.setPropertyNameLength(typeNameLength);
        currProperty.setPropertyName(typeName);
        currProperty.setDataType(dataType);
        lastInteger += getDetransformedInteger(referents[i]);
        refcounter++;
        currProperty.setReferentValue(lastInteger);
        gameObjects[getInstanceIndex(typeID, i)].addProperty(currProperty);
        gameObjects[getInstanceIndex(typeID, i)].addPropertyName(typeName);
        printf("Referent Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m)\n", i, i, lastInteger, lastInteger);
      }
    }

    voidFreadValue = fread(instancebuf, 1, 4, rbxl);
  }
  int lastParentInteger;
  unsigned int numObjectsPRNT = 0;
  int *objects = NULL, *parents = NULL;
  string parentName;

  if (strcmp(instancebuf, "PRNT")) {
    fprintf(stderr, "\e[31mInvalid parent header\e[0m\n");
    return 4;
  }
  else {
    compressedLength = readLittleEndian(rbxl);
    decompressedLength = readLittleEndian(rbxl);
    fseek(rbxl, 4, SEEK_CUR); // Skip padding
    decompressed = decompressData(rbxl, compressedLength, decompressedLength);
    printf("\e[32m==== BEGIN PRNT DUMP ====\e[0m\n");
    stringpos = 0;
    numObjectsPRNT = (int)(unsigned char)decompressed.at(++stringpos);
    numObjectsPRNT += (int)(unsigned char)decompressed.at(++stringpos) << 8;
    numObjectsPRNT += (int)(unsigned char)decompressed.at(++stringpos) << 16;
    numObjectsPRNT += (int)(unsigned char)decompressed.at(++stringpos) << 24;
    printf("Number of objects: \e[31m%d\e[0m (\e[31m%X\e[0m)\n", numObjectsPRNT, numObjectsPRNT);
    printf("Raw parent data: \e[31m");
    dumpStdString(decompressed);
    printf("\e[0m\n");
    objects = (int*) realloc(objects, sizeof(int) * numObjectsPRNT);
    parents = (int*) realloc(parents, sizeof(int) * numObjectsPRNT);

    for (i = 0; i < numObjectsPRNT; i++) {
      objects[i] = 0; // Damn you, realloc!
      parents[i] = 0;
    }


    for (i = 0; i < numObjectsPRNT * 4; i++) {
      referentIndex = i % numObjectsPRNT;
      shiftBy = (3 - i / numObjectsPRNT) * 8;
      objects[referentIndex] += (int)(unsigned char) decompressed.at(5 + i) << shiftBy;
      parents[referentIndex] += (int)(unsigned char) decompressed.at(5 + i + numObjectsPRNT * 4) << shiftBy;
    }

    lastInteger = 0;
    lastParentInteger = 0;
    for (i = 0; i < numObjectsPRNT; i++) {
      lastInteger += getDetransformedInteger(objects[i]);
      lastParentInteger += getDetransformedInteger(parents[i]);
      if (lastParentInteger < 0) {
        parentName = "DataModel/nil";
      }
      else {
        parentName = gameObjects[lastParentInteger].getTypeName();
      }
      gameObjects[lastInteger].setParentReferent(lastParentInteger);
      printf("PRNT Entry #\e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m): \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m - %s) is a child of \e[33;1m%d\e[0m (\e[33;1m0x%X\e[0m - %s)\n", i, i, lastInteger, lastInteger, gameObjects[lastInteger].getTypeName().c_str(), lastParentInteger, lastParentInteger, parentName.c_str());
    }
  }
    cout << "Please wait, dumping instances..." << endl;

  for (i = 0; i < numObjectsPRNT; i++) {
    //cout << "try to open" << (getExtractedPath(i) + "properties.rbx_props") << endl;
	currObjectStream.open((getExtractedPath(i) + "properties.rbx_props").c_str());
    //cout << "opening stream" << (getExtractedPath(i) + "properties.rbx_props") << endl << flush;
	currObjectStream << "// RBXL Analysis Object Property Extraction" << endl;
	currObjectStream << "// This file is not meant to be edited manually" << endl;
	for (j = 0; j < (unsigned int) gameObjects[i].getPropertyCount(); j++) {
		switch (gameObjects[i].getProperty(j).getDataType()) {
			case 0x1:
                if (gameObjects[i].getProperty(j).getPropertyName() == "Source") {
                    dumpSource(getExtractedPath(i), gameObjects[i].getProperty(j).getStringValue());
                }
                else {
				    currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (String Entry, " << countLines(gameObjects[i].getProperty(j).getStringValue()) + 1 << " Lines): " << gameObjects[i].getProperty(j).getStringValue() << endl;
                }
                //cout << gameObjects[i].getProperty(j).getPropertyName() << " (String Entry, " << countLines(gameObjects[i].getProperty(j).getStringValue()) << " Lines):" << gameObjects[i].getProperty(j).getStringValue() << endl;
				break;
            case 0x2:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Bool Entry): " << (gameObjects[i].getProperty(j).getBoolValue() ? "true" : "false") << endl;
                break;
            case 0x3:
            case 0x12:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Int32/Enum/Token Entry): " << gameObjects[i].getProperty(j).getIntValue() << " (" << hexVal(gameObjects[i].getProperty(j).getIntValue()) << ")" << endl;
                break;
            case 0x4:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Float Entry): " << gameObjects[i].getProperty(j).getFloatValue() << " (" << hexValFloat(gameObjects[i].getProperty(j).getFloatValue()) << ")" << endl;
                break;
            case 0x5:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Double Entry): " << gameObjects[i].getProperty(j).getDoubleValue() << " (" << hexValDouble(gameObjects[i].getProperty(j).getDoubleValue()) << ")" << endl;
                break;
            case 0x7:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (UDim2 Entry): SX: ";
                currObjectStream << gameObjects[i].getProperty(j).getUDim2Value().getScaleX() << " (" << hexValFloat(gameObjects[i].getProperty(j).getUDim2Value().getScaleX()) << "), OX: ";
                currObjectStream << gameObjects[i].getProperty(j).getUDim2Value().getOffsetX() << " (" << hexVal(gameObjects[i].getProperty(j).getUDim2Value().getOffsetX()) << "), SY: ";
                currObjectStream << gameObjects[i].getProperty(j).getUDim2Value().getScaleY() << " (" << hexValFloat(gameObjects[i].getProperty(j).getUDim2Value().getScaleY()) << "), OY: ";
                currObjectStream << gameObjects[i].getProperty(j).getUDim2Value().getOffsetY() << " (" << hexVal(gameObjects[i].getProperty(j).getUDim2Value().getOffsetY()) << ")" << endl;
                break;
            case 0x8:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Ray Entry): OX: ";
                currObjectStream << gameObjects[i].getProperty(j).getRayValue().getOriginX() << " (" << hexValFloat(gameObjects[i].getProperty(j).getRayValue().getOriginX()) << "), OY: ";
                currObjectStream << gameObjects[i].getProperty(j).getRayValue().getOriginY() << " (" << hexValFloat(gameObjects[i].getProperty(j).getRayValue().getOriginY()) << "), OZ: ";
                currObjectStream << gameObjects[i].getProperty(j).getRayValue().getOriginZ() << " (" << hexValFloat(gameObjects[i].getProperty(j).getRayValue().getOriginZ()) << "), DX: ";
                currObjectStream << gameObjects[i].getProperty(j).getRayValue().getDirectionX() << " (" << hexValFloat(gameObjects[i].getProperty(j).getRayValue().getDirectionX()) << "), DY: ";
                currObjectStream << gameObjects[i].getProperty(j).getRayValue().getDirectionY() << " (" << hexValFloat(gameObjects[i].getProperty(j).getRayValue().getDirectionY()) << "), DZ: ";
                currObjectStream << gameObjects[i].getProperty(j).getRayValue().getDirectionZ() << " (" << hexValFloat(gameObjects[i].getProperty(j).getRayValue().getDirectionZ()) << ")" << endl;
                break;
            case 0x9:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Faces Entry): ";
                currObjectStream << dumpFaces(gameObjects[i].getProperty(j).getFacesValue()) << endl;
                break;
            case 0xA:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Axis Entry): ";
                currObjectStream << dumpAxis(gameObjects[i].getProperty(j).getAxisValue()) << endl;
                break;
            case 0xB:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (BrickColor Entry): ";
                currObjectStream << gameObjects[i].getProperty(j).getBrickColorValue().getColorString() << " (" << hexVal(gameObjects[i].getProperty(j).getBrickColorValue().getColor()) << ")" << endl;
                break;
            case 0xC:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Color3 Entry): R: ";
                currObjectStream << gameObjects[i].getProperty(j).getColor3Value().getRInt() << "(" << hexVal(gameObjects[i].getProperty(j).getColor3Value().getRInt()) << " - " << gameObjects[i].getProperty(j).getColor3Value().getR() << " - " << hexValFloat(gameObjects[i].getProperty(j).getColor3Value().getR()) << "), G: ";
                currObjectStream << gameObjects[i].getProperty(j).getColor3Value().getGInt() << "(" << hexVal(gameObjects[i].getProperty(j).getColor3Value().getGInt()) << " - " << gameObjects[i].getProperty(j).getColor3Value().getG() << " - " << hexValFloat(gameObjects[i].getProperty(j).getColor3Value().getG()) << "), B: ";
                currObjectStream << gameObjects[i].getProperty(j).getColor3Value().getBInt() << "(" << hexVal(gameObjects[i].getProperty(j).getColor3Value().getBInt()) << " - " << gameObjects[i].getProperty(j).getColor3Value().getB() << " - " << hexValFloat(gameObjects[i].getProperty(j).getColor3Value().getB()) << ")" << endl;
                break;
            case 0xD:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Vector2 Entry): X: ";
                currObjectStream << gameObjects[i].getProperty(j).getVector2Value().getX() << "(" << hexValFloat(gameObjects[i].getProperty(j).getVector2Value().getX()) << "), Y: ";
                currObjectStream << gameObjects[i].getProperty(j).getVector2Value().getY() << "(" << hexValFloat(gameObjects[i].getProperty(j).getVector2Value().getY()) << ")" << endl;
                break;
            case 0xE:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Vector3 Entry): X: ";
                currObjectStream << gameObjects[i].getProperty(j).getVector3Value().getX() << "(" << hexValFloat(gameObjects[i].getProperty(j).getVector3Value().getX()) << "), Y: ";
                currObjectStream << gameObjects[i].getProperty(j).getVector3Value().getY() << "(" << hexValFloat(gameObjects[i].getProperty(j).getVector3Value().getY()) << "), Z: ";
                currObjectStream << gameObjects[i].getProperty(j).getVector3Value().getZ() << "(" << hexValFloat(gameObjects[i].getProperty(j).getVector3Value().getZ()) << ")" << endl;
                break;
            case 0x10:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (CFrame Entry): " << dumpCFrame(gameObjects[i].getProperty(j).getCFrameValue()) << endl;
                break;
            case 0x13:
                currObjectStream << gameObjects[i].getProperty(j).getPropertyName() << " (Referent Value): ";
                if (gameObjects[i].getProperty(j).getReferentValue() == -1) {
                    currObjectStream << "DataModel (";
                }
                else {
                    currObjectStream << getExtractedPath(gameObjects[i].getProperty(j).getReferentValue()) << " (";
                }
                    currObjectStream << hexVal(gameObjects[i].getProperty(j).getReferentValue()) << ")" << endl;
                break;
		}
	}
    currObjectStream.close();
  }
  cout << "Done!" << endl;
  free(ints32);
  free(floats);

  fclose(rbxl);

  return 0;
}
