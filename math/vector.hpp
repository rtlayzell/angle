#ifndef _MATH_VECTOR_HPP
#define _MATH_VECTOR_HPP

#include <cmath>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <iosfwd>

#include "angle.hpp"

namespace math {

	namespace internal {
		template <typename _T, std::size_t _N>
		struct vector_base {
		private:
			_T _data[_N];

		public:

			////////////////////////////////////////////////////////////////////////////////
			// subscript operators.

			_T& operator [](std::size_t index) noexcept { return _data[index]; }
			_T const& operator [](std::size_t index) const noexcept { return _data[index]; }
		};

		template <typename _T>
		struct vector_base<_T, 2> {
			_T x, y;

			////////////////////////////////////////////////////////////////////////////////
			// constructors.

			constexpr vector_base() = default;

			vector_base(_T x, _T y)
				: x(x), y(y) {}

			vector_base(radians<_T> const& theta, _T radius = static_cast<_T>(1))
				: x(radius * std::cos(theta.value()))
				, y(radius * std::sin(theta.value())) {}

			////////////////////////////////////////////////////////////////////////////////
			// subscript operators.

			_T& operator [](std::size_t index) noexcept {
				return *(reinterpret_cast<_T*>(this) + index);
			}

			_T const& operator [](std::size_t index) const noexcept {
				return *(reinterpret_cast<_T const*>(this) + index);
			}
		};

		template <typename _T>
		struct vector_base<_T, 3> {
			_T x, y, z;

			////////////////////////////////////////////////////////////////////////////////
			// constructors.

			constexpr vector_base() = default;

			vector_base(_T x, _T y, _T z = static_cast<_T>(1))
				: x(x), y(y), z(z) {}

			vector_base(radians<_T> const& theta, radians<_T> const& phi, _T radius = static_cast<_T>(1))
				: x(radius * std::sin(theta.value()) * std::cos(phi.value()))
				, y(radius * std::sin(theta.value()) * std::sin(phi.value()))
				, z(radius * std::cos(theta.value())) {}

			////////////////////////////////////////////////////////////////////////////////
			// subscript operators.

			_T& operator [](std::size_t index) noexcept {
				return *(reinterpret_cast<_T*>(this) + index);
			}

			_T const& operator [](std::size_t index) const noexcept {
				return *(reinterpret_cast<_T const*>(this) + index);
			}
		};

		template <typename _T>
		struct vector_base<_T, 4> {
			_T x, y, z, w;

			////////////////////////////////////////////////////////////////////////////////
			// constructors.

			constexpr vector_base() = default;

			vector_base(_T x, _T y, _T z, _T w = static_cast<_T>(1))
				: x(x), y(y), z(z), w(w) {}

			////////////////////////////////////////////////////////////////////////////////
			// subscript operators.

			_T& operator [](std::size_t index) noexcept {
				return *(reinterpret_cast<_T*>(this) + index);
			}

			_T const& operator [](std::size_t index) const noexcept {
				return *(reinterpret_cast<_T const*>(this) + index);
			}
		};
	}

	template <typename _T, std::size_t _N>
	struct vector : public internal::vector_base<_T, _N> {
		static_assert(std::is_arithmetic<_T>::value,
			"vector<T, N> requires arithmetic type.");

		////////////////////////////////////////////////////////////////////////////////
		// type definitions.

		typedef _T value_type;
		typedef _T* pointer;
		typedef _T& reference;
		typedef _T const* const_pointer;
		typedef _T const& const_reference;
		typedef std::size_t size_type;

		typedef _T* iterator;
		typedef _T const* const_iterator;
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		////////////////////////////////////////////////////////////////////////////////
		// constructors.

		using internal::vector_base<_T, _N>::vector_base;

		////////////////////////////////////////////////////////////////////////////////
		// unary arithmetic operators.

		vector operator +() const noexcept {
			return *this;
		}

		vector operator -() const noexcept {
			return -1 * (*this);
		}

		////////////////////////////////////////////////////////////////////////////////
		// compound arithmetic operators.

		vector& operator += (vector const& other) noexcept {
			std::transform(this->begin(), this->end(), other.begin(),
				this->begin(), std::plus<_T>());
			return *this;
		}

		vector& operator -= (vector const& other) noexcept {
			std::transform(this->begin(), this->end(), other.begin(),
				this->begin(), std::minus<_T>());
			return *this;
		}

		vector& operator *= (value_type scalar) noexcept {
			std::transform(this->begin(), this->end(), this->begin(),
				std::bind(std::multiplies<_T>(), std::placeholders::_1, scalar));
			return *this;
		}

		vector& operator /= (value_type scalar) noexcept {
			std::transform(this->begin(), this->end(), this->begin(),
				std::bind(std::divides<_T>(), std::placeholders::_1, scalar));
			return *this;
		}

		////////////////////////////////////////////////////////////////////////////////
		// methods.

		typename std::common_type<_T, float>::type length() const noexcept{
			return std::sqrt(this->length_sqr());
		}

		typename std::common_type<_T, float>::type length_sqr() const noexcept {
			return dot_product(*this, *this);
		}

		////////////////////////////////////////////////////////////////////////////////
		// iteration.

		iterator begin() noexcept { return std::addressof((*this)[0]); }
		iterator end() noexcept { return std::addressof((*this)[_N]); }

		const_iterator begin() const noexcept { return std::addressof((*this)[0]); }
		const_iterator end() const noexcept { return std::addressof((*this)[_N]); }

		const_iterator cbegin() const noexcept { return std::addressof((*this)[0]); }
		const_iterator cend() const noexcept { return std::addressof((*this)[_N]); }

		reverse_iterator rbegin() noexcept { return this->end(); }
		reverse_iterator rend() noexcept { return this->begin(); }

		const_reverse_iterator rbegin() const noexcept { return this->end(); }
		const_reverse_iterator rend() const noexcept { return this->begin(); }

		const_reverse_iterator crbegin() const noexcept { return this->cend(); }
		const_reverse_iterator crend() const noexcept { return this->cbegin(); }

		////////////////////////////////////////////////////////////////////////////////
		// conversion operators.

		template <typename _T2>
		operator vector<_T2, _N>() const noexcept {
			vector<_T2, _N> result;
			std::copy(this->begin(), this->end(), result.begin());
			return result;
		}

		template <typename _T2>
		operator vector<_T2, _N + 1>() const noexcept {
			vector<_T2, _N + 1> result;
			std::copy(this->begin(), this->end(), result.begin());
			*(result.end() - 1) = static_cast<_T2>(1);
			return result;
		}
	};

	template <typename _T> using vector2 = vector<_T, 2>;
	template <typename _T> using vector3 = vector<_T, 3>;
	template <typename _T> using vector4 = vector<_T, 4>;


	////////////////////////////////////////////////////////////////////////////////
	// equality operators.

	template <typename _T1, typename _T2, std::size_t _N>
	bool operator == (vector<_T1, _N> const& lhs, vector<_T2, _N> const& rhs) {
		auto first1 = lhs.begin();
		auto first2 = rhs.begin();
		auto last1 = lhs.end();

		for (; first1 != last1; ++first1, ++first2)
			if (!(*first1 == *first2)) return false;
		return true;
	}

	template <typename _T1, typename _T2, std::size_t _N>
	bool operator != (vector<_T1, _N> const& lhs, vector<_T2, _N> const& rhs) {
		return !(lhs == rhs);
	}

	////////////////////////////////////////////////////////////////////////////////
	// binary arithmetic operators.

	template <typename _T1, typename _T2, std::size_t _N>
	inline vector<typename std::common_type<_T1, _T2>::type, _N> operator + (vector<_T1, _N> const& lhs, vector<_T2, _N> const& rhs) {
		typedef typename std::common_type<_T1, _T2>::type common_t;
		return vector<common_t, _N>(lhs) += rhs;
	}

	template <typename _T1, typename _T2, std::size_t _N>
	inline vector<typename std::common_type<_T1, _T2>::type, _N> operator - (vector<_T1, _N> const& lhs, vector<_T2, _N> const& rhs) {
		typedef typename std::common_type<_T1, _T2>::type common_t;
		return vector<common_t, _N>(lhs) -= rhs;
	}

	template <typename _T1, typename _T2, std::size_t _N>
	inline vector<typename std::common_type<_T1, _T2>::type, _N> operator * (vector<_T1, _N> const& vec, _T2 scalar) {
		typedef typename std::common_type<_T1, _T2>::type common_t;
		return vector<common_t, _N>(vec) *= scalar;
	}

	template <typename _T1, typename _T2, std::size_t _N>
	inline vector<typename std::common_type<_T1, _T2>::type, _N> operator * (_T1 scalar, vector<_T2, _N> const& vec) {
		typedef typename std::common_type<_T1, _T2>::type common_t;
		return vector<common_t, _N>(vec) *= scalar;
	}

	template <typename _T1, typename _T2, std::size_t _N>
	inline vector<typename std::common_type<_T1, _T2>::type, _N> operator / (vector<_T1, _N> const& vec, _T2 scalar) {
		typedef typename std::common_type<_T1, _T2>::type common_t;
		return vector<common_t, _N>(vec) /= scalar;
	}

	////////////////////////////////////////////////////////////////////////////////
	// functions.

	template <typename _T1, typename _T2>
	vector<typename std::common_type<_T1, _T2>::type, 3> cross_product(vector<_T1, 3> const& lhs, vector<_T2, 3> const& rhs) {
		typedef typename std::common_type<_T1, _T2>::type common_t;
		return vector<common_t, 3> {
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x };
	}

	template <typename _T1, typename _T2, std::size_t _N>
	typename std::common_type<_T1, _T2, float>::type dot_product(vector<_T1, _N> const& lhs, vector<_T2, _N> const& rhs) {
		typedef typename std::common_type<_T1, _T2, float>::type common_t;
		return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), static_cast<common_t>(0));
	}

 	template <typename _T1, typename _T2, std::size_t _N>
 	vector<typename std::common_type<_T1, _T2, float>::type, _N> projection(vector<_T1, _N> const& vec, vector<_T2, _N> const& n) {
		return dot_product(vec, n) / dot_product(n, n) * n;
	}

	template <typename _T1, typename _T2, std::size_t _N>
 	vector<typename std::common_type<_T1, _T2, float>::type, _N> reflection(vector<_T1, _N> const& vec, vector<_T2, _N> const& n) {
		typedef typename std::common_type<_T1, _T2, float>::type common_t;
		return vec - static_cast<common_t>(2) * projection(vec, n);
	}

	template <typename _T1, typename _T2, std::size_t _N>
	radians<typename std::common_type<_T1, _T2, float>::type> inner_angle(vector<_T1, _N> const& lhs, vector<_T2, _N> const& rhs) {
		return radians<typename std::common_type<_T1, _T2, float>::type> { std::acos(dot_product(lhs, rhs) / (lhs.length() * rhs.length())) };
	}

	template <typename _T1, typename _T2, std::size_t _N>
	radians<typename std::common_type<_T1, _T2, float>::type> outer_angle(vector<_T1, _N> const& lhs, vector<_T2, _N> const& rhs) {
		return math::pi * 2 - inner_angle(lhs, rhs);
	}


	template <typename _T, std::size_t _N> vector<_T, _N> const& min(vector<_T, _N> const& vec) { return vec; }
	template <typename _T, std::size_t _N> vector<_T, _N> const& max(vector<_T, _N> const& vec) { return vec; }

	template <typename _T1, typename _T2, std::size_t _N>
	vector<typename std::common_type<_T1, _T2>::type, _N> min(
		vector<_T1, _N> const& lhs,
		vector<_T2, _N> const& rhs) {
			vector<typename std::common_type<_T1, _T2>::type, _N> result;
			std::transform(lhs.begin(), lhs.end(), rhs.begin(), result.begin(),
				[](_T1 const& a, _T2 const& b) { return std::min(a, b); });
			return result;
		}

	template <typename _T1, typename _T2, std::size_t _N>
	vector<typename std::common_type<_T1, _T2>::type, _N> max(
		vector<_T1, _N> const& lhs,
		vector<_T2, _N> const& rhs) {
			vector<typename std::common_type<_T1, _T2>::type, _N> result;
			std::transform(lhs.begin(), lhs.end(), rhs.begin(), result.begin(),
				[](_T1 const& a, _T2 const& b) { return std::max(a, b); });
			return result;
		}

	template <typename _T1, typename _T2, typename... _Ts, std::size_t _N>
	vector<typename std::common_type<_T1, _T2, _Ts...>::type, _N> min(
		vector<_T1, _N> const& v1,
		vector<_T2, _N> const& v2,
		vector<_Ts, _N> const&... vs) {
			return min(min(v1, v2), vs...);
		}

	template <typename _T1, typename _T2, typename... _Ts, std::size_t _N>
	vector<typename std::common_type<_T1, _T2, _Ts...>::type, _N> max(
		vector<_T1, _N> const& v1,
		vector<_T2, _N> const& v2,
		vector<_Ts, _N> const&... vs) {
			return max(max(v1, v2), vs...);
		}

	////////////////////////////////////////////////////////////////////////////////
	// streaming operators.

	template <typename _Elem, typename _Traits, typename _T, std::size_t _N>
	std::basic_ostream<_Elem, _Traits>& operator << (std::basic_ostream<_Elem, _Traits>& os, vector<_T, _N> const& vec) {
		std::copy(vec.begin(), vec.end(), std::ostream_iterator<_T, _Elem, _Traits>(os, ", "));
		return os << "\b\b \b";
	}
}


#endif  // _MATH_VECTOR_HPP