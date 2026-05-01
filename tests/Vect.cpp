#include <cmath>
#include <iostream>
#include "Vect.h"

Vect::Vect(std::initializer_list<float> inputFloats) 
    :dimension(inputFloats.size()), values(inputFloats)
{
}

Vect::Vect(int dim) 
    :dimension(dim)
{
    values.reserve(dim);
}

Vect::Vect(Vect vector, std::initializer_list<float> inputFloats) 
    :dimension(vector.dimension + inputFloats.size())
{
    values.reserve(vector.dimension + inputFloats.size());
    int i;
    for (i = 0; i < vector.dimension; i++)
    {
        values.push_back(vector[i]);
    }

    for (float val : inputFloats) 
    {
        values.push_back(val);
        i++;
    }
}

bool Vect::operator== (Vect &rhs) const
{
    if (*this == rhs) {return true;}

    return (Equals(rhs));
}

bool Vect::operator!= (Vect *rhs) const
{
    return !(this == rhs);
}

Vect Vect::operator+ (const Vect &rhs) const
{
    return Add(*this, rhs);
}

Vect Vect::operator+= (const Vect &rhs) 
{
    assert(dimension == rhs.dimension);

    for (int i = 0; i < dimension; i++) 
    {
        values[i] += rhs[i];
    }
    
    return *this;
}

Vect Vect::operator-= (const Vect &rhs) 
{
    assert(dimension == rhs.dimension);

    for (int i = 0; i < dimension; i++) 
    {
        values[i] -= rhs[i];
    }
    
    return *this;
}

Vect Vect::operator* (float scalar) const
{
    return ScalarMultiply(scalar, *this);
}

Vect operator*(float scalar, Vect &rhs)
{
    return rhs * scalar;
}

Vect Vect::operator/ (float scalar) const
{
    return *this * (1/scalar);
}

Vect Vect::operator- (const Vect &rhs) const
{
    return *this + (rhs * -1.0f);
}

bool Vect::Equals(Vect &other) const
{
    if (other.dimension != dimension) return false;  

    for (int i = 0; i < dimension; i++)
    {
        if (other[i] != values[i])
        {
            return false;
        }
    }
    return true;
}

float Vect::Magnitude()
{
    float mag = 0;
    for (int i = 0; i < dimension; i++) 
    {
        mag += values[i] * values[i];
    }
    mag = sqrt(mag);

    return mag;
};

Vect Vect::Add(const Vect &lhs, const Vect &rhs) const
{
    assert(lhs.dimension == rhs.dimension);
    
    Vect output(lhs.dimension);
    for (int i = 0; i < lhs.dimension; i++)
    {
        output[i] = lhs[i] + rhs[i];
    }
    return output;
};

Vect Vect::ScalarMultiply(float scalar, const Vect &vector)
{
    Vect output(vector.dimension);
    for (int i = 0; i < vector.dimension; i++)
    {
        output[i] = scalar * vector[i];
    }
    return output;
};

float Vect::Dot(Vect &lhs, Vect &rhs)
{
    assert(lhs.dimension == rhs.dimension);
    
    float output = 0;
    for (int i = 0; i < lhs.dimension; i++)
    {
        output += lhs[i] * rhs[i];
    }
    return output;
};

Vect Vect::Cross(Vect &lhs, Vect &rhs)
{
    assert(lhs.dimension == 3);
    assert(lhs.dimension == rhs.dimension);

    Vect vectVals = {
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0]
    };

    return Vect(vectVals);
};

Vect Vect::Normalize(Vect &vector)
{
    Vect output(vector.dimension);
    for (int i = 0; i < vector.dimension; i++)
    {
        output[i] = vector[i] / vector.Magnitude();;
    }
    return output;
}

Vect Vect::Lerp(const Vect &lhs, const Vect &rhs, float t)
{
    assert(lhs.dimension == rhs.dimension);

    if (t > 1) {t = 1;}
    else if (t < 0) {t = 0;}

    Vect output = {
        lhs[0] + (rhs[0] - lhs[0]) * t,
        lhs[1] + (rhs[1] - lhs[1]) * t,
        lhs[2] + (rhs[2] - lhs[2]) * t
    };

    return output;
}

glm::vec3 Vect::ToGLM() const 
{
    assert(dimension == 3);

    return glm::vec3((*this)[0], (*this)[1], (*this)[2]);
}

Vect Vect::ConvertToBasis(Vect &vector, Vect *basis) 
{
    return Vect(0);
}
