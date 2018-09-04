#ifndef GF_HPP
#define GF_HPP 1

#include <iostream>
#include <sstream>

template <typename T>
class TD;

template <typename T> 
class GF {
	T val;
	T mod;
	
	public:
	///Constructors, destructors, etc.
	//Uses perfect forwarding?
	template <typename P, typename Q>
	GF(P &&val_in, Q &&mod_in) : val(std::forward<P>(val_in)), mod(std::forward<Q>(mod_in)) {
		//TD<decltype(val_in)> tval_in;
		//TD<decltype(mod_in)> tmod_in;
		//std::cout << "val = " << val << std::endl;
		//std::cout << "mod = " << mod << std::endl;
	}
	
	///Arithmetic
	//TODO: Is it worth doing a template specialization for int, where
	//there is a risk of overflow?
	GF operator+(const GF &other) const {
		T newval = (val + other.val) % mod;
		return GF(move(newval), mod);
	}
	
	GF &operator+=(const GF &other) {
		val = (val + other.val) % mod;
		return *this;
	}
	
	GF operator+(const T &other) const {
		T newval = (val + other) % mod;
		return GF(move(newval), mod);
	}
	
	GF &operator+=(const T &other) {
		val = (val + other) % mod;
		return *this;
	}
	
	GF operator-() const {
		T newval = mod + -val;
		return GF(std::move(newval), mod);
	}
	
	GF operator*(const GF &other) const {
		T newval = (val * other.val) % mod;
		return GF(std::move(newval), mod);
	}
	
	GF &operator*=(const GF &other) {
		val = (val * other.val) % mod;
		return *this;
	}
	
	GF operator*(const T &other) const {
		T newval = (val * other) % mod;
		return GF(std::move(newval), mod);
	}
	
	GF &operator*=(const T &other) {
		val = (val * other) % mod;
		return *this;
	}
	
	///Pretty-printing
	operator std::string() const {
		std::stringstream s;
		s << "[" << val << "|" << mod << "]";
		return s.str();
	}
};

template <typename T> //Deduced at compile-time
std::ostream &operator<<(std::ostream &o, const GF<T> &out) {
	o << std::string(out);
	return o;
}

#endif
