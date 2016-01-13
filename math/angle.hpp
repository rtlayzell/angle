#ifndef _MATH_ANGLE_HPP
#define _MATH_ANGLE_HPP

#include <cmath>
#include <iosfwd>
#include <type_traits>

namespace math {

	template <typename _T>
	struct radian_traits {
		static_assert(::std::is_floating_point<_T>::value,
			"radian_traits<_T> requires floating point type.");
		static constexpr _T pi() noexcept {
			return static_cast<_T>(3.14159265359);
		}
	};

	template <typename _T>
	struct degree_traits {
		static_assert(::std::is_arithmetic<_T>::value,
			"degree_traits<_T> requires arithmetic type.");
		static constexpr _T pi() noexcept {
			return static_cast<_T>(180);
		}
	};

	template <typename _T>
	struct gradian_traits {
		static_assert(::std::is_arithmetic<_T>::value,
			"gradian_traits<_T> requires arithmetic type.");
		static constexpr _T pi() noexcept {
			return static_cast<_T>(200);
		}
	};

	template <typename _T>
	struct revolution_traits {
		static_assert(::std::is_floating_point<_T>::value,
			"revolution_traits<_T> requires floating point type.");
		static constexpr _T pi() noexcept {
			return static_cast<_T>(0.5);
		}
	};

	template <typename _T, typename _Traits> struct basic_angle;

	template <typename _T> using radians = basic_angle<_T, radian_traits<_T>>;
	template <typename _T> using degrees = basic_angle<_T, degree_traits<_T>>;
	template <typename _T> using gradians = basic_angle<_T, gradian_traits<_T>>;
	template <typename _T> using revolutions = basic_angle<_T, revolution_traits<_T>>;


	template <
		typename _T,
		typename _Traits = radian_traits<_T>>
	struct basic_angle {

		//////////////////////////////////////////////////////////////////////////////
		// type definitions.

		typedef _Traits traits_type;
		typedef _T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef value_type const* const_pointer;
		typedef value_type const& const_reference;

		//////////////////////////////////////////////////////////////////////////////
		// construction.

		constexpr explicit basic_angle(value_type val = static_cast<_T>(0)) noexcept
			: _value { val } {}

		constexpr basic_angle(basic_angle&&) = default;
		constexpr basic_angle(basic_angle const&) = default;

		//////////////////////////////////////////////////////////////////////////////
		// assignment operators.

		basic_angle& operator = (basic_angle&&) = default;
		basic_angle& operator = (basic_angle const&) = default;

		//////////////////////////////////////////////////////////////////////////////
		// accessor methods.

		constexpr value_type value() const noexcept {
			return _value;
		}

		//////////////////////////////////////////////////////////////////////////////
		// compound arithmetic operators.

		constexpr basic_angle operator +() const noexcept {
			return basic_angle { _value };
		}

		constexpr basic_angle operator -() const noexcept {
			return basic_angle { -_value };
		}

		basic_angle& operator ++() noexcept {
			++_value;
			return *this;
		}

		basic_angle& operator --() noexcept {
			--_value;
			return *this;
		}

		basic_angle operator ++(int) noexcept {
			basic_angle tmp(*this); ++*this;
			return *this;
		}

		basic_angle operator --(int) noexcept {
			basic_angle tmp(*this); --*this;
			return *this;
		}

		basic_angle& operator += (basic_angle const& other) noexcept {
			_value += other._value;
			return *this;
		}

		basic_angle& operator -= (basic_angle const& other) noexcept {
			_value -= other._value;
			return *this;
		}

		basic_angle& operator *= (value_type val) noexcept {
			_value *= val;
			return *this;
		}

		basic_angle& operator /= (value_type val) noexcept {
			_value /= val;
			return *this;
		}

		//////////////////////////////////////////////////////////////////////////////
		// conversion operators.

		template <typename _Traits2>
		constexpr operator basic_angle<_T, _Traits2>() const noexcept {
			return _convert<_T, _Traits2>();
		}

		template <typename _T2, typename _Traits2>
		constexpr operator basic_angle<_T2, _Traits2>() const noexcept {
			return _convert<_T2, _Traits2>();
		}

		//////////////////////////////////////////////////////////////////////////////
		// comparison operators.



		//////////////////////////////////////////////////////////////////////////////
		// conversion operators.

		template <typename _CharT, typename _OsTraits>
		friend std::basic_ostream<_CharT, _OsTraits>& operator << (std::basic_ostream<_CharT, _OsTraits>& ostr, basic_angle const& angle) {
			return ostr << angle._value;
		}

		template <typename _CharT, typename _IsTraits>
		friend std::basic_istream<_CharT, _IsTraits>& operator << (std::basic_istream<_CharT, _IsTraits>& istr, basic_angle& angle) {
			return istr >> angle._value;
		}

	private:
		value_type _value;

		template <typename _T2, typename _Traits2>
		constexpr basic_angle<_T2, _Traits2> _convert() const noexcept {
			typedef typename std::common_type<_T, _T2>::type common_t;
			return basic_angle<_T2, _Traits2> { static_cast<common_t>(_value) * _Traits2::pi() / _Traits::pi()  };
		}
	};

	constexpr const radians<long double> pi = degrees<long double>(180);

	//////////////////////////////////////////////////////////////////////////////
	// binary arithmetic operators.

	template <typename _T1, typename _T2, typename _Traits1, typename _Traits2>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits1> operator + (
		basic_angle<_T1, _Traits1> const& lhs, basic_angle<_T2, _Traits2> const& rhs) {
			typedef typename std::common_type<_T1, _T2>::type common_t;
			return basic_angle<common_t, _Traits1>(lhs) += rhs;
		}

	template <typename _T1, typename _T2, typename _Traits>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits> operator + (
		basic_angle<_T1, _Traits> const& lhs, _T2 const& rhs) {
			typedef typename std::common_type<_T1, _T2>::type common_t;
			return basic_angle<common_t, _Traits>(lhs) += rhs;
		}

	template <typename _T1, typename _T2, typename _Traits>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits> operator + (
		_T1 const& lhs, basic_angle<_T2, _Traits> const& rhs) {
			typedef typename std::common_type<_T1, _T2>::type common_t;
			return basic_angle<common_t, _Traits>(lhs) += rhs;
		}


	template <typename _T1, typename _T2, typename _Traits1, typename _Traits2>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits1> operator - (
		basic_angle<_T1, _Traits1> const& lhs, basic_angle<_T2, _Traits2> const& rhs) {
			typedef typename std::common_type<_T1, _T2>::type common_t;
			return basic_angle<common_t, _Traits1>(lhs) -= rhs;
		}

	template <typename _T1, typename _T2, typename _Traits>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits> operator - (
		basic_angle<_T1, _Traits> const& lhs, _T2 const& rhs) {
			typedef typename std::common_type<_T1, _T2>::type common_t;
			return basic_angle<common_t, _Traits>(lhs) -= rhs;
		}

	template <typename _T1, typename _T2, typename _Traits>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits> operator - (
		_T1 const& lhs, basic_angle<_T2, _Traits> const& rhs) {
			typedef typename std::common_type<_T1, _T2>::type common_t;
			return basic_angle<common_t, _Traits>(lhs) -= rhs;
		}

	template <typename _T1, typename _T2, typename _Traits>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits> operator * (
		basic_angle<_T1, _Traits> const& lhs, _T2 const& rhs) {
			typedef typename std::common_type<_T1, _T2>::type common_t;
			return basic_angle<common_t, _Traits>(lhs) *= rhs;
		}

	template <typename _T1, typename _T2, typename _Traits>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits> operator * (
		_T1 const& lhs, basic_angle<_T2, _Traits> const& rhs) { return rhs * lhs; }

	template <typename _T1, typename _T2, typename _Traits>
	inline constexpr basic_angle<typename std::common_type<_T1, _T2>::type, _Traits> operator / (
		basic_angle<_T1, _Traits> const& lhs, _T2 const& rhs) {
			typedef typename std::common_type<_T1, _T2>::type common_t;
			return basic_angle<common_t, _Traits>(lhs) /= rhs;
		}

	//////////////////////////////////////////////////////////////////////////////
	// comparison operators.

	template <typename _T, typename _Traits>
	inline constexpr bool operator == (basic_angle<_T, _Traits> const& lhs, typename std::common_type<basic_angle<_T, _Traits>>::type const& rhs) noexcept {
		return lhs.value() == rhs.value();
	}

	template <typename _T, typename _Traits>
	inline constexpr bool operator < (basic_angle<_T, _Traits> const& lhs, typename std::common_type<basic_angle<_T, _Traits>>::type const& rhs) noexcept {
		return lhs.value() < rhs.value();
	}

	template <typename _T, typename _Traits>
	inline constexpr bool operator != (basic_angle<_T, _Traits> const& lhs, typename std::common_type<basic_angle<_T, _Traits>>::type const& rhs) {
		return !(lhs == rhs);
	}

	template <typename _T, typename _Traits>
	inline constexpr bool operator <= (basic_angle<_T, _Traits> const& lhs, typename std::common_type<basic_angle<_T, _Traits>>::type const& rhs) {
		return !(rhs < lhs);
	}

	template <typename _T, typename _Traits>
	inline constexpr bool operator >= (basic_angle<_T, _Traits> const& lhs, typename std::common_type<basic_angle<_T, _Traits>>::type const& rhs) {
		return !(lhs < rhs);
	}

	template <typename _T, typename _Traits>
	inline constexpr bool operator > (basic_angle<_T, _Traits> const& lhs, typename std::common_type<basic_angle<_T, _Traits>>::type const& rhs) {
		return rhs < lhs;
	}

	//////////////////////////////////////////////////////////////////////////////
	// helper functions.

	template <typename _T>
	constexpr radians<typename std::common_type<_T, float>::type> rad(_T const& angle) {
		typedef typename std::common_type<_T, float>::type common_t;
		return radians<common_t> { static_cast<common_t>(angle) };
	}

	template <typename _T>
	constexpr revolutions<typename std::common_type<_T, float>::type> revs(_T const& angle) {
		typedef typename std::common_type<_T, float>::type common_t;
		return revolutions<common_t> { static_cast<common_t>(angle) };
	}

	template <typename _T>
	constexpr degrees<_T> deg(_T const& angle) {
		return degrees<_T> { angle };
	}

	template <typename _T>
	constexpr gradians<_T> grad(_T const& angle) {
		return gradians<_T> { angle };
	}

	template <typename _T, typename _Traits>
	constexpr radians<typename std::common_type<_T, float>::type> rad(basic_angle<_T, _Traits> const& angle) {
		typedef typename std::common_type<_T, float>::type common_t;
		return radians<common_t> { angle };
	}

	template <typename _T, typename _Traits>
	constexpr revolutions<typename std::common_type<_T, float>::type> revs(basic_angle<_T, _Traits> const& angle) {
		typedef typename std::common_type<_T, float>::type common_t;
		return revolutions<common_t> { angle };
	}

	template <typename _T, typename _Traits>
	constexpr degrees<_T> deg(basic_angle<_T, _Traits> const& angle) {
		return degrees<_T> { angle };
	}

	template <typename _T, typename _Traits>
	constexpr gradians<_T> grad(basic_angle<_T, _Traits> const& angle) {
		return gradians<_T> { angle };
	}
}

namespace math {
	template <typename _T, typename _Traits>
	inline typename std::common_type<_T, float>::type sin(basic_angle<_T, _Traits> const& x) { return std::sin(math::rad(x).value()); }
	template <typename _T, typename _Traits>
	inline typename std::common_type<_T, float>::type cos(basic_angle<_T, _Traits> const& x) { return std::cos(math::rad(x).value()); }
	template <typename _T, typename _Traits>
	inline typename std::common_type<_T, float>::type tan(basic_angle<_T, _Traits> const& x) { return std::tan(math::rad(x).value()); }
}


#endif // _MATH_ANGLE_HPP