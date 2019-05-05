#include <iostream>
#include <angle.hpp>
#include <vector.hpp>
#include <cstdlib>
#include <algorithm>


template <typename _FwdIter1, typename _FwdIter2>
void near_zero(_FwdIter1 first1, _FwdIter1 last1, _FwdIter2 first2,
	typename std::iterator_traits<_FwdIter1>::value_type e = std::numeric_limits<typename std::iterator_traits<_FwdIter1>::value_type>::epsilon()) {
		typedef typename std::iterator_traits<_FwdIter1>::value_type value_type;
		for (; first1 != last1; ++first1, ++first2) {
			*first2 = *first1;
			if (std::abs(*first2) < e)
				*first2 = static_cast<value_type>(0);
		}
	}


// template <typename... _Ts, typename _R>
// using enable_if_floating_point = typename std::enable_if<std::is_floating_point<_Ts>::value &&..., _R>::type;

template <typename _T1, typename _T2>
typename std::enable_if<
	std::is_floating_point<typename std::common_type<_T1, _T2>::type>::value,
	bool>::type near_equal(_T1 const& a, _T2 const& b, typename std::common_type<_T1, _T2>::type e =
		std::numeric_limits<typename std::common_type<_T1, _T2>::type>::epsilon()) {

			return !(e < std::abs(a - b));
		}

template <typename _FwdIter1, typename _FwdIter2>
typename std::enable_if<std::is_floating_point<typename std::iterator_traits<_FwdIter1>::value_type>::value, bool>::type
	near_equal(_FwdIter1 first1, _FwdIter1 last1, _FwdIter2 first2,
	typename std::iterator_traits<_FwdIter1>::value_type e = std::numeric_limits<typename std::iterator_traits<_FwdIter1>::value_type>::epsilon()) {

		for (; first1 != last1; ++first1, ++first2)
			if (!near_equal(*first1, *first2, e)) return false;
		return true;
	}

template <typename _Cont1, typename _Cont2>
typename std::enable_if<std::is_floating_point<typename _Cont1::value_type>::value, bool>::type
	near_equal(_Cont1 const& a, _Cont2 const& b,
	typename _Cont1::value_type e = std::numeric_limits<typename _Cont1::value_type>::epsilon()) {

		return near_equal(std::begin(a), std::end(a), std::begin(b), e);
	}

namespace math {
	template <typename _T, std::size_t _N>
	vector<_T, _N> abs(vector<_T, _N> const& v) {

		vector<_T, _N> result;
		for (auto first = result.begin(); first != result.end(); ++first)
			*first = std::abs(*first);
		return result;
	}
}

namespace math {

	template <typename _T>
	_T clamp(_T const& a, _T const& min, _T const& max) {
		return std::min(max, std::max(min, a));
	}
}

int main(int, char**) {

	math::vector2<float> u { math::deg(45) };
	math::vector2<float> v { math::deg(0) };

	std::cout << u << " : " << u.length() << std::endl;
	std::cout << v << " : " << v.length() << std::endl;

	math::vector2<float> p = math::projection(u, v);
	math::vector2<float> r = math::reflection(u, v);

	std::cout << p << " : " << p.length() << std::endl;
	std::cout << r << " : " << r.length() << std::endl;


	return 0;
}