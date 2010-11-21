#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

/** This reads a given filename and creates
 * a char array which can be passed to the 
 * shader compiler
 */
char* readShader(std::string filename);
