//
//  xyzVector.h
//  REDESIGNC
//
//  Created by Joseph Yesselman on 9/28/14.
//  Copyright (c) 2014 Joseph Yesselman. All rights reserved.
//

#ifndef __REDESIGNC__xyzVector__
#define __REDESIGNC__xyzVector__

#include <iostream>
// C++ headers
#include <sstream>
#include <cassert>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>

//RNAMake Headers
#include <base/string.h>
#include <base/json.h>

namespace math {

template<typename T>
class xyzVector {

private:
	template<typename> friend
	class xyzMatrix;

public:

	typedef T Value;

public: // Creation

	inline
	xyzVector():
			x_(0),
			y_(0),
			z_(0){}

	inline
	xyzVector(xyzVector const & v) :
			x_(v.x_),
			y_(v.y_),
			z_(v.z_) {}

	template<typename U>
	inline
	xyzVector(xyzVector<U> const & v) :
			x_(v.x_),
			y_(v.y_),
			z_(v.z_) {}

	inline
	xyzVector(Value const & x, Value const & y, Value const & z) :
			x_(x),
			y_(y),
			z_(z) {}

	inline
	xyzVector(std::vector<T> const & v) :
			x_(v[0]),
			y_(v[1]),
			z_(v[2]) {}

	inline
	explicit
	xyzVector(Value const & t) :
			x_(t),
			y_(t),
			z_(t) {}

	inline
	xyzVector(String const & s) {
		auto spl = base::split_str_by_delimiter(s, " ");
		assert(spl.size() >= 3);
		x_ = (Value) std::stod(spl[0]);
		y_ = (Value) std::stod(spl[1]);
		z_ = (Value) std::stod(spl[2]);
	}

	inline
	xyzVector(json::JSON & j) {
		x_ = j[0].ToFloat();
		y_ = j[1].ToFloat();
		z_ = j[2].ToFloat();
	}

public:

	inline
	String const
	get_str() const {
		return std::to_string(x_) + " " + std::to_string(y_) + " " +
			   std::to_string(z_);
	}

	inline
	json::JSON
	get_json() const {
		return json::Array(x_, y_, z_);
	}

public: // Assignment

	/// @brief Copy assignment
	inline
	xyzVector &
	operator=(xyzVector const & v) {
		if (this != &v) {
			x_ = v.x_;
			y_ = v.y_;
			z_ = v.z_;
		}
		return *this;
	}

	/// @brief Copy assignment
	template<typename U>
	inline
	xyzVector &
	operator=(xyzVector<U> const & v) {
		x_ = v.x_;
		y_ = v.y_;
		z_ = v.z_;
		return *this;
	}

	/// @brief += xyzVector
	template<typename U>
	inline
	xyzVector &
	operator+=(xyzVector<U> const & v) {
		x_ += v.x_;
		y_ += v.y_;
		z_ += v.z_;
		return *this;
	}


	/// @brief -= xyzVector
	template<typename U>
	inline
	xyzVector &
	operator-=(xyzVector<U> const & v) {
		x_ -= v.x_;
		y_ -= v.y_;
		z_ -= v.z_;
		return *this;
	}

	// @brief = Value
	inline
	xyzVector &
	operator=(Value const & t) {
		x_ = y_ = z_ = t;
		return *this;
	}


	/// @brief += Value
	inline
	xyzVector &
	operator+=(Value const & t) {
		x_ += t;
		y_ += t;
		z_ += t;
		return *this;
	}


	/// @brief -= Value
	inline
	xyzVector &
	operator-=(Value const & t) {
		x_ -= t;
		y_ -= t;
		z_ -= t;
		return *this;
	}


	/// @brief *= Value
	inline
	xyzVector &
	operator*=(Value const & t) {
		x_ *= t;
		y_ *= t;
		z_ *= t;
		return *this;
	}


	/// @brief /= Value
	inline
	xyzVector &
	operator/=(Value const & t) {
		assert(t != Value(0));
		Value const inv_t(Value(1) / t);
		x_ *= inv_t;
		y_ *= inv_t;
		z_ *= inv_t;
		return *this;
	}


public: // Methods


	/// @brief Zero
	inline
	xyzVector &
	zero() {
		x_ = y_ = z_ = Value(0);
		return *this;
	}


	/// @brief Negate
	inline
	xyzVector &
	negate() {
		x_ = -x_;
		y_ = -y_;
		z_ = -z_;
		return *this;
	}


	/// @brief -xyzVector (negated copy)
	inline
	xyzVector
	operator-() const {
		return xyzVector(-x_, -y_, -z_);
	}


	/// @brief Negated copy
	inline
	xyzVector
	negated() const {
		return xyzVector(-x_, -y_, -z_);
	}


	/// @brief Negated: Return via argument (slightly faster)
	inline
	void
	negated(xyzVector & a) const {
		a.x_ = -x_;
		a.y_ = -y_;
		a.z_ = -z_;
	}


	/// @brief xyzVector + xyzVector
	friend
	inline
	xyzVector
	operator+(xyzVector const & a, xyzVector const & b) {
		return xyzVector(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_);
	}


	/// @brief xyzVector + Value
	friend
	inline
	xyzVector
	operator+(xyzVector const & v, Value const & t) {
		return xyzVector(v.x_ + t, v.y_ + t, v.z_ + t);
	}


	/// @brief Value + xyzVector
	friend
	inline
	xyzVector
	operator+(Value const & t, xyzVector const & v) {
		return xyzVector(t + v.x_, t + v.y_, t + v.z_);
	}


	/// @brief xyzVector - xyzVector
	friend
	inline
	xyzVector
	operator-(xyzVector const & a, xyzVector const & b) {
		return xyzVector(a.x_ - b.x_, a.y_ - b.y_, a.z_ - b.z_);
	}


	/// @brief xyzVector - Value
	friend
	inline
	xyzVector
	operator-(xyzVector const & v, Value const & t) {
		return xyzVector(v.x_ - t, v.y_ - t, v.z_ - t);
	}


	/// @brief Value - xyzVector
	friend
	inline
	xyzVector
	operator-(Value const & t, xyzVector const & v) {
		return xyzVector(t - v.x_, t - v.y_, t - v.z_);
	}


	/// @brief xyzVector * Value
	friend
	inline
	xyzVector
	operator*(xyzVector const & v, Value const & t) {
		return xyzVector(v.x_ * t, v.y_ * t, v.z_ * t);
	}


	/// @brief Value * xyzVector
	friend
	inline
	xyzVector
	operator*(Value const & t, xyzVector const & v) {
		return xyzVector(t * v.x_, t * v.y_, t * v.z_);
	}


	/// @brief xyzVector / Value
	friend
	inline
	xyzVector
	operator/(xyzVector const & v, Value const & t) {
		assert(t != Value(0));
		Value const inv_t(Value(1) / t);
		return xyzVector(v.x_ * inv_t, v.y_ * inv_t, v.z_ * inv_t);
	}

	/// @brief Normalize
	inline
	xyzVector &
	normalize() {
		Value const length_ = get_length();
		assert(length_ != Value(0));
		Value const inv_length(Value(1) / length_);
		x_ *= inv_length;
		y_ *= inv_length;
		z_ *= inv_length;
		return *this;
	}

	/// @brief Distance
	inline
	Value
	distance(xyzVector const & v) const {
		return std::sqrt(square(x_ - v.x_) + square(y_ - v.y_) + square(z_ - v.z_));
	}

	/// @brief Distance squared
	inline
	Value
	distance_squared(xyzVector const & v) const {
		return square(x_ - v.x_) + square(y_ - v.y_) + square(z_ - v.z_);
	}

	/// @brief Dot product
	inline
	Value
	dot(xyzVector const & v) const {
		return (x_ * v.x_) + (y_ * v.y_) + (z_ * v.z_);
	}

	/// @brief Cross product
	inline
	xyzVector
	cross(xyzVector const & v) const {
		return xyzVector(
				(y_ * v.z_) - (z_ * v.y_),
				(z_ * v.x_) - (x_ * v.z_),
				(x_ * v.y_) - (y_ * v.x_)
		);
	}

public:
    friend
    std::ostream &
    operator<< (
            std::ostream & stream ,
            xyzVector const & v) {
        stream << v.get_str();
        return stream;
    }


public: // Properties: accessors


	/// @brief Value x const
	inline
	Value const &
	get_x() const { return x_; }

	/// @brief Value y const
	inline
	Value const &
	get_y() const { return y_; }


	/// @brief Value z const
	inline
	Value const &
	get_z() const { return z_; }


	/// @brief Length
	inline
	Value
	get_length() const {
		return std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
	}


	/// @brief Length squared
	inline
	Value
	get_length_squared() const {
		return (x_ * x_) + (y_ * y_) + (z_ * z_);
	}


	/// @brief Norm
	inline
	Value
	get_norm() const {
		return std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
	}


	/// @brief Norm squared
	inline
	Value
	get_norm_squared() const {
		return (x_ * x_) + (y_ * y_) + (z_ * z_);
	}


	/// @brief Magnitude
	inline
	Value
	get_magnitude() const {
		return std::sqrt((x_ * x_) + (y_ * y_) + (z_ * z_));
	}


	/// @brief Magnitude squared
	inline
	Value
	get_magnitude_squared() const {
		return (x_ * x_) + (y_ * y_) + (z_ * z_);
	}


public: // Properties: value assignment


	/// @brief x assignment
	inline
	void
	set_x(Value const & x_a) { x_ = x_a; }


	/// @brief y assignment
	inline
	void
	set_y(Value const & y_a) { y_ = y_a; }


	/// @brief z assignment
	inline
	void
	set_z(Value const & z_a) { z_ = z_a; }


public: // Comparison


	/// @brief xyzVector == xyzVector
	friend
	inline
	bool
	operator==(xyzVector const & a, xyzVector const & b) {
		return (a.x_ == b.x_) && (a.y_ == b.y_) && (a.z_ == b.z_);
	}


	/// @brief xyzVector != xyzVector
	friend
	inline
	bool
	operator!=(xyzVector const & a, xyzVector const & b) {
		return (a.x_ != b.x_) || (a.y_ != b.y_) || (a.z_ != b.z_);
	}


private: // Methods


	/// @brief square( t ) == t * t
	inline
	static
	Value
	square(Value const & t) { return t * t; }

private: // Fields

	/// @brief Coordinates of the 3 coordinate vector
	Value x_;
	Value y_;
	Value z_;


}; // xyzVector

typedef xyzVector<double> Vector;
typedef std::vector<Vector> Vectors;

typedef xyzVector<double> Point;
typedef std::vector<Point> Points;


}

#endif /* defined(__REDESIGNC__xyzVector__) */
