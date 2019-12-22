#include <fstream>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <functional>
#include <iostream>
#include <sstream>  // for istringstream and ostringstream
#include <string>
#include "HashTable.h"
#include "diff.h"

using namespace std;

bool runtest(string oldName, string newName, string diffName, string newName2)
{
	if (diffName == oldName || diffName == newName ||
		newName2 == oldName || newName2 == diffName ||
		newName2 == newName)
	{
		cerr << "Files used for output must have names distinct from other files" << endl;
		return false;
	}
	ifstream oldFile(oldName, ios::binary);
	if (!oldFile)
	{
		cerr << "Cannot open " << oldName << endl;
		return false;
	}
	ifstream newFile(newName, ios::binary);
	if (!newFile)
	{
		cerr << "Cannot open " << newName << endl;
		return false;
	}
	ofstream diffFile(diffName, ios::binary);
	if (!diffFile)
	{
		cerr << "Cannot create " << diffName << endl;
		return false;
	}
	createDiff(oldFile, newFile, diffFile);
	diffFile.close();

	oldFile.clear();   // clear the end of file condition
	oldFile.seekg(0);  // reset back to beginning of the file
	ifstream diffFile2(diffName, ios::binary);
	if (!diffFile2)
	{
		cerr << "Cannot read the " << diffName << " that was just created!" << endl;
		return false;
	}
	ofstream newFile2(newName2, ios::binary);
	if (!newFile2)
	{
		cerr << "Cannot create " << newName2 << endl;
		return false;
	}
	assert(applyDiff(oldFile, diffFile2, newFile2));
	newFile2.close();

	newFile.clear();
	newFile.seekg(0);
	ifstream newFile3(newName2, ios::binary);

	if (!newFile)
	{
		cerr << "Cannot open " << newName2 << endl;
		return false;
	}
	if (!equal(istreambuf_iterator<char>(newFile), istreambuf_iterator<char>(),
		istreambuf_iterator<char>(newFile3), istreambuf_iterator<char>()))
	{

		cerr << newName2 << " is not identical to " << newName
			<< "; test FAILED" << endl;
		return false;
	}
	return true;
}




int main()
{

	assert(runtest("greeneggs1.txt", "greeneggs2.txt", "greeneggsDiff.txt", "greeneggsNew.txt"));
	assert(runtest("smallmart1.txt", "smallmart2.txt", "smallmartDiff.txt", "smallmartNew.txt"));
	assert(runtest("warandpeace1.txt", "warandpeace2.txt", "warandpeaceDiff.txt", "warandpeaceNew.txt"));
	assert(runtest("strange1.txt", "strange2.txt", "strangeDiff.txt", "strangeNew.txt"));

	////assert(runtest("test1.txt", "test2.txt", "testDiff.txt", "testNew.txt"));
	//ifstream oldFile("test1.txt", ios::binary);
	//ifstream oldFile2("test2.txt", ios::binary);
	//ifstream diffFile("testDiff.txt", ios::binary);
	//ofstream newFile("testNew.txt", ios::binary);

	//applyDiff(oldFile, diffFile, newFile);

	//
	cerr << "Test PASSED" << endl;

	return 0;
}