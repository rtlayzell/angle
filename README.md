## C++11 Math Template Library

The Math Template Library provides a set of common mathematical abstractions and functions common within the fields of computing and graphics.

### Abstractions

- Angle
- Vector
- Quaternion
- Matrix

### Functions

- Interpolation

#### Angle

```cpp
math::radians<float> r { 3.14159f };
math::degrees<int> d { 180 };
math::gradians<int> g { 200 };
math::revolutions<float> v { 0.0f };
```

Each angle type can be implicitly cast to any other angle type.

```cpp
void func(math::radians<float> r) {
	...
}

math::degrees<int> d { 180 };
func(d); // 'd' will be converted to radians<float> with 3.14159..
```