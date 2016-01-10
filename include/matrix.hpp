#ifndef _MATH_MATRIX_HPP
#define _MATH_MATRIX_HPP

#include <angle.hpp>
#include <vector.hpp>
#include <quaternion.hpp>
#include <cmath>

namespace math {

	template <typename _T, std::size_t _M, std::size_t _N>
	struct matrix;

	template <typename _T, std::size_t _N>
	struct linear_array {

		linear_array operator -() const noexcept;
		linear_array operator +() const noexcept;

		linear_array& operator += (linear_array const& other);
		linear_array& operator += (vector<_T, _N> const& other);
		linear_array& operator -= (linear_array const& other);
		linear_array& operator -= (vector<_T, _N> const& other);
		linear_array& operator *= (_T const& scalar);
		linear_array& operator /= (_T const& scalar);

	};

	namespace internal {
		template <typename _MatrixT> struct matrix_identity {};
		template <typename _MatrixT> struct matrix_scaling {};
		template <typename _MatrixT> struct matrix_rotation {};
		template <typename _MatrixT> struct matrix_translation {};

		template <typename _T, std::size_t _N>
		struct matrix_identity<matrix<_T, _N, _N>> {
			static const matrix<_T, _N, _N> identity;
		};

		////////////////////////////////////////////////////////////////////////
		// 2-dimensional matrix transforms.

		template <typename _T>
		struct matrix_scaling<matrix<_T, 3, 3>> {
			void scale(vector2<_T> const& scale) noexcept;

			vector2<_T> scale() const noexcept;
		}

		template <typename _T>
		struct matrix_translation<matrix<_T, 3, 3>> {
			void translate(vector2<_T> const& translation) noexcept;
			void translation(vector2<_T> const& translation) noexcept;

			void translate(_T const& x, _T const& y) noexcept;
			void translation(_T const& x, _T const& y) noexcept;

			vector2<_T> translation() const noexcept;
		};

		template <typename _T>
		struct matrix_rotation<matrix<_T, 3, 3>> {
			void rotate(radians<_T> const& angle) noexcept;
			void rotation(radians<_T> const& angle) noexcept;

			radians<_T> rotation() const noexcept;
		};

		////////////////////////////////////////////////////////////////////////
		// 3-dimensional matrix transforms.

		template <typename _T>
		struct matrix_scaling<matrix<_T, 4, 4>> {
			void scale(vector3<_T> const& scale) noexcept;

			vector3<_T> scale() const noexcept;
		}

		template <typename _T>
		struct matrix_translation<matrix<_T, 4, 4>> {
			void translate(vector3<_T> const& translation) noexcept;
			void translation(vector3<_T> const& translation) noexcept {
				auto mat = reinterpret_cast<matrix<_T, 4, 4>*>(this);
			}

			vector3<_T> translation() const noexcept;
		};

		template <typename _T>
		struct matrix_rotation<matrix<_T, 4, 4>> {
			void rotate(radians<_T> const& angle) noexcept;
			void rotation(radians<_T> const& angle) noexcept;

			radians<_T> rotation() const noexcept;
		};
	}

	/// _T = type of values
	/// _M = number of columns
	/// _N = number of rows
	template <typename _T, std::size_t _M, std::size_t _N>
	struct matrix :
		public internal::matrix_identity<matrix<_T, _M, _N>>,
		public internal::matrix_scaling<matrix<_T, _M, _N>>,
		public internal::matrix_rotation<matrix<_T, _M, _N>>,
		public internal::matrix_translation<matrix<_T, _M, _N>> {

		static_assert(std::is_arithmetic<_T>::value,
			"matrix<T, M, N> requires arithmetic type.");

		typedef _T value_type;
		typedef _T* pointer;
		typedef _T& reference;
		typedef _T const* const_pointer;
		typedef _T const& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

		typedef _T* iterator;
		typedef _T const* const_iterator;

		linear_array<_T, _N> operator [](size_type index) noexcept;
		linear_array<_T, _N> const operator [](size_type index) const noexcept;

		matrix operator -() const noexcept;
		matrix operator +() const noexcept;

		matrix& operator += (matrix const& other) noexcept;
		matrix& operator -= (matrix const& other) noexcept;
		matrix& operator *= (matrix const& other) noexcept;
		matrix& operator *= (value_type const& other) noexcept;
		matrix& operator /= (value_type const& other) noexcept;


		typename std::common_type<_T, float>::type determinant() const noexcept;

		matrix inverse() const noexcept;
		matrix decompose() const noexcept;

		matrix upper_decompose() const noexcept;
		matrix lower_decompose() const noexcept;

		iterator begin() noexcept;
		iterator end() noexcept;
		const_iterator begin() const noexcept;
		const_iterator end() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;

	private:
		value_type _data[_M * _N];
	};

	template <typename _T> using matrix2x2 = matrix<_T, 2, 2>;
	template <typename _T> using matrix3x3 = matrix<_T, 3, 3>;
	template <typename _T> using matrix4x4 = matrix<_T, 4, 4>;
}

#endif // _MATH_MATRIX_HPP