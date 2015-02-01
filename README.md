# RBXLAnalysis
An application for converting .rbxl files into a VCS-friendly form.

## Installing

### Cygwin
Clone the repo, get Cygwin with g++ and compile with the following command:

    g++ -O2 -Wall -finline-functions analysis.cpp -o analysis

### GNU/Linux
Clone the repo, get build-essentials and compile with the following command:

    g++ -O2 -Wall -finline-functions analysis.cpp -o analysis

## Usage
I plan on adding more options to the app later, but for now it can only be used to dump the information.
Use the following command to dump:

    analysis /path/to/rbxl-file

It will create a new directory called "extract" containing all the possible properties of the instances inside the file.
