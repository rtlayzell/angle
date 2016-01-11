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
		template <typename _MatrixT> struct matrix_transforms {};

		template <typename _T, std::size_t _N>
		struct matrix_identity<matrix<_T, _N, _N>> {
			static const matrix<_T, _N, _N> identity;
		};

		////////////////////////////////////////////////////////////////////////
		// 2-dimensional matrix transforms.

		template <typename _T>
		struct matrix_transforms<matrix<_T, 3, 3>> {

			////////////////////////////////////////////////////////////////////////
			// scaling.

			void scale(vector2<_T> const& scale) noexcept;

			vector2<_T> scale() const noexcept;

			////////////////////////////////////////////////////////////////////////
			// translation.

			void translate(vector2<_T> const& translation) noexcept;
			void translation(vector2<_T> const& translation) noexcept;

			void translate(_T const& x, _T const& y) noexcept;
			void translation(_T const& x, _T const& y) noexcept;

			vector2<_T> translation() const noexcept;

			////////////////////////////////////////////////////////////////////////
			// rotation.

			void rotate(radians<_T> const& angle) noexcept;
			void rotation(radians<_T> const& angle) noexcept;

			radians<_T> rotation() const noexcept;
		};

		////////////////////////////////////////////////////////////////////////
		// 3-dimensional matrix transforms.

		template <typename _T>
		struct matrix_transforms<matrix<_T, 4, 4>> {

			////////////////////////////////////////////////////////////////////////
			// scaling.

			void scale(vector3<_T> const& scale) noexcept;

			vector3<_T> scale() const noexcept;

			////////////////////////////////////////////////////////////////////////
			// translation.

			void translate(vector3<_T> const& translation) noexcept;
			void translation(vector3<_T> const& translation) noexcept;

			void translate(_T const& x, _T const& y, _T const& z) noexcept;
			void translation(_T const& x, _T const& y, _T const& z) noexcept;

			vector3<_T> translation() const noexcept;

			////////////////////////////////////////////////////////////////////////
			// rotation.

			void xrotate(radians<_T> const& angle) noexcept;
			void xrotation(radians<_T> const& angle) noexcept;

			void yrotate(radians<_T> const& angle) noexcept;
			void yrotation(radians<_T> const& angle) noexcept;

			void zrotate(radians<_T> const& angle) noexcept;
			void zrotation(radians<_T> const& angle) noexcept;

			radians<_T> xrotation() const noexcept;
			radians<_T> yrotation() const noexcept;
			radians<_T> zrotation() const noexcept;
		};
	}

	/// _T = type of values
	/// _M = number of columns
	/// _N = number of rows
	template <typename _T, std::size_t _M, std::size_t _N>
	struct matrix :
		public internal::matrix_identity<matrix<_T, _M, _N>>,
		public internal::matrix_transforms<matrix<_T, _M, _N>> {

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
		typedef std::reverse_iterator<iterator> reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		////////////////////////////////////////////////////////////////////////////////

		linear_array<_T, _N> operator [](size_type index) noexcept;
		linear_array<_T, _N> const operator [](size_type index) const noexcept;

		////////////////////////////////////////////////////////////////////////////////
		// unary arithmetic operators.

		matrix operator +() const noexcept {
			return *this;
		}

		matrix operator -() const noexcept {
			return -1 * (*this);
		}

		////////////////////////////////////////////////////////////////////////////////
		// compound arithmetic operators.

		matrix& operator += (matrix const& other) noexcept {
			std::transform(this->begin(), this->end(), other.begin(),
				this->begin(), std::plus<_T>());
			return *this;
		}

		matrix& operator -= (matrix const& other) noexcept {
			std::transform(this->begin(), this->end(), other.begin(),
				this->begin(), std::minus<_T>());
			return *this;
		}

		matrix& operator *= (matrix const& other) noexcept;

		matrix& operator *= (value_type const& scalar) noexcept {
			std::transform(this->begin(), this->end(), this->begin(),
				std::bind(std::multiplies<_T>(), std::placeholders::_1, scalar));
			return *this;
		}

		matrix& operator /= (value_type const& scalar) noexcept {
			std::transform(this->begin(), this->end(), this->begin(),
				std::bind(std::divides<_T>(), std::placeholders::_1, scalar));
			return *this;
		}


		typename std::common_type<_T, float>::type determinant() const noexcept;

		matrix inverse() const noexcept;
		matrix decompose() const noexcept;

		matrix upper_decompose() const noexcept;
		matrix lower_decompose() const noexcept;

		iterator begin() noexcept { return { &_data[0]; } }
		iterator end() noexcept { return { &_data[_M * _N]; } }
		const_iterator begin() const noexcept { return { &_data[0]; } }
		const_iterator end() const noexcept { return { &_data[_M * _N]; } }
		const_iterator cbegin() const noexcept { return { &_data[0]; } }
		const_iterator cend() const noexcept { return { &_data[_M * _N]; } }

		reverse_iterator rbegin() noexcept { return this->end(); }
		reverse_iterator rend() noexcept { return this->begin(); }
		const_reverse_iterator rbegin() const noexcept { return this->end(); }
		const_reverse_iterator rend() const noexcept { return this->begin(); }
		const_reverse_iterator crbegin() const noexcept { return this->cend(); }
		const_reverse_iterator crend() const noexcept { return this->cbegin(); }

	private:
		value_type _data[_M * _N];
	};

	template <typename _T> using matrix2x2 = matrix<_T, 2, 2>;
	template <typename _T> using matrix3x3 = matrix<_T, 3, 3>;
	template <typename _T> using matrix4x4 = matrix<_T, 4, 4>;
}

#endif // _MATH_MATRIX_HPP