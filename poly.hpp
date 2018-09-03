#ifndef ELEM_HPP
#define ELEM_HPP 1

#include <iostream>
#include <vector>
#include <stdexcept>
#include <sstream>

//A lot of this stuff is coded in the easiest way possible. In reality,
//the implementation has a lot of unnecessary object copies. I think I
//know how to optimize this code, but it would become a lot messier...

template <typename T>
class poly {
	std::vector<T> c; //coeffs
	
	public:
	///Constructors, destructors, etc.
	poly() {} //Need to include this even though it is empty
	poly(const std::vector<T> &cin) : c(cin.begin(), cin.end()) {}
	poly(std::vector<T> &&cin) : c(cin) {} //Should invoke vector's move constructor
	
	//Compiler should auto-generate the copy and move ctors, and the
	//dtor. I think it should also handle the assignments and move-
	//assignment operators too. How do I check?
	
	///Arithmetic
	poly operator+(const poly &other) const {
		poly<T> ret;
		
		int offset = c.size() - other.c.size();
		auto it_this = c.begin();
		auto end_this = c.end();
		
		auto it_other = other.c.begin();
		//auto end_other = other.c.end();
		
		for (;offset < 0; offset++, ++it_other) {
			ret.c.push_back(*it_other);
		}
		
		for (;offset > 0; offset--, ++it_this) {
			ret.c.push_back(*it_this);
		}
		
		for (; it_this != end_this; ++it_this, ++it_other) {
			ret.c.push_back(*it_this + *it_other);
		} 
		
		return ret;
	}
	
	//Right now this is just a short hand for x = x + y. It is not
	//faster (not that that really matters)
	poly &operator+=(const poly &other) {
		std::vector<T> newc;
		
		int offset = c.size() - other.c.size();
		auto it_this = c.begin();
		auto end_this = c.end();
		
		auto it_other = other.c.begin();
		
		for (;offset < 0; offset++, ++it_other) {
			newc.push_back(*it_other);
		}
		
		for (;offset > 0; offset--, ++it_this) {
			newc.push_back(*it_this);
		}
		
		for (; it_this != end_this; ++it_this, ++it_other) {
			newc.push_back(*it_this + *it_other);
		} 
		
		c = std::move(newc);
		
		return *this;
	}
	
	poly operator-() const {
		poly ret(*this);
		for (auto &a : ret.c) {
			a = -a;
		}
		return ret;
	}
	
	poly operator-(const poly &other) const {
		return *this + -other;
	}
	
	poly &operator-=(const poly &other) {
		return *this = *this + -other;
	}
	
	poly operator*(const T &scalar) const {
		poly ret(*this);
		for (auto &a: ret.c) a = a * scalar;
		return ret;
	}
	
	poly &operator*=(const T &scalar) {
		for(auto &a: c) a = a * scalar;
		return *this;
	}
	
	poly operator*(const poly &other) const {
		//a b c d
		//A B C
		
		//Outer product table:
		/* Aa Ab Ac Ad
		 * Ba Bb Bc Bd
		 * Ca Cb Cc Cd
		 * */
		 
		poly ret;
		int len = c.size();
		int otherlen = other.c.size();
		//Hopefully type T has reasonable default constructors
		ret.c.resize(len + otherlen - 1);
		
		int i = 0, j = 0;
		for (auto u : c) {
			for (auto v: other.c) {
				ret.c[i+j] = ret.c[i+j] + u*v; //Don't want to force people to write operator+=
				j++;
			}
			i++;
			j = 0;
		}
		
		return ret;
	}
	
	poly &operator*=(const poly &other) {
		//a b c d
		//A B C
		
		//Outer product table:
		/* Aa Ab Ac Ad
		 * Ba Bb Bc Bd
		 * Ca Cb Cc Cd
		 * */
		std::vector<T> newc;
		int len = c.size();
		int otherlen = other.c.size();
		//Hopefully type T has reasonable default constructors
		newc.resize(len + otherlen - 1);
		
		int i = 0, j = 0;
		for (auto u : c) {
			for (auto v: other.c) {
				newc[i+j] = newc[i+j] + u*v;
				j++;
			}
			i++;
			j = 0;
		}
		
		c = std::move(newc);
		return *this;
	}
	
	//TODO: This doesn't check that shift is a reasonable quantity
	//(although I think vector will do something reasonable)
	poly operator<<(int shift) const {
		poly ret(*this);
		ret.c.resize(c.size() + shift);
		return ret;
	}
	
	poly &operator<<=(int shift) {
		c.resize(c.size() + shift);
		return *this;
	}
	
	//"integer" divison
	poly operator/(const poly &other) const {
		poly ret;
		int len = c.size();
		int otherlen = other.c.size();
		if (otherlen > len) return ret;
		ret.c.reserve(len - otherlen + 1);
		
		//   a b c d e f g
		// ÷       D E F G
		
		int offset = len - otherlen;
		poly divd(*this); //dividend
		poly divr = other << offset; //divisor
		
		auto leadCoeff = divr.c[0]; //TODO: check for divide by zero
		
		for(int i = 0; i <= offset; i++) {
			T tmp = divd.c[i] / leadCoeff;
			divd -= divr*tmp; //The op-assign operators really shouldn't be copying stuff... inefficient and bug-heavy
			ret.c.push_back(tmp);
			divr <<= -1;
		}
		
		return ret;
	}
	
	//TODO: implement this
	poly &operator/=(const poly &other);
	
	//TODO: is RVO still gonna happen?
	poly operator%(const poly &other) const {
		poly divd(*this);
		int len = c.size();
		int otherlen = other.c.size();
		if (otherlen > len) return divd;
		
		//   a b c d e f g
		// ÷       D E F G
		
		int offset = len - otherlen;
		poly divr = other << offset; //divisor
		
		auto leadCoeff = divr.c[0]; //TODO: check for divide by zero
		
		for(int i = 0; i <= offset; i++) {
			T tmp = divd.c[i] / leadCoeff;
			divd -= divr*tmp; //The op-assign operators really shouldn't be copying stuff... inefficient and bug-heavy
			divr <<= -1;
		}
		
		divd.c = std::vector<T>(divd.c.begin() + offset + 1, divd.c.end());
		
		return divd;
	}
	
	//TODO: implement this
	poly &operator%=(const poly &other);
	
	///Pretty-printing
	operator std::string() const {
		std::stringstream s;
		s << "{";
		auto it = c.begin();
		s << *it;
		++it;
		for (; it != c.end(); ++it) s << "," << *it;
		s << "}";
		return s.str();
	}
};

template <typename T> //Deduced at compile-time
std::ostream &operator<<(std::ostream &o, const poly<T> &p) {
	o << std::string(p);
	return o;
}

#endif
