#pragma once

class TestCheck
{
public:
	virtual bool isPassed() const = 0;
	virtual std::string describe() const = 0;
};

struct TestChecklist
{
	std::map<std::string, std::unique_ptr<TestCheck>> checks;
};
