// Lab001.cpp : GolAnd071
// 17.09.2022

#include <iostream>
#include <set>
#include <random>
#include <iomanip>
#include <fstream>

class State 
{
public:
    virtual bool contains(int) const = 0;
};

class DiscreteState : public State
{
private:
    int const state;

public:
    DiscreteState() : state(0) { }
    DiscreteState(int state) : state(state) { }

    bool contains(int s) const override
    {
        return s == state;
    }
};

class SegmentState : public State 
{
private:
    int const beg, end;

public:
    SegmentState() : beg(0), end(-1) { }
    SegmentState(int beg, int end) : beg(beg), end(end) { }

    bool contains(int s) const override 
    {
        return s >= beg && s <= end;
    }
};

class SetState : public State 
{
private:
    std::set<int> const states;

public:
    SetState() : states() { }
    SetState(std::set<int> const& src) : states(src) { }

    bool contains(int s) const override 
    {
        return states.count(s) > 0;
    }
};

class SegmentStateGaps : public State 
{
private:
    SegmentState const main;
    SetState const gaps;

public:
    SegmentStateGaps() : main(), gaps() { }
    SegmentStateGaps(SegmentState s, SetState ss) : main(s), gaps(ss) { }

    bool contains(int s) const override 
    {
        return main.contains(s) && !gaps.contains(s);
    }
};

class SegmentStateAdds : public State 
{
private:
    SegmentState const main;
    SetState const adds;

public:
    SegmentStateAdds() : main(), adds() { }
    SegmentStateAdds(SegmentState s, SetState ss) : main(s), adds(ss) { }

    bool contains(int s) const override 
    {
        return main.contains(s) || adds.contains(s);
    }
};

class SegmentStateGapsAdds : public State 
{
private:
    SegmentState const main;
    SetState const gaps, adds;

private:
    SegmentStateGapsAdds() : main(), gaps(), adds() { }
    SegmentStateGapsAdds(SegmentState s, SetState ss1, SetState ss2) : main(s), gaps(ss1), adds(ss2) { }

    bool contains(int s) const override 
    {
        return main.contains(s) && !gaps.contains(s) || adds.contains(s);
    }
};

class StateUnion : public State 
{
private:
    State const& state1, & state2;

public:
    StateUnion() : state1(DiscreteState()), state2(DiscreteState()) { }
    StateUnion(State const& s1, State const& s2) : state1(s1), state2(s2) { }

    bool contains(int s) const override 
    {
        return state1.contains(s) || state2.contains(s);
    }
};

class StateIntersection : public State 
{
private:
    State const& state1, & state2;

public:
    StateIntersection() : state1(DiscreteState()), state2(DiscreteState()) { }
    StateIntersection(State const& s1, State const& s2) : state1(s1), state2(s2) { }

    bool contains(int s) const override 
    {
        return state1.contains(s) && state2.contains(s);
    }
};

class ProbabilityTest 
{
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : 
        seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

    float operator()(State const& s) const 
    {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }
};

#pragma region UtilityForTests

template <class T>
void func1(T s) {
    for (int i = 0; i < 10000; i += 10) {
        ProbabilityTest pt(10, 0, 100, i + 1);
        std::cout << std::fixed << std::setw(10) << pt(s) << '\n';
    }
    std::cout << '\n';
}

template <class T>
void func2(T s, int ss) {
    ProbabilityTest pt1(10, 0, 100, 1000000), pt2(10, 0, 200, 1000000), pt3(10, 0, 500, 1000000);
    std::cout << ss << ' ' << 101 << ' ' << std::fixed << std::setprecision(6) << pt1(s) << ' ' << ss / 101.0 << '\n';
    std::cout << ss << ' ' << 201 << ' ' << std::fixed << std::setprecision(6) << pt2(s) << ' ' << ss / 201.0 << '\n';
    std::cout << ss << ' ' << 501 << ' ' << std::fixed << std::setprecision(6) << pt3(s) << ' ' << ss / 501.0 << "\n\n";
}

template <class T>
void func3(T s) {
    func1(s);
    func2(s, 11);
}

#pragma endregion

int main(int argc, const char* argv[])
{
#pragma region Test 1
    // -- Test 1 ------------------------------ //
    {
        func1(SegmentState(0, 10));
        func1(SetState({ 1, 3, 5, 7, 11, 23, 48, 57, 60, 90, 99 }));
    }
    // ---------------------------------------- //
#pragma endregion
    
#pragma region Test 2
    // -- Test 2 ------------------------------ //
    {
        func2(SegmentState(0, 10), 11);
        func2(SegmentState(0, 20), 21);
        func2(SegmentState(0, 50), 51);
    }
    // ---------------------------------------- //
#pragma endregion

#pragma region Test 3
    // -- Test 3 ------------------------------ //
    {
        func3(StateUnion(SegmentState(0, 9), DiscreteState(11)));
        func3(SegmentStateGaps(SegmentState(0, 11), SetState({ 5 })));
        func3(StateIntersection(SegmentState(0, 11), SegmentState(1, 12)));
    }
    // ---------------------------------------- //
#pragma endregion
}
