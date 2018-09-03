#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include "poly.hpp"

using namespace std;

int main() {	
	vector<int> vi1 = {2,1,1};
	vector<int> vi2 = {2,1,1,1,1};
	
	poly<int> p1(vi1);
	poly<int> p2(move(vi2));
	
	vector<poly<int> > vp1 = {p1, p2};
	poly<poly<int> > ppi1(vp1);
	
	cout << ppi1 << endl;
		
	p1 = poly<int>(vector<int>{1,2,3,4});
	p2 = poly<int>(vector<int>{10,-7,-18});
	poly<int> p3 = poly<int>(vector<int>{38, 90, 74});
	
	vector<poly<int> > vp2 = {p1, p2, p3+=p1*p2};
	
	cout << p3 << "/" << p1 << " = " << p3/p1 << endl;
	cout << "True answer should be " << p2 << endl;
	cout << p3 << "%" << p1 << " = " << p3 % p1 << endl;
	cout << "True answer should be {38,90,74}" << endl;
	
	poly<poly<int> > ppi2(vp2);
	
	cout << ppi2 << endl;
	
	cout << "-: " << ppi1 - ppi2 << endl;
	cout << "*: " << ppi1 * ppi2 << endl;
	cout << "smul(" << ppi1 << "," << p1 << "): " << ppi1*p1 << endl;
	
	cout << "Hello world" << endl;
	return 0;
}
