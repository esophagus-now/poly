#include <iostream>
#include <string>
#include <sstream>
#include <type_traits>
#include "poly.hpp"
#include "gf.hpp"
#include "printer.hpp"

using namespace std;

typedef GF<unsigned> GF2;
typedef GF<poly<GF2> > GF2m;
typedef GF<poly<GF2m > > RS; //Reed-Solomon?

poly<GF2 > gf2poly(string s) {
	vector<GF2 > v;
	
	for (const auto &a: s) {
		if (a == '0') {
			v.emplace_back(0,2,0,1);
		} else {
			v.emplace_back(1,2,0,1);
		}
	}
	
	return poly<GF<unsigned> >(move(v));
}

template <typename T> //Auto-deduced?
vector<GF<T> > getcgroup(const GF<T> &g) {
	vector<GF<T> > ret;
	ret.emplace_back(g); //Should use copy constructor
	
	auto tmp = g;
	
	do {
		tmp *= g;
		ret.emplace_back(tmp); //Should use copy constructor
	} while(tmp != g.one && tmp != g);
	
	return ret;
}

int main() {
	auto p1 = gf2poly("10010010");
	auto p2 = gf2poly("01110111");
	auto zero = gf2poly("00000000");
	auto one = gf2poly("00000001");
	auto mod = gf2poly("110000111"); //Primitive
	auto thing1 = GF2m(p1, mod, zero, one);
	auto thing2 = GF2m(p2, mod, zero, one);
	cout << p1 << endl;
	cout << p2 << endl;
	cout << p1+p2 << endl << endl;
	cout << p1*p2 << endl;
	cout << (p1*p2)%mod << endl;
	
	auto g1 = GF<unsigned>(3,19,0,1);
	auto g2 = GF<unsigned>(4,19,0,1);
	cout << g2/g1 << endl;
	
	auto test = GF2m(gf2poly("10110011"), mod, zero, one);
	auto cgrp = getcgroup(test);
	for (auto &a: cgrp) {
		cout << a << ", ";
	}
	cout << endl << endl;
	
	for (auto &a: getcgroup(thing1)) {
		cout << a << ", ";
	}
	cout << endl << endl;
	
	for (auto &a: getcgroup(thing2)) {
		cout << a << ", ";
	}
	cout << endl;
	
	vector<char> vc({'a', 'b', 'c', 'd', 'e'});
	do {
		for (const auto &c : vc) {
			cout << c;
		}
		cout << endl;
	} while (next_permutation(vc.begin(), vc.end()));
	
	return 0;
}
