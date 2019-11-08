#include "Sequence.h"
#include <iostream>
using namespace std;

Sequence::Sequence()
{
	m_head = nullptr;
	m_size = 0;
}

Sequence::Sequence(const Sequence& other)					//copy constructor
{
	if (other.m_head == nullptr)	//if other is an empty Sequence
	{
		this->m_head = nullptr;
		this->m_size = 0;
	}
	else							//if other has stuff in it
	{
		Node* p = other.m_head;		//define p to walk the Sequence
		this->m_head = nullptr;
		this->m_size = 0;

		while (p != nullptr)		//insert what p points to into the new Sequence, walks p until the end (when p = nullptr)
		{
			insert(m_size, p->m_value); 
			p = p->m_next;
		}
	}

}

Sequence& Sequence::operator =(const Sequence& other)		//assignment operator
{
	if (this == &other)				//if both sides of = are the same object, do nothing and return *this
		return *this;
	
	Sequence temp(other);			//create a temp object as a copy of other
	swap(temp);						//swap m_head and m_size

	return *this;					//return the new object. temp containing the old Sequence is deleted by destructor
}

bool Sequence::empty() const
{
	if (m_size == 0)
		return true;
	else
		return false;
}

int Sequence::size() const
{
	return m_size;
}

int Sequence::insert(int pos, const ItemType& value)
{
	if (pos > m_size || pos < 0) //if pos is invalid 
		return -1;

	if (m_head == nullptr)	//if initially empty
	{
		Node* p = new Node();	//create new object p with previous and next node being nothing
		p->m_next = nullptr;
		p->m_prev = nullptr;
		p->m_value = value;	
		m_head = p;				//change m_head of this Sequence to point to the new Node

		m_size++;

		return 0;
	}

							//if not initially empty

	Node* p = m_head;		//create temporary pointer p
	
	if (pos == 0)			//if insert at beginning 
	{
		Node* newNode = new Node(); 
		newNode->m_next = p;			//next node is the original first node, which is pointed by m_head
		newNode->m_prev = nullptr;		//previous Node doesn't exist
		newNode->m_value = value;		

		p->m_prev = newNode;			//make the original first node to point back to the new node as m_prev
		
		m_head = newNode;				//switch m_head to the new first node
	}
	else
	{
		for (int i = 0; i < pos - 1; i++) //walks, so that temp points at the element at (pos - 1)
		{
			p = p->m_next;
		}

		Node* newNode = new Node();		//create new node
		newNode->m_value = value;
		newNode->m_prev = p;			//newNode points back at the previous node at (pos-1)

		if (pos == m_size)				//if the newNode is at the end;
		{
			newNode->m_next = nullptr;  //the next node then should be nothing
		}
		else
		{
			newNode->m_next = p->m_next;			//newNode points to the next Node (originally the one at pos)
			newNode->m_next->m_prev = newNode;		//the original node at pos should point back at newNode
		}
		p->m_next = newNode;			//make the node at (pos-1) point to newNode
	}

	m_size++;		//increase size by one (true regardless where to insert)
	return pos;
}

int Sequence::insert(const ItemType& value)
{
	int index = size();					//default place to insert is at the end, if value > all nodes
	ItemType x;

	for (int i = 0; i < size(); i++)	//if any node's m_value is greater or equal to value
	{
		get(i, x);					
		if (value <= x)
		{
			index = i;					//then set index to that position
			break;
		}
	}
	
	if (insert(index, value) == -1)		//if cannot insert for some reason
		return -1;
	else
		return index;
	
}

bool Sequence::erase(int pos)
{
	if (pos < 0 || pos >= m_size)		//if invalid position
		return false;

	if (m_head == nullptr)				//if the Sequence is empty
		return false;

	Node* p = m_head;			//create temporary pointer p

	if (pos == 0)				//if erase at beginning 
	{
		m_head = p->m_next;		//make m_head points to what's after (could be nullptr if there was only one node)

		if (m_size != 1)		//if the first node isn't the only node
			p->m_next->m_prev = nullptr;		//the next node's m_prev pointer should be nullptr

		delete p;				//delete the first node
	}
	else
	{
		for (int i = 0; i < pos; i++)	//walks p, so that p points at the element at pos
		{
			p = p->m_next;	
		}

		p->m_prev->m_next = p->m_next;		//the node before p should now point to the node after p (skipping p)
											//p->m_next could be nullptr
				
		if (pos < m_size -1)					//if not deleting at the end;
			p->m_next->m_prev = p->m_prev;		//the next node's previous node should now be what's before p

		delete p;							//delete this node
	}

	m_size--;		//decrease size by one (true regardless where to insert)
	return true;
}

int Sequence::remove(const ItemType& value)
{
	if (m_head == nullptr)	//if sequence is empty
		return 0;			//removed zero instances

	Node* p = m_head;		//temporary pointer p 
	int index = 0;			//keep track of where p is
	int count = 0;			//keep track of how many removed

	while (p != nullptr)	//when not at the end
	{
		if (p->m_value == value)	//if the value match
		{
			p = p->m_next;			//advance p
			erase(index);			//erase at the current index
			count++;				
			continue;				//check the same position again
		}
		else
		{
			p = p->m_next;			//if no match, moves to the next one
		}
		index++;					//index increases after p moves
	}

	return count;
}

bool Sequence::get(int pos, ItemType& value) const
{
	if (pos < 0 || pos >= m_size)			//if invalid position
		return false;
	
	if (m_head == nullptr)					//if Sequence empty
		return false;
		
	Node* p = m_head;						//create temporary pointer p
	
	for (int i = 0; i < pos; i++)	//walks p to pos
	{
		p = p->m_next;
	}

	value = p->m_value;		//copy m_value at p into value
	
	return true;
}

bool Sequence::set(int pos, const ItemType& value)
{
	if (pos < 0 || pos >= m_size)		//if invalid position
		return false;
	
	if (m_head == nullptr)				//if Sequence empty
		return false;

	Node* p = m_head;				//create temporary pointer p

	for (int i = 0; i < pos; i++)	//walks p to pos
	{
		p = p->m_next;				
	}

	p->m_value = value;		//copy m_value at p into value

	return true;
}

int Sequence::find(const ItemType& value) const
{
	if (m_head == nullptr)		//if Sequence empty
		return -1;				//not possible to find anything

	Node* p = m_head;
	int index = 0;			
	bool found = false;			

	while (p != nullptr)			//while not at the end
	{
		if (p->m_value == value)	//if value match
		{
			found = true;			//set flag
			break;					//stop checking
		}
		p = p->m_next;				//advance to the next if no value match
		index++;					
	}
	
	if (found)						//if the flag is set to true
		return index;				//return the right index
	else							//if not found and reached the end of Sequence
		return -1;					//failed to find
}

void Sequence::swap(Sequence& other) 
{
	Node* tempHead = this->m_head;		//exchange m_head
	this->m_head = other.m_head;
	other.m_head = tempHead;

	int tempSize = this->m_size;		//exchange m_size
	this->m_size = other.m_size;
	other.m_size = tempSize;
}

void Sequence::dump() const
{
	ItemType item;
	
	for (int i = 0; i < size(); i++)
	{
		get(i, item);
		cerr << item << endl;
	}
}

Sequence::~Sequence()					//destructor
{
	if (m_head != nullptr)				//if Sequence is not empty (if it is, just let m_head and m_size go out of scope)
	{
		Node* p = m_head;				

		while (p->m_next != nullptr)	//when not at last node
		{
			p = p->m_next;				//advance to the next node 
			delete p->m_prev;			//delete previous node
		}

		delete p;						//after getting to the last node, delete it
	}
}

int subsequence(const Sequence& seq1, const Sequence& seq2)
{
	if (seq1.empty() || seq2.empty() || (seq1.size() < seq2.size()))	//when one of them is empty or seq1 is smaller than seq2 
		return -1;				//not possible to find

	if (&seq1 == &seq2)			//if they are the same Sequence
		return 0;				//matches from the first value

	int startPt = 0;
	bool contains = false;
	ItemType value1, value2;

	for (int i = 0; i < seq1.size(); i++)		//loop through seq1
	{
		seq1.get(i, value1);		//get node i in seq1
		seq2.get(0, value2);		//get the first node in seq2

		if (value1 == value2)		//if found one term match
		{
			for (int a = 0; a < seq2.size(); a++)	//loop through the rest of seq1 and seq2 TOGETHER
			{
				if (!seq1.get(a + i, value1) || !seq2.get(a, value2)) //if seq1 reached end
				{
					contains = false;
					break;
				}

				if (value1 != value2) //not matched
				{
					contains = false;
					break;
				}

				if ((a + i + 1) == seq1.size() && (a + 1) < seq2.size()) //if seq1 reached end while seq2 did not
				{
					contains = false;
					break;
				}

				contains = true;		//if above didn't happen and break, default to true
				startPt = i;			//record the index
			}	
		}
		
		if (contains)	//stop checking seq1's other entries if already found a subsequence starting point
			break;
	}

	if (contains)			//if found
		return startPt;
	else					//if didn't find subsequence and reached the end of seq1
		return -1;			
}

void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result)
{
	Sequence temp;				//create temp Sequence to store the new Sequence, will swap with result at the endd

	int maxSize;
	if (seq1.size() > seq2.size())		//find the max times it needs to loop through both sequence
		maxSize = seq1.size();
	else
		maxSize = seq2.size();
		
	ItemType item;
	int s = 0;						//current index in temp
	for (int i = 0; i < maxSize; i++)
	{
		if (seq1.get(i, item))		//if successfully gotten the value
		{
			temp.insert(s, item);	//insert and mark increase in temp's index
			s++;
		}
		
		if (seq2.get(i, item))		//if successfully gotten the value 
		{
			temp.insert(s, item);	//insert and mark increase in temp's index
			s++;
		}
	}

	temp.swap(result);	//swap temp with result so that result holds the interleaved sequence
						//let the original contents in s3 go out of scope with temp and cleaned by destructor
}