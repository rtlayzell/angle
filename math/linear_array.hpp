#ifndef _MATH_LINEAR_ARRAY_HPP
#define _MATH_LINEAR_ARRAY_HPP

#include <vector.hpp>

namespace math {

	namespace internal {

		template <typename _VectorT>
		struct vector_base {

			template <typename _VectorT2> _VectorT& operator += (vector_base<_VectorT2> const& other);
			template <typename _VectorT2> _VectorT& operator -= (vector_base<_VectorT2> const& other);
			template <typename _VectorT2> _VectorT& operator *= (typename vector_traits<_VectorT>::value_type const& other);
			template <typename _VectorT2> _VectorT& operator /= (typename vector_traits<_VectorT>::value_type const& other);
		};
	}

	//////////////////////////////////////////////////////////////////////
	// forward declarations.

	template <typename _T>
	struct vector_traits;

	template <typename _T, std::size_t _N>
	struct vector;

	template <typename _T, std::size_t _N>
	struct vector_view;

	//////////////////////////////////////////////////////////////////////
	// vector_traits specializations.

	template <typename _T, std::size_t _N>
	struct vector_traits<vector<_T, _N>> {
		typedef _T value_type;
		typedef _T* pointer;
		typedef _T& reference;
		typedef std::size_t size_type;

		static const std::size_t dimensions = _N;
	};

	template <typename _T, std::size_t _N>
	struct vector_traits<vector_view<_T, _N>> {
		typedef _T value_type;
		typedef _T* pointer;
		typedef _T& reference;
		typedef std::size_t size_type;

		static const std::size_t dimensions = _N;
	};

	//////////////////////////////////////////////////////////////////////
	// vector implementation.

	template <typename _T, std::size_t _N>
	struct vector : vector_base<vector<_T, _N>> final {
		typedef vector_traits<vector<_T, _N>> traits_type;
		typedef typename traits_type::value_type value_type;
		typedef typename traits_type::pointer pointer;
		typedef typename traits_type::reference reference;
		typedef typename traits_type::size_type size_type;
	};

	//////////////////////////////////////////////////////////////////////
	// vector_view implementation.

	template <typename _T, std::size_t _N>
	struct vector_view : vector_base<vector_view<_T, _N>> final {
		typedef vector_traits<vector_view<_T, _N>> traits_type;
		typedef typename traits_type::value_type value_type;
		typedef typename traits_type::pointer pointer;
		typedef typename traits_type::reference reference;
		typedef typename traits_type::size_type size_type;
	};

	//////////////////////////////////////////////////////////////////////
	// vector type definitions.

	template <typename _T> using vector2 = vector<_T, 2>;
	template <typename _T> using vector3 = vector<_T, 3>;
	template <typename _T> using vector4 = vector<_T, 4>;

	template <typename _T> using vector2_view = vector_view<_T, 2>;
	template <typename _T> using vector3_view = vector_view<_T, 3>;
	template <typename _T> using vector4_view = vector_view<_T, 4>;
}

#endif // _MATH_LINEAR_ARRAY_HPP
