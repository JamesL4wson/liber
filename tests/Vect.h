#pragma once

#include <initializer_list>
#include <vector>
#include <glm/glm.hpp>

class Vect 
{
    public:
        Vect(std::initializer_list<float> inputFloats);
        Vect(int length);
        Vect(Vect vector, std::initializer_list<float> inputFloats);
        
        inline float& operator[] (int index);
        inline const float& operator[] (int index) const;
        bool operator== (Vect &rhs) const;
        bool operator!=(Vect *rhs) const;
        Vect operator+ (const Vect &rhs) const;
        Vect operator+= (const Vect &rhs);
        Vect operator-= (const Vect &rhs);
        Vect operator* (float scalar) const;
        friend Vect operator* (float scalar, Vect &rhs);
        Vect operator/ (float scalar) const;
        Vect operator- (const Vect &rhs) const;
        
        static float Dot(Vect &lhs, Vect &rhs);
        static Vect ScalarMultiply(float scalar, const Vect &vector);
        static Vect Cross(Vect &lhs, Vect &rhs);
        static Vect Normalize(Vect &vector);
        static Vect Lerp(const Vect &lhs, const Vect &rhs, float t);
        glm::vec3 ToGLM() const;

        Vect ConvertToBasis(Vect &vector, Vect *basis);
        
        int dimension;
        float Magnitude();

    private:
        std::vector<float> values; 
        bool Equals(Vect &other) const;
        Vect Add(const Vect &lhs, const Vect &rhs) const;

};

inline float& Vect::operator[] (const int index)
{
    return values[index];
}

inline const float& Vect::operator[](int index) const
{
    return values[index];
}
