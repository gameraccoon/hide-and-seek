#pragma once

// 3.1415...
extern const float PI;

/** Hiding details of implementation of rotation angle (if in the future we will want to change float to class) */
class Rotator
{
public:
	explicit Rotator(float angle);

	/** Get value of angle */
	float getValue() const;
	/** Get rotator that mirrored from X-axis */
	friend Rotator operator-(Rotator rot);

	friend bool operator==(const Rotator& left, const Rotator& right);

	friend bool operator!=(const Rotator& left, const Rotator& right);

	friend Rotator operator+(const Rotator& left, const Rotator& right);

    friend Rotator operator+=(Rotator& left, const Rotator& right);

    friend Rotator operator-(const Rotator& left, const Rotator& right);

    friend Rotator operator-=(Rotator& left, const Rotator& right);

private:
	float mValue;
	mutable float calculatedValue;
	void reset();
};
