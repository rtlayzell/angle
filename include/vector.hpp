#ifndef _MATH_VECTOR_HPP
#define _MATH_VECTOR_HPP

#include <cmath>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <type_traits>
#include <iosfwd>

namespace math {

	namespace internal {
		template <typename _T, std::size_t _N>
		struct vector_base {
		private:
			_T _data[_N];

		public:
			_T& operator [](std::size_t index) noexcept { return _data[index]; }
			_T const& operator [](std::size_t index) const noexcept { return _data[index]; }
		};

		template <typename _T>
		struct vector_base<_T, 2> {
			_T x, y;

			vector_base(_T x, _T y) : x(x), y(y) {}

			_T& operator [](std::size_t index) noexcept { return *(reinterpret_cast<_T*>(this) + index); }
			_T const& operator [](std::size_t index) const noexcept { return *(reinterpret_cast<_T const*>(this) + index); }
		};

		template <typename _T>
		struct vector_base<_T, 3> {
			_T x, y, z;

			vector_base(_T x, _T y, _T z = static_cast<_T>(1))
				: x(x), y(y), z(z) {}

			_T& operator [](std::size_t index) noexcept { return *(reinterpret_cast<_T*>(this) + index); }
			_T const& operator [](std::size_t index) const noexcept { return *(reinterpret_cast<_T const*>(this) + index); }
		};

		template <typename _T>
		struct vector_base<_T, 4> {
			_T x, y, z, w;

			vector_base(_T x, _T y, _T z, _T w = static_cast<_T>(1))
				: x(x), y(y), z(z), w(w) {}

			_T& operator [](std::size_t index) noexcept { return *(reinterpret_cast<_T*>(this) + index); }
			_T const& operator [](std::size_t index) const noexcept { return *(reinterpret_cast<_T const*>(this) + index); }
		};
	}

	template <typename _T, std::size_t _N>
	struct vector : public internal::vector_base<_T, _N> {
		static_assert(std::is_arithmetic<_T>::value, "vector<_T> requires arithmetic type.");

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
		// construction.

		using internal::vector_base<_T, _N>::vector_base;

		constexpr vector operator -() const noexcept { return *this; }
		constexpr vector operator +() const noexcept { return *this; }

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
	};

	template <typename _T> using vector2 = vector<_T, 2>;
	template <typename _T> using vector3 = vector<_T, 3>;
	template <typename _T> using vector4 = vector<_T, 4>;

	////////////////////////////////////////////////////////////////////////////////

	template <typename _T, typename _U, std::size_t _N>
	bool operator == (vector<_T, _N> const& lhs, vector<_U, _N> const& rhs);

	template <typename _T, typename _U, std::size_t _N>
	bool operator != (vector<_T, _N> const& lhs, vector<_U, _N> const& rhs);

	////////////////////////////////////////////////////////////////////////////////

	template <typename _T, typename _U, std::size_t _N>
	vector<typename std::common_type<_T, _U>::type, _N> operator + (vector<_T, _N> const& lhs, vector<_U, _N> const& rhs) {
		typedef typename std::common_type<_T, _U>::type common_t;
		return vector<common_t, _N>(lhs) += rhs;
	}

	template <typename _T, typename _U, std::size_t _N>
	vector<typename std::common_type<_T, _U>::type, _N> operator - (vector<_T, _N> const& lhs, vector<_U, _N> const& rhs) {
		typedef typename std::common_type<_T, _U>::type common_t;
		return vector<common_t, _N>(lhs) -= rhs;
	}

	////////////////////////////////////////////////////////////////////////////////

	template <typename _T, typename _U, std::size_t _N>
	typename std::common_type<_T, _U, float>::type dot_product(vector<_T, _N> const& lhs, vector<_U, _N> const& rhs) {
		typedef typename std::common_type<_T, _U, float>::type common_t;
		return std::inner_product(lhs.begin, lhs.end, rhs.begin(), static_cast<common_t>(0));
	}

 	template <typename _T, typename _U, std::size_t _N>
 	vector<typename std::common_type<_T, _U, float>::type, _N> projection(vector<_T, _N> const& vec, vector<_U, _N> const& n) {
		typedef typename std::common_type<_T, _U, float>::type common_t;
		return dot_product(vec, n) / dot_product(n, n) * n;
	}

	template <typename _T, typename _U, std::size_t _N>
 	vector<typename std::common_type<_T, _U, float>::type, _N> reflection(vector<_T, _N> const& vec, vector<_U, _N> const& n) {
		typedef typename std::common_type<_T, _U, float>::type common_t;
		return vec - static_cast<common_t>(2) * projection(vec, n);
	}

	////////////////////////////////////////////////////////////////////////////////

	template <typename _Elem, typename _Traits, typename _T, std::size_t _N>
	std::basic_ostream<_Elem, _Traits>& operator << (std::basic_ostream<_Elem, _Traits>& os, vector<_T, _N> const& vec) {
		std::copy(vec.begin(), vec.end(), std::ostream_iterator<_T, _Elem, _Traits>(os, " "));
		return os << '\b';
	}
}


#endif  // _MATH_VECTOR_HPP