#include <iostream>
#include <angle.hpp>

#include <cmath>
namespace math {
	using namespace ::angle;
	
	inline float cos(radians<float> angle) { return ::std::cos(angle.value()); }
	inline double cos(radians<double> angle) { return ::std::cos(angle.value()); }
	inline long double cos(radians<long double> angle) { return ::std::cos(angle.value()); }
}



int main(int, char**) {
	math::degrees<float> d { 90 };
	math::radians<float> r = 2 * d;
	
	std::cout << r << std::endl; 
	std::cout << ::std::cos(r.value()) << std::endl;
	
	
	return 0;
}