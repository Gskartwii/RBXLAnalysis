#ifndef _BRICKCOLOR_CPP
#define _BRICKCOLOR_CPP
#include <string>

using namespace std;

class BrickColor {
  int color;

public:
  void setColor(int);
  int  getColor();
  string getColorString();

  BrickColor& operator=(BrickColor*);
};

BrickColor& BrickColor::operator=(BrickColor* n) {
  color = n->getColor();
  return *this;
}

void BrickColor::setColor(int n) {
  color = n;
}

int BrickColor::getColor() {
  return color;
}
string BrickColor::getColorString() {
  switch (color) { // Just so happens that I have this already done in PHP, just a quick copy-paste and...
	case 1:
		return "White";
	case 2:
		return "Grey";
	case 3:
		return "Light yellow";
	case 5:
		return "Brick yellow";
	case 6:
		return "Light green (Mint)";
	case 9:
		return "Light-reddish violet";
	case 11:
		return "Pastel Blue";
	case 12:
		return "Light orange brown";
	case 18:
		return "Nougat";
	case 21:
		return "Bright red";
	case 22:
		return "Med. reddish violet";
	case 23:
		return "Bright blue";
	case 24:
		return "Bright yellow";
	case 25:
		return "Earth orange";
	case 26:
		return "Black";
	case 27:
		return "Dark grey";
	case 28:
		return "Dark green";
	case 29:
		return "Medium green";
	case 36:
		return "Lig. Yellowich orange";
	case 37:
		return "Bright green";
	case 38:
		return "Dark orange";
	case 39:
		return "Light bluish violet";
	case 40:
		return "Transparent";
	case 41:
		return "Tr. Red";
	case 42:
		return "Tr. Lg blue";
	case 43:
		return "Tr. Blue";
	case 44:
		return "Tr. Yellow";
	case 45:
		return "Light blue";
	case 47:
		return "Tr. Flu. Reddish orange";
	case 48:
		return "Tr. Green";
	case 49:
		return "Tr. Flu Green";
	case 50:
		return "Phosph. White";
	case 100:
		return "Light red";
	case 101:
		return "Medium red";
	case 102:
		return "Medium blue";
	case 103:
		return "Light grey";
	case 104:
		return "Bright violet";
	case 105:
		return "Br. yellowish orange";
	case 106:
		return "Bright orange";
	case 107:
		return "Bright bluish green";
	case 108:
		return "Earth yellow";
	case 110:
		return "Bright bluish violet";
	case 111:
		return "Tr. Brown";
	case 112:
		return "Medium bluish violet";
	case 113:
		return "Tr. Medi. reddish violet";
	case 115:
		return "Med. yellowish green";
	case 116:
		return "Med. bluish green";
	case 118:
		return "Light bluish green";
	case 119:
		return "Br. yellowish green";
	case 120:
		return "Lig. yellowish green";
	case 121:
		return "Med. yellowish orange";
	case 123:
		return "Br. reddish orange";
	case 124:
		return "Bright reddish violet";
	case 125:
		return "Light orange";
	case 126:
		return "Tr. Bright bluish violet";
	case 127:
		return "Gold";
	case 128:
		return "Dark nougat";
	case 131:
		return "Silver";
	case 133:
		return "Neon orange";
	case 134:
		return "Neon green";
	case 135:
		return "Sand blue";
	case 136:
		return "Sand violet";
	case 137:
		return "Medium orange";
	case 138:
		return "Sand yellow";
	case 140:
		return "Earth blue";
	case 141:
		return "Earth green";
	case 143:
		return "Tr. Flu. Blue";
	case 145:
		return "Sand blue metallic";
	case 146:
		return "Sand violet metallic";
	case 147:
		return "Sand yellow metallic";
	case 148:
		return "Dark grey metallic";
	case 149:
		return "Black metallic";
	case 150:
		return "Light grey metallic";
	case 151:
		return "Sand green";
	case 153:
		return "Sand red";
	case 154:
		return "Dark red";
	case 157:
		return "Tr. Flu. Yellow";
	case 158:
		return "Tr. Flu. Red";
	case 168:
		return "Gun metallic";
	case 176:
		return "Red flip/flop";
	case 178:
		return "Yellow flip/flop";
	case 179:
		return "Silver flip/flop";
	case 180:
		return "Curry";
	case 190:
		return "Fire Yellow";
	case 191:
		return "Flame yellowish orange";
	case 192:
		return "Reddish brown";
	case 193:
		return "Flame reddish orange";
	case 194:
		return "Medium stone grey";
	case 195:
		return "Royal blue";
	case 196:
		return "Dark Royal blue";
	case 198:
		return "Bright reddish lilac";
	case 199:
		return "Dark stone gray";
	case 200:
		return "Lemon metalic";
	case 208:
		return "Light stone gray";
	case 209:
		return "Dark Curry";
	case 210:
		return "Faded green";
	case 211:
		return "Turqoise";
	case 212:
		return "Light Royal blue";
	case 213:
		return "Medium Royal blue";
	case 216:
		return "Rust";
	case 217:
		return "Brown";
	case 218:
		return "Reddish lilac";
	case 219:
		return "Lilac";
	case 220:
		return "Light lilac";
	case 221:
		return "Bright purple";
	case 222:
		return "Light purple";
	case 223:
		return "Light pink";
	case 224:
		return "Light brick yellow";
	case 225:
		return "Warm yellowish orange";
	case 226:
		return "Cool yellow";
	case 232:
		return "Dove blue";
	case 268:
		return "Medium lilac";
	case 1001:
		return "Instutional white";
	case 1002:
		return "Mid gray";
	case 1003:
		return "Really black";
	case 1004:
		return "Really red";
	case 1005:
		return "Neon orange";
	case 1006:
		return "Alder";
	case 1007:
		return "Dusty Rose";
	case 1008:
		return "Olive";
	case 1009:
		return "New Yeller";
	case 1010:
		return "Really blue";
	case 1011:
		return "Navy blue";
	case 1012:
		return "Deep blue";
	case 1013:
		return "Cyan";
	case 1014:
		return "CGA brown";
	case 1015:
		return "Magenta";
	case 1016:
		return "Pink";
	case 1017:
		return "Deep orange";
	case 1018:
		return "Teal";
	case 1019:
		return "Toothpaste";
	case 1020:
		return "Lime green";
	case 1021:
		return "Camo";
	case 1022:
		return "Grime";
	case 1023:
		return "Lavender";
	case 1024:
		return "Pastel light blue";
	case 1025:
		return "Pastel orange";
	case 1026:
		return "Pastel violet";
	case 1027:
		return "Pastel blue-green";
	case 1028:
		return "Pastel green";
	case 1029:
		return "Pastel yellow";
	case 1030:
		return "Pastel brown";
	case 1031:
		return "Royal purple";
	case 1032:
		return "Hot pink";
	default:
		return "ERR_UNKNOWN_BRICKCOLOR_VALUE";
  }
}

#endif
