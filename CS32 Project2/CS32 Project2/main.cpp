


#include "Sequence.h"
#include <iostream>
#include <cassert>
using namespace std;

void test()
{
	Sequence s;
	assert(s.insert(0, "10") == 0);
	assert(s.insert(0, "20") == 0); // 20, 10
	assert(s.size() == 2);

	ItemType var = "999";
	assert(s.get(0, var) && var == "20"); 
	assert(s.get(1, var) && var == "10");	//test correct insert position

	Sequence t;
	assert(t.insert(0, "1000") == 0);
	assert(t.insert(0, "2000") == 0);
	assert(t.size() == 2);

	assert(t.find("1000") == 1);
	assert(t.find("2000") == 0);		//test find
	
	s.swap(t);

	assert(s.get(0, var) && var == "2000");
	assert(s.get(1, var) && var == "1000");	//test swap

	s.erase(1);								//test erase
	assert(s.size() == 1);					//test correctly changed size

	s.swap(t);							
		
	assert(s.size() == 2);					//test size swapping

	assert(t.get(0, var) && var == "2000");
	assert(s.get(0, var) && var == "20");
	assert(s.get(1, var) && var == "10");	//test correct exchange of entire Sequence

	assert(t.remove("2000") == 1);			
	assert(s.remove("20") == 1);
	assert(s.remove("10") == 1);			//test remove

	assert(t.size() == 0);					
	assert(s.size() == 0);					//test empty
	
	assert(s.insert(0, "1") == 0);
	assert(s.insert(1, "2") == 1);
	assert(s.insert(2, "4") == 2);
	assert(s.insert(2, "3") == 2);		
	assert(s.insert(4, "4") == 4);

	assert(s.get(0, var) && var == "1");
	assert(s.get(1, var) && var == "2");
	assert(s.get(2, var) && var == "3");
	assert(s.get(3, var) && var == "4");
	assert(s.get(4, var) && var == "4");	//test correct insert position

	assert(s.find("4") == 3);				//test find with duplicates

	assert(s.remove("4") == 2);				//test erase

	Sequence n(s);
	assert(n.get(0, var) && var == "1");
	assert(n.get(1, var) && var == "2");
	assert(n.get(2, var) && var == "3");	//test copy constructor

	Sequence m = n;
	assert(n.get(0, var) && var == "1");
	assert(n.get(1, var) && var == "2");
	assert(n.get(2, var) && var == "3");	//test operator =

	Sequence o, p;
	o = p;
	assert(o.size() == 0 && p.size() == 0);	//test operator = with empty sequence
	o = m;
	assert(o.size() == 3);					//test operator = with empty sequence
	p = o;
	assert(o.size() == 3);					//test operator = with empty sequence

	Sequence q;
	Sequence r(q);
	assert(q.size() == 0 && r.size() == 0);	//test copy constructor on empty sequence

	p = q;
	assert(p.size() == 0);					//test operator = with empty sequence


	//========== subsequence =========//

	Sequence a;
	Sequence b;
	Sequence c;
	Sequence d;

	assert(subsequence(a, a) == -1);		
			//test for empty sequence

	assert(a.insert(0, "10") == 0);			
	assert(a.insert(1, "11") == 1);
	assert(a.insert(2, "12") == 2);
	assert(a.insert(3, "13") == 3);
	assert(a.insert(4, "14") == 4);	

	assert(subsequence(a, a) == 0);			//test for same sequence
	assert(subsequence(a, b) == -1);		
	assert(subsequence(b, a) == -1);		//test for empty sequence

	assert(b.insert(0, "14") == 0);

	assert(c.insert(0, "10") == 0);
	assert(c.insert(1, "11") == 1);
	assert(c.insert(2, "12") == 2);
	assert(c.insert(3, "13") == 3);
	assert(c.insert(4, "14") == 4);
	assert(c.insert(5, "15") == 5);

	assert(d.insert(0, "10") == 0);
	assert(d.insert(1, "12") == 1);
	assert(d.insert(2, "13") == 2);
	assert(d.insert(3, "14") == 3);

	assert(subsequence(a, b) == 4);		//test for containing Sequence of one element
	assert(subsequence(a, c) == -1);	//test for size constraint
	assert(subsequence(a, d) == -1);	//test for non-continuous match

	assert(subsequence(b, d) == -1);	//test for reverse order (should fail)
	assert(subsequence(d, b) == 3);		//test for containing Sequence of one element
	assert(subsequence(b, c) == -1);	//test for reverse order (should fail) 
	assert(subsequence(c, b) == 4);		//test for containing Sequence of one element
	
	

	//========== interleave =========//

	Sequence x, y, result;
	ItemType item;

	interleave(x, y, result);
	assert(result.size() == 0);			//test for when both sequence empty
	interleave(x, x, result);
	assert(result.size() == 0);			//test for when both are empty and are the same

	assert(x.insert(0, "100") == 0);
	assert(x.insert(1, "300") == 1);
	assert(x.insert(2, "500") == 2);

	interleave(x, x, result);			
	assert(result.size() == 6);			//test for aliasing (should duplicate x)

	assert(y.insert(0, "200") == 0);
	assert(y.insert(1, "400") == 1);
	assert(y.insert(2, "600") == 2);
	assert(y.insert(3, "1000") == 3);
	assert(y.insert(4, "1000") == 4);

	interleave(x, y, result);

	result.get(0, item);
	assert(item == "100");
	result.get(1, item);
	assert(item == "200");
	result.get(2, item);
	assert(item == "300");
	result.get(3, item);
	assert(item == "400");
	result.get(4, item);
	assert(item == "500");
	result.get(5, item);
	assert(item == "600");
	result.get(6, item);
	assert(item == "1000");
	result.get(7, item);
	assert(item == "1000");	//test for correct interleave order for sequence of different size

	interleave(x, x, x);
	assert(x.size() == 6);	//test for aliasing

	x.get(0, item);
	assert(item == "100");
	x.get(1, item);
	assert(item == "100");
	x.get(2, item);
	assert(item == "300");
	x.get(3, item);
	assert(item == "300");
	x.get(4, item);
	assert(item == "500");
	x.get(5, item);
	assert(item == "500");	//test for correct order of duplication

	interleave(x, y, x);
	assert(x.size() == 11);	//test for aliasing

	x.get(0, item);
	assert(item == "100");
	x.get(1, item);
	assert(item == "200");
	x.get(2, item);
	assert(item == "100");
	x.get(3, item);
	assert(item == "400");
	x.get(4, item);
	assert(item == "300");
	x.get(5, item);
	assert(item == "600");
	x.get(6, item);
	assert(item == "300");
	x.get(7, item);
	assert(item == "1000");
	x.get(8, item);
	assert(item == "500");
	x.get(9, item);
	assert(item == "1000");
	x.get(10, item);
	assert(item == "500");	//test correct order in the case with aliasing

}

int main()
{
	test();
	cerr << "Passed all tests" << endl;
}