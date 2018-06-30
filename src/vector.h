#pragma once

#include "ed.h"

namespace ed
{

    template <int Dimension>
    class Vector
    {
    public:
        Vector();
        Vector(float x);
        Vector(float x, float y);
        Vector(float x, float y, float z);
        Vector(float x, float y, float z, float w);

        float  x() const;
        float& x();

        float  y() const;
        float& y();

        float  z() const;
        float& z();

        float  w() const;
        float& w();

        Vector<Dimension> abs() const;
        Vector<Dimension> min(const Vector<Dimension>& other) const;
        Vector<Dimension> max(const Vector<Dimension>& other) const;

        bool isZero(float epsilon = 1e-5f) const;
        bool isNormalized(float epsilon = 1e-5f) const;

        float distance(const Vector<Dimension>& point) const;
        float squaredDistance(const Vector<Dimension>& point) const;

        Vector<Dimension> normalized() const;
        float magnitude() const;
        float squaredMagnitude() const;

        float  operator[] (int i) const;
        float& operator[] (int i);

    private:
        float mData[Dimension];
    };

    typedef Vector<2> Vector2;
    typedef Vector<3> Vector3;
    typedef Vector<4> Vector4;

    template <int Dimension>
    inline Vector<Dimension> normalize(const Vector<Dimension>& v)
    {
        return v.normalized();
    }

    template <int Dimension>
    inline float magnitude(const Vector<Dimension>& v)
    {
        return v.magnitude();
    }

    template <int Dimension>
    inline float dot(const Vector<Dimension>& v1, const Vector<Dimension>& v2)
    {
        float result = 0.0f;
        for (int i = 0; i < Dimension; ++i) {
            result += v1[i] * v2[i];
        }
        return result;
    }

    inline Vector3 cross(const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1.y() * v2.z() - v1.z() * v2.y(),
                       v1.z() * v2.x() - v1.x() * v2.z(),
                       v1.x() * v2.y() - v1.y() * v2.x());
    }

    /*******************************************************************************
    *   Vector2 Operators                                                          *
    *******************************************************************************/

    Vector2 operator+ (float s, const Vector2& v)
    {
        return Vector2(s + v.x(), s + v.y());
    }

    Vector2 operator+ (const Vector2& v, float s)
    {
        return Vector2(v.x() + s, v.y() + s);
    }

    Vector2 operator+ (const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1.x() + v2.x(), v1.y() + v2.y());
    }

    Vector2 operator- (float s, const Vector2& v)
    {
        return Vector2(s - v.x(), s - v.y());
    }

    Vector2 operator- (const Vector2& v, float s)
    {
        return Vector2(v.x() - s, v.y() - s);
    }

    Vector2 operator- (const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1.x() - v2.x(), v1.y() - v2.y());
    }

    Vector2 operator* (float s, const Vector2& v)
    {
        return Vector2(s * v.x(), s * v.y());
    }

    Vector2 operator* (const Vector2& v, float s)
    {
        return Vector2(v.x() * s, v.y() * s);
    }

    Vector2 operator* (const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1.x() * v2.x(), v1.y() * v2.y());
    }

    Vector2 operator/ (float s, const Vector2& v)
    {
        return Vector2(s / v.x(), s / v.y());
    }

    Vector2 operator/ (const Vector2& v, float s)
    {
        return Vector2(v.x() / s, v.y() / s);
    }

    Vector2 operator/ (const Vector2& v1, const Vector2& v2)
    {
        return Vector2(v1.x() / v2.x(), v1.y() / v2.y());
    }

    /*******************************************************************************
    *   Vector3 Operators                                                          *
    *******************************************************************************/

    Vector3 operator+ (float s, const Vector3& v)
    {
        return Vector3(s + v.x(), s + v.y(), s + v.z());
    }

    Vector3 operator+ (const Vector3& v, float s)
    {
        return Vector3(v.x() + s, v.y() + s, v.z() + s);
    }

    Vector3 operator+ (const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
    }

    Vector3 operator- (float s, const Vector3& v)
    {
        return Vector3(s - v.x(), s - v.y(), s - v.z());
    }

    Vector3 operator- (const Vector3& v, float s)
    {
        return Vector3(v.x() - s, v.y() - s, v.z() - s);
    }

    Vector3 operator- (const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
    }

    Vector3 operator* (float s, const Vector3& v)
    {
        return Vector3(s * v.x(), s * v.y(), s * v.z());
    }

    Vector3 operator* (const Vector3& v, float s)
    {
        return Vector3(v.x() * s, v.y() * s, v.z() * s);
    }

    Vector3 operator* (const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
    }

    Vector3 operator/ (float s, const Vector3& v)
    {
        return Vector3(s / v.x(), s / v.y(), s / v.z());
    }

    Vector3 operator/ (const Vector3& v, float s)
    {
        return Vector3(v.x() / s, v.y() / s, v.z() / s);
    }

    Vector3 operator/ (const Vector3& v1, const Vector3& v2)
    {
        return Vector3(v1.x() / v2.x(), v1.y() / v2.y(), v1.z() / v2.z());
    }

    /*******************************************************************************
    *   Vector Constructors                                                        *
    *******************************************************************************/

    template <int Dimension>
    Vector<Dimension>::Vector()
        : mData { 0.0f }
    {
    }

    template <int Dimension>
    Vector<Dimension>::Vector(float x)
    {
        for (int i = 0; i < Dimension; ++i) {
            mData[i] = x;
        }
    }

    template <int Dimension>
    Vector<Dimension>::Vector(float x, float y)
        : mData { x, y }
    {
    }

    template <int Dimension>
    Vector<Dimension>::Vector(float x, float y, float z)
        : mData { x, y, z }
    {
    }

    template <int Dimension>
    Vector<Dimension>::Vector(float x, float y, float z, float w)
        : mData { x, y, z, w }
    {
    }

    /*******************************************************************************
    *   Vector Accessors                                                           *
    *******************************************************************************/

    template <int Dimension>
    float Vector<Dimension>::x() const
    {
        return mData[0];
    }

    template <int Dimension>
    float& Vector<Dimension>::x()
    {
        return mData[0];
    }

    template <int Dimension>
    float Vector<Dimension>::y() const
    {
        return mData[1];
    }

    template <int Dimension>
    float& Vector<Dimension>::y()
    {
        return mData[1];
    }

    template <int Dimension>
    float Vector<Dimension>::z() const
    {
        return mData[2];
    }

    template <int Dimension>
    float& Vector<Dimension>::z()
    {
        return mData[2];
    }

    template <int Dimension>
    float Vector<Dimension>::w() const
    {
        return mData[3];
    }

    template <int Dimension>
    float& Vector<Dimension>::w()
    {
        return mData[3];
    }

    /*******************************************************************************
    *   Vector Methods                                                             *
    *******************************************************************************/

    template <int Dimension>
    Vector<Dimension> Vector<Dimension>::abs() const
    {
        Vector<Dimension> result;
        for (int i = 0; i < Dimension; ++i) {
            result[i] = fabsf(mData[i]);
        }
        return result;
    }

    template <int Dimension>
    Vector<Dimension> Vector<Dimension>::min(const Vector<Dimension>& other) const
    {
        Vector<Dimension> result;
        for (int i = 0; i < Dimension; ++i) {
            result[i] = std::min(mData[i], other[i]);
        }
        return result;
    }
    
    template <int Dimension>
    Vector<Dimension> Vector<Dimension>::max(const Vector<Dimension>& other) const
    {
        Vector<Dimension> result;
        for (int i = 0; i < Dimension; ++i) {
            result[i] = std::max(mData[i], other[i]);
        }
        return result;
    }

    template <int Dimension>
    bool Vector<Dimension>::isZero(float epsilon) const
    {
        for (int i = 0; i < Dimension; ++i) {
            if (fabsf(mData[i]) > epsilon) {
                return false;
            }
        }
        return true;
    }
    
    template <int Dimension>
    bool Vector<Dimension>::isNormalized(float epsilon) const
    {
        return fabsf(magnitude() - 1.0f) < epsilon;
    }
    
    template <int Dimension>
    float Vector<Dimension>::distance(const Vector<Dimension>& point) const
    {
        (*this - point).magnitude();
    }
    
    template <int Dimension>
    float Vector<Dimension>::squaredDistance(const Vector<Dimension>& point) const
    {
        (*this - point).squaredMagnitude();
    }

    template <int Dimension>
    Vector<Dimension> Vector<Dimension>::normalized() const
    {
        return *this / magnitude();
    }
    
    template <int Dimension>
    float Vector<Dimension>::magnitude() const
    {
        return sqrtf(squaredMagnitude());
    }
    
    template <int Dimension>
    float Vector<Dimension>::squaredMagnitude() const
    {
        float result = 0.0f;
        for (int i = 0; i < Dimension; ++i) {
            result += mData[i] * mData[i];
        }
        return result;
    }

    template <int Dimension>
    float Vector<Dimension>::operator[] (int i) const
    {
        return mData[i];
    }
    
    template <int Dimension>
    float& Vector<Dimension>::operator[] (int i)
    {
        return mData[i];
    }

}