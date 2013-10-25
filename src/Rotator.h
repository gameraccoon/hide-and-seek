#ifndef ROTATOR_H
#define ROTATOR_H

// 3.1415...
extern const float PI;

/** Hiding details of implementation of rotation angle (if in the future we will want to change float to class) */
class Rotator
{
public:
	Rotator(float angle);
	~Rotator();

	float GetValue();
	
	friend Rotator operator-(Rotator rot);

	friend bool operator==(const Rotator& left, const Rotator& right);

	friend bool operator!=(const Rotator& left, const Rotator& right);

	friend Rotator operator+(const Rotator& left, const Rotator& right);

    friend Rotator operator+=(Rotator& left, const Rotator& right);

    friend Rotator operator-(const Rotator& left, const Rotator& right);

    friend Rotator operator-=(Rotator& left, const Rotator& right);
protected:
	float Value;
};

#endif
