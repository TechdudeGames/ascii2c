//ascii2c.cpp
//Purpose: To convert a text file (passed to the program via the command line or Windows drag and drop) into c code for use in making a bootlogo for the Rockbox bootloader.
//Date: Jan 16, 2021
//Author: TechdudeGames https://github.com/TechdudeGames

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main ( int argc, char *argv[] )

{
	//Declare some variables
	FILE *fp;
	unsigned long long size;
	char *buffer;
	string filename = "";
	string output;
	if(argc == 1){
		//Only the program was passed, therefor we are do usage
		cout << "ASCII2C by TechdudeGames" << endl;
		cout << "Usage: ASCII2C infile" << endl;
		return 0;
	}
	//We got here so I suppose the next parts are filename sections
	for(int i = 1; i != argc; i++){
		filename += argv[i];
	}

	fp = fopen(filename.c_str(), "r");
	if(fp == nullptr){
		cout << "File " << filename << " doesn't exist." << endl;
		return 1;
	}
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);
	buffer = (char*) malloc(sizeof(char) * size);
	fread(buffer, 1, size, fp);
	fclose(fp);

	//Start writing code
	//This works by adding it a character at a time since there are certain ones that need escape codes to appear correctly.
	output += "printf(\"";
	for(unsigned long long j = 0; j != size; j++){
		//cout << buffer[j];
		switch (buffer[j]){
			case '\\':
				output += "\\\\";
				break;
			case '\n':
				//In Rockbox each printf automatically increases the line number, so we don't add \n
				output += "\");\nprintf(\"";
				break;
			case '\t':
				output += "\\t";
				break;
			case '\"':
				output += "\\\"";
				break;
			case '%':
				output += "%%%%";
				break;
			case '\'':
				output += "\\\'";
				break;
			default:
				output += buffer[j];
				break;
		}
	}
	//Finishing the last line
	output += "\");";
	//Piping to a new file in the current directory
	fp = fopen("ascii.c", "w+");
	if(fp == nullptr){
		cout << "Couldn't open file for writing." << endl;
		return 1;
	}
	fwrite(output.c_str(), 1, output.length(), fp);
	fclose(fp);
	//Free buffer
	free(buffer);
	return 0;
}
