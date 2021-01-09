class ExpectedValues
{
private:
    int expectedInt{};
    double expectedDouble{};

public:
    ExpectedValues(int i)
    {
        expectedInt = i;
    }
    ExpectedValues(double d)
    {
        expectedDouble = d;
    }
    ExpectedValues(int i, double d)
    {
        expectedInt = i;
        expectedDouble = d;
    }

    // Getter and Setter
    int getExpectedInt()
    {
        return expectedInt;
    }

    void setExpectedInt(int i)
    {
        expectedInt = i;
    }

    double getExpectedDouble()
    {
        return expectedDouble;
    }

    void setExpectedDouble(double d)
    {
        expectedDouble = d;
    }
};
