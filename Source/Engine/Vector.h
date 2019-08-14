#pragma once
#define EPS 1e-9
#include <SDL_stdinc.h>
#include <cmath>

class Transform;

template<class TYPE>
class Vector
{
public:

	TYPE x, y, z;

	Vector()
	{}
	Vector(TYPE x, TYPE y, TYPE z=0) : x(x), y(y), z(z)
	{}

	// Operators ------------------------------------------------
	inline Vector operator -(const Vector &v) const
	{
		Vector r;

		r.x = x - v.x;
		r.y = y - v.y;
		r.z = z - v.z;

		return(r);
	}

	inline Vector operator + (const Vector &v) const
	{
		Vector r;

		r.x = x + v.x;
		r.y = y + v.y;
		r.z = z + v.z;

		return(r);
	}

	inline const Vector& operator -=(const Vector &v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return(*this);
	}

	inline const Vector& operator +=(const Vector &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;

		return(*this);
	}

	inline bool operator ==(const Vector& v) const
	{
		return (x == v.x && y == v.y && z==v.z);
	}

	inline bool operator !=(const Vector& v) const
	{
		return (x != v.x || y != v.y || z !=v.z);
	}

	inline Vector& operator =(const Vector& v)
	{
		
		x = v.x;
		y = v.y;
		z = v.z;
		return (*this);
	}

	inline Vector& operator =(const Vector* v)
	{
		x = v->x;
		y = v->y;
		z = v->z;
		return (*this);
	}

	inline Vector operator /(const TYPE& value)
	{
		Vector temp;
		temp.x = x/value;
		temp.y = y/value;
		temp.z = z/value;
		return temp;
	}

	inline Vector operator *(const TYPE& value)
	{
		Vector temp;
		temp.x = x * value;
		temp.y = y * value;
		temp.z = z * value;
		return temp;
	}

	// Utils ------------------------------------------------
	inline bool IsZero() const
	{
		return (x == 0 && y == 0 && z==0);
	}

	inline Vector& SetToZero()
	{
		x = y = z = 0;
		return(*this);
	}

	inline Vector& Negate()
	{
		x = -x;
		y = -y;
		z = -z;

		return(*this);
	}

	// Distances ---------------------------------------------
	inline TYPE DistanceTo(const Vector& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;
		TYPE fz = z - v.z;

		return sqrt((fx*fx) + (fy*fy) +(fz*fz));
	}

	inline Vector& SetMultiply(const Transform& t0, const Vector& v1)
	{
		x = t0.m[0][0] * v1.x + t0.m[0][1] * v1.y + t0.m[0][2];
		y = t0.m[1][0] * v1.x + t0.m[1][1] * v1.y + t0.m[1][2];
		return *this;
	}
};

typedef Vector<int> iVector;
typedef Vector<float> fVector;

class Transform
{
public:
	float m[2][3];

	inline Transform()
	{}

	inline Transform& setIdentity()
	{
		m[0][0] = m[1][1] = 1.0f;
		m[0][1] = m[0][2] = m[1][0] = m[1][2] = 0.0f;
		return *this;
	}

	inline Transform& setTranslation(const fVector& trans)
	{
		m[0][0] = m[1][1] = 1.0f;
		m[0][1] = m[1][0] = 0.0f;
		m[0][2] = trans.x;
		m[1][2] = trans.y;
		return *this;
	}

	inline Transform& setTranslationOnly(const fVector& trans)
	{
		m[0][2] = trans.x;
		m[1][2] = trans.y;
		return *this;
	}

	inline fVector getTranslation() const
	{
		return fVector(m[0][2], m[1][2]);
	}


	inline Transform& setRotation(float angle)
	{
		m[0][0] = m[1][1] = cosf(angle);
		m[1][0] = sinf(angle);
		m[0][1] = -m[1][0];
		m[0][2] = m[1][2] = 0.0f;
		return *this;
	}

	inline Transform setRotationOnly(float angle)
	{
		fVector scale = getScale();
		float c = cosf(angle);
		float s = sinf(angle);
		m[0][0] = c * scale.x;
		m[0][1] = -s * scale.x;
		m[1][0] = s * scale.y;
		m[1][1] = c * scale.y;
		return *this;
	}

	inline float getRotation() const
	{
		float scaleX = sqrtf(sqrt(m[0][0]) + sqrt(m[0][1]));
		float angle = acosf(m[0][0] / scaleX);
		if(m[1][0] < -EPS)
			angle = 2 * (float)M_PI - angle;
		return angle;
	};

	inline Transform& setScale(const fVector& scale)
	{
		m[0][0] = scale.x;
		m[1][1] = scale.y;
		m[0][1] = m[0][2] = m[1][0] = m[1][2] = 0.0f;
		return *this;
	}

	inline Transform& setScaleOnly(const fVector& scale)
	{
		fVector oldScale = getScale();
		float factorX = scale.x / oldScale.x;
		float factorY = scale.y / oldScale.y;
		m[0][0] *= factorX;
		m[0][1] *= factorX;
		m[1][0] *= factorY;
		m[1][1] *= factorY;
		return *this;
	}

	inline fVector getScale() const
	{
		return fVector(sqrtf(sqrt(m[0][0]) + sqrt(m[0][1])),
			sqrtf(sqrt(m[1][0]) + sqrt(m[1][1])));
	}

	inline Transform& set(const fVector& trans, float angle, const fVector& scale)
	{
		float c = cosf(angle);
		float s = sinf(angle);
		m[0][0] = c * scale.x;
		m[0][1] = -s * scale.x;
		m[1][0] = s * scale.y;
		m[1][1] = c * scale.y;
		m[0][2] = trans.x;
		m[1][2] = trans.y;
		return *this;
	}

	inline Transform& setMultiply(const Transform& t0, const Transform& t1)
	{
		for(int row = 0; row < 2; ++row)
		{
			m[row][0] = t0.m[row][0] * t1.m[0][0] + t0.m[row][1] * t1.m[1][0];
			m[row][1] = t0.m[row][0] * t1.m[0][1] + t0.m[row][1] * t1.m[1][1];
			m[row][2] = t0.m[row][0] * t1.m[0][2] + t0.m[row][1] * t1.m[1][2] + t0.m[row][2];
		}
		return *this;
	}

	inline Transform& invert(const Transform& t)
	{
		float invdet = 1.0f / (t.m[0][0] * t.m[1][1] - t.m[0][1] * t.m[1][0]);
		m[0][0] = t.m[1][1] * invdet;
		m[0][1] = -t.m[0][1] * invdet;
		m[0][2] = (t.m[0][1] * t.m[1][2] - t.m[0][2] * t.m[1][1]) * invdet;
		m[1][0] = -t.m[1][0] * invdet;
		m[1][1] = t.m[0][0] * invdet;
		m[1][2] = (t.m[0][2] * t.m[1][0] - t.m[0][0] * t.m[1][2]) * invdet;
		return *this;
	}

	inline Transform getInverse() const
	{
		Transform res;
		return res.invert(*this);
	}

	inline Transform operator*(const Transform& t) const
	{
		Transform res;
		return res.setMultiply(*this, t);
	}

	inline fVector operator*(const fVector& v) const
	{
		fVector res;
		return res.SetMultiply(*this, v);
	}
};