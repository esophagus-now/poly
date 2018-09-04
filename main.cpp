#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include "poly.hpp"
#include "gf.hpp"
#include "printer.hpp"

using namespace std;

poly<GF<unsigned> > gf2poly(string s) {
	vector<GF<unsigned> > v;
	
	for (const auto &a: s) {
		if (a == '0') {
			v.emplace_back(0,2,0,1);
		} else {
			v.emplace_back(1,2,0,1);
		}
	}
	
	return poly<GF<unsigned> >(move(v));
}

int main() {
	auto p1 = gf2poly("10010010");
	auto p2 = gf2poly("01110111");
	auto zero = gf2poly("0");
	auto one = gf2poly("1");
	auto mod = gf2poly("110000111");
	auto thing1 = GF<poly<GF<unsigned> > >(p1, mod, zero, one);
	auto thing2 = GF<poly<GF<unsigned> > >(p2, mod, zero, one);
	cout << p1.compact("") << endl;
	cout << p2.compact("") << endl;
	cout << (p1+p2).compact("") << endl << endl;
	cout << (p1*p2).compact("") << endl;
	cout << ((p1*p2)%mod).compact("") << endl;
	
	auto g1 = GF<int>(3,19,0,1);
	auto g2 = GF<int>(4,19,0,1);
	cout << g2/g1 << endl;
	
	cout << thing1.compact("") << endl;
	return 0;
}
