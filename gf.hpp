#ifndef GF_HPP
#define GF_HPP 1

#include <iostream>
#include <sstream>
#include <stdexcept>

template <typename T> //For getting the compiler to tell you a type
class TD;

template <typename T> 
class GF {
	T val;
	T mod;
	
	public:	
	T zero; //additive identity
	T one; //multiplicative identity
	//It's a shame to copy these into evey element... maybe shared
	//pointers would be better?
	///Constructors, destructors, etc.
	GF() {} //Need to put it even though it doesn't do anything
	
	//Uses perfect forwarding?
	template <typename P, typename Q, typename R, typename S>
	GF(P &&val_in, Q &&mod_in, R &&zero_in, S &&one_in) : 
		val(std::forward<P>(val_in)), 
		mod(std::forward<Q>(mod_in)),
		zero(std::forward<R>(zero_in)), 
		one(std::forward<S>(one_in))  {
		//TD<decltype(val_in)> tval_in;
		//TD<decltype(mod_in)> tmod_in;
		//TD<decltype(zero_in)> tzero_in;
		//TD<decltype(one_in)> tone_in;
		//std::cout << "val = " << val << std::endl;
		//std::cout << "mod = " << mod << std::endl;
	}
	
	///Arithmetic
	//TODO: Is it worth doing a template specialization for int, where
	//there is a risk of overflow?
	GF operator+(const GF &other) const {
		T newval = (val + other.val) % mod;
		return GF(std::move(newval), mod, zero, one);
	}
	
	GF &operator+=(const GF &other) {
		val = (val + other.val) % mod;
		return *this;
	}
	
	GF operator+(const T &other) const {
		T newval = (val + other) % mod;
		return GF(std::move(newval), mod, zero, one);
	}
	
	GF &operator+=(const T &other) {
		val = (val + other) % mod;
		return *this;
	}
	
	GF operator-() const {
		T newval = mod + -val;
		return GF(std::move(newval), mod, zero, one);
	}
	
	GF operator*(const GF &other) const {
		T newval = (val * other.val) % mod;
		GF ret(std::move(newval), mod, zero, one);
		return ret;
	}
	
	GF &operator*=(const GF &other) {
		val = (val * other.val) % mod;
		return *this;
	}
	
	GF operator*(int zero) const {
		if (zero) {
			throw std::logic_error("Cannot multiply by arbitrary ints");
		}
		return GF(zero,mod,zero,one);
	}
	
	GF operator*(const T &other) const {
		T newval = (val * other) % mod;
		return GF(std::move(newval), mod, zero, one);
	}
	
	GF &operator*=(const T &other) {
		val = (val * other) % mod;
		return *this;
	}
	
	//This is honest-to-God division, unlike the "integer divison"
	//in the poly class. Hmmmmm is that a problem?
	GF operator/(const GF &other) const {
		//We need to find multiplicative inverse of other.
		//I think we need to use Euclidean algorithm here.
		T a[2] = {zero, one};
		//T b[2] = {zero, one};
		T r[2] {mod, other.val};
		T nexta;
		do {
			//std::cout << "r = " << r[0] << ", " << r[1] << " and ";
			//std::cout << "a= " << a[0] << ", " << a[1] << std::endl;
			T q = r[0]/r[1];
			T nextr = r[0]%r[1];
			nexta = a[0] + -q*a[1];
			//T nextb = b[0] + -q*b[1];
			r[0] = r[1];
			r[1] = nextr;
			a[0] = a[1];
			a[1] = nexta;
			//b[0] = b[1];
			//b[1] = nextb;
		} while(r[1] != zero);
		//Inverse is stored in a[0]
		T inverse = (a[0]+mod)%mod;
		return GF((val * inverse)%mod, mod, zero, one);
	}
	
	///Relational operators
	bool operator==(const GF &other) const {
		return val == other.val;
	}
	
	bool operator!=(const GF &other) const {
		return val != other.val;
	}
	
	bool operator==(const T&other) const {
		return val == other;
	}
	
	bool operator!=(const T&other) const {
		return val != other;
	}
	
	///Pretty-printing
	operator std::string() const {
		std::stringstream s;
		s << "[" << val << "|" << mod << "]";
		return s.str();
	}
	
	std::string compact(std::string dummy) const {
		std::stringstream s;
		s << val;
		return s.str();
	}
};

template <typename T> //Deduced at compile-time
std::ostream &operator<<(std::ostream &o, const GF<T> &out) {
	o << out.compact(" ");
	return o;
}

#endif
