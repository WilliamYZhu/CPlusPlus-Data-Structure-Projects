#ifndef DIFF_H
#define DIFF_H

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include "HashTable.h"
using namespace std;

int findLastMatch(string newStr, string oldStr, int offset);
bool getInt(istream& inf, int& n);
bool getCommand(istream& inf, char& cmd, int& length, int& offset);


void createDiff(istream& oldfile, istream& newfile, ostream& difffile)		
{

	///////SET UP FILE STREAM////////
	if (!oldfile)
	{
		cerr << "failed to create input file stream" << endl;
		return;
	}
	if (!newfile)
	{
		cerr << "failed to create input file stream" << endl;
		return;
	}
	if (!difffile)
	{
		cerr << "failed to create output file stream" << endl;
		return;
	}

	string oldStr;
	string newStr;
	string temp;
	char tempchar;

	getline(oldfile, temp);							//read in old file line by line
	oldStr = temp;
	oldfile.unget();
	if (oldfile.get(tempchar) && tempchar == '\n')	//if the file end with \n, make sure to add the ignored \n
		oldStr += "\n";
	while (getline(oldfile, temp))					
		oldStr += temp + "\n";

	getline(newfile, temp);							//read in new file line by line
	newStr = temp;
	newfile.unget();
	if (newfile.get(tempchar) && tempchar == '\n')	//if the file end with \n, make sure to add the ignored \n
		newStr += "\n";
	while (getline(newfile, temp))
		newStr += temp + "\n";

	///////DEFINE WINDOWSIZE////////
	int tempsize = 0;
	if (oldStr.size() < 64)
		tempsize = 4;
	else if (oldStr.size() < 128)
		tempsize = 8;
	else
		tempsize = 16;
	const int WINDOWSIZE = tempsize;


	///////SETUP HASHTABLE////////
	HashTable oldTable;
	hash<string> str_hash;		//using <functional>

	for (size_t i = 0; i < oldStr.size() - WINDOWSIZE + 1; i++)		//hash every segment within a rolling window
	{
		string substring = oldStr.substr(i, WINDOWSIZE);
		int hashcode = str_hash(substring);
		oldTable.insert(hashcode, substring, i);
	}


	///////WALK THROUGH NEW STRING////////
	/////////CREATE INSTRUCTION///////////
	string instruction;
	while (int(newStr.size()) >= WINDOWSIZE)			//while we can still process an entire window
	{
		string strSeg = newStr.substr(0, WINDOWSIZE);				
		int hashcode = str_hash(strSeg);
		int offset = oldTable.find(hashcode, strSeg, newStr, oldStr);		//find the best offset
		if (offset != -1)		//if found a matching element, we are going to copy it over
		{
			newStr = newStr.substr(WINDOWSIZE);
			int maxForwardIncrement = findLastMatch(newStr, oldStr, offset + WINDOWSIZE);
			newStr = newStr.substr(maxForwardIncrement);
			instruction += "C" + to_string(WINDOWSIZE + maxForwardIncrement) + "," + to_string(offset);
		}
		else					//if we don't find it in the old file, we are going to add a new one
								//we add as much as possible in one go, by finding the next place where we can copy things from old file
								//and add everything before that together
		{
			string toBeAdded;			
			toBeAdded += newStr[0];
			newStr = newStr.substr(1);			//move one char forward
			string strSeg2 = newStr.substr(0, WINDOWSIZE);
			int hashcode2 = str_hash(strSeg2);
			int offset2 = oldTable.find(hashcode2, strSeg2, newStr, oldStr);
			while (offset2 == -1 && newStr.size() > 0)			//while haven't encountered the next match where we can simply start to copy things over
			{
				toBeAdded += newStr[0];
				newStr = newStr.substr(1);		//move one char forward
				strSeg2 = newStr.substr(0, WINDOWSIZE);
				hashcode2 = str_hash(strSeg2);
				offset2 = oldTable.find(hashcode2, strSeg2, newStr, oldStr);		//check if the current window have matching element
			}
			instruction += "A" + to_string(toBeAdded.size()) + ":" + toBeAdded;		//add all the characters before the next match
		}
		
	}
	instruction += "A" + to_string(newStr.size()) + ":" + newStr; //Add the rest

	///////WRITE TO FILE////////
	difffile << instruction << endl;
}


//{
//	newStr = newStr.substr(WINDOWSIZE);
//	instruction += "A" + to_string(WINDOWSIZE) + ":" + strSeg;		//add these eight characters
//}



bool applyDiff(istream& oldfile, istream& difffile, ostream& newfile)
{
	///////SET UP FILE STREAM////////
	if (!oldfile)
	{
		cerr << "failed to create input file stream" << endl;
		return false;
	}
	if (!difffile)
	{
		cerr << "failed to create input file stream" << endl;
		return false;
	}
	if (!newfile)
	{
		cerr << "failed to create output file stream" << endl;
		return false;
	}

	string oldStr;
	string temp;
	char tempchar;
	
	getline(oldfile, temp);
	oldStr = temp;
	oldfile.unget();
	if (oldfile.get(tempchar) && tempchar == '\n')
		oldStr += "\n";
	while (getline(oldfile, temp))
		oldStr += temp + "\n";
	

	///////EXECUTE COMMANDS////////
	char cmd = ' ';
	int length;
	int offset;

	while (cmd != 'x')			//while haven't encountered the end of file
	{
		if (!getCommand(difffile, cmd, length, offset))		//if command is not valid
			return false;
		else if (cmd == 'A')		//Add
		{
			string temp;
			for (int i = 0; i < length; i++)	//get everything that needs to be added
			{
				temp += difffile.get();
			}
			newfile << temp;
		}
		else if (cmd == 'C')		//Copy
		{
			string temp = oldStr.substr(offset, length);
			newfile << temp;
		}
		//for \n and \r, the program will simply ignore it
	}
	return true;
}



int findLastMatch(string newStr, string oldStr, int offset)
{
	size_t i = 0;
	while (i < newStr.size() && (i + offset) < oldStr.size())
	{
		if (oldStr[i + offset] != newStr[i])
			break;
		i++;
	}

	return int(i);
}


bool getInt(istream& inf, int& n)
{
	char ch;
	if (!inf.get(ch) || !isascii(ch) || !isdigit(ch))
		return false;
	inf.unget();
	inf >> n;
	return true;
}

bool getCommand(istream& inf, char& cmd, int& length, int& offset)
{
	if (!inf.get(cmd))
	{
		cmd = 'x';  // signals end of file
		return true;
	}
	char ch;
	switch (cmd)
	{
	case 'A':
		return getInt(inf, length) && inf.get(ch) && ch == ':';		//store the length to operate in length, skip over :
	case 'C':
		return getInt(inf, length) && inf.get(ch) && ch == ',' && getInt(inf, offset);	//store length, skip over , , get offset
	case '\r':
	case '\n':
		return true;
	}
	return false;
}


#endif