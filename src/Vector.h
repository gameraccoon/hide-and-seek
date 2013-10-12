#ifndef VECTOR_H
#define VECTOR_H

class Vector
{
public:
	Vector(void);
	Vector(const Vector& vect);
	virtual ~Vector(void);
	float X, Y, Z;

	friend const Vector operator+(const Vector& left, const Vector& right);

    friend Vector& operator+=(Vector& left, const Vector& right);

    friend bool operator==(const Vector& left, const Vector& right);
};

#endif