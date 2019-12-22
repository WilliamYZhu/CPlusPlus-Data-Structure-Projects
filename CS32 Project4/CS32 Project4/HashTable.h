#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const int BUCKETNUMBER = 1009;


//use std::pair for string-count pair

//put into vector

//have a hash table containig pointers to each vector



class item			//item is for storing string and offset pair in a hashtable 
{
public:
	item(int hashcode, string str, int offset);
	int hashcode();
	string str();
	int offset();

private:
	int m_hashcode;
	string m_str;
	int m_offset;
};

item::item(int hashcode, string str, int offset)
{
	m_hashcode = hashcode;
	m_str = str;
	m_offset = offset;
}

int item::hashcode()
{
	return m_hashcode;
}

string item::str()
{
	return m_str;
}

int item::offset()
{
	return m_offset;
}



class HashTable			
{
public:
	HashTable();
	int size();
	void insert(int hashcode, string str, int offset);
	int find(int hashcode, const string& strSeg, const string& newStr, const string& oldStr);
	int findLastMatch(string newStr, string oldStr, int offset);
	
private:
	vector<vector<item>> ptrTable;			//the hash table is implemented as a vector of buckets, where each bucket is a vector of items
};

HashTable::HashTable()
{
	for (int i = 0; i < BUCKETNUMBER; i++)			//initialize the vectors
	{
		vector<item> vec;
		ptrTable.push_back(vec);
	}
}

int HashTable::size()
{
	return ptrTable.size();	
}

void HashTable::insert(int hashcode, string str, int offset)			
//use the hashed bucketcode to find the bucket, push_back a new entry
{
	int bucketcode = abs(hashcode % BUCKETNUMBER);
	ptrTable[bucketcode].push_back(item(hashcode, str, offset));
}

int HashTable::find(int hashcode, const string& strSeg, const string& newStr, const string& oldStr)		
//find at most SEARCHLIMIT number of segments that matches with one we are looking for, return the one that continues to match for the longest
{
	const int SEARCHLIMIT = 20;
	int searchcount = 0;
	int best = -1;
	vector<int> offsets;		//store the offset position
	vector<int> extensions;		//store how long it continues afterwards

	int bucketcode = abs(hashcode % BUCKETNUMBER);
	if (ptrTable[bucketcode].size() != 0)				//if there is something in the bucket
	{
		for (size_t i = 0; i < ptrTable[bucketcode].size(); i++)		//for everything in the vector of that bucket
		{
			if (ptrTable[bucketcode][i].str() == strSeg)				//if the element(string) matches with what we are looking for
			{
				int offset = ptrTable[bucketcode][i].offset();			//keep track of both where they are (offsets) and how long they continue (extensions)
				offsets.push_back(offset);
				int extension = findLastMatch(newStr, oldStr, offset);
				extensions.push_back(extension);						
				searchcount++;
			}
			if (searchcount >= SEARCHLIMIT)		//if found too many matching items
				break;
		}
		if (offsets.size() > 0)			//if at least found one element that match 
		{
			int bestIndex = max_element(extensions.begin(), extensions.end()) - extensions.begin();
			best = offsets[bestIndex];
		}
	}
	return best;
}

int HashTable::findLastMatch(string newStr, string oldStr, int offset)
//a helper function for HashTable::find
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


#endif