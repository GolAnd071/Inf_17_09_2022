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
    State const* state1, * state2;

public:
    StateUnion() : state1(), state2() { }
    StateUnion(State* s1, State* s2) : state1(s1), state2(s2) { }

    bool contains(int s) const override 
    {
        return state1 -> contains(s) || state2 -> contains(s);
    }
};

class StateIntersection : public State 
{
private:
    State const* state1, * state2;

public:
    StateIntersection() : state1(), state2() { }
    StateIntersection(State* s1, State* s2) : state1(s1), state2(s2) { }

    bool contains(int s) const override 
    {
        return state1 -> contains(s) && state2 -> contains(s);
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

int main(int argc, const char* argv[])
{
#pragma region Test 1
    // -- Test 1 ------------------------------ //
    {
        // Open file to export data to Python script
        std::fstream table;
        table.open("..\\Plots\\temp.txt", std::ios::out);
        system("del \"..\\Plots\\ResultsTest1.txt\"");

        // Create a couple of states
        SegmentState s(45, 54);
        SetState ss({ 1, 10, 11, 25, 37, 89, 95, 97, 98, 99 });

        // Probability test
        for (int t = 10; t <= 10000; t += 10) {
            ProbabilityTest pt(1001, 0, 100, t);
            table << t << ' ' << pt(s) << ' ' << pt(ss) << '\n';
        }

        // Close file
        table.close();
        system("ren \"..\\Plots\\temp.txt\" \"ResultsTest1.txt\"");
    }
    // ---------------------------------------- //
#pragma endregion
    
#pragma region Test 2
    // -- Test 2 ------------------------------ //
    {
        // Create state
        SetState ss({ 1, 10, 11, 25, 37, 89, 95, 97, 98, 99 });

        // Probability test
        ProbabilityTest pt(1001, 0, 100, 100000);
        std::cout << std::setprecision(2) << 10.0 / 100.0 << std::setw(10) << std::setprecision(5) << pt(ss) << "\n\n";
    }
    // ---------------------------------------- //
#pragma endregion

#pragma region Test 3
    // -- Test 3 ------------------------------ //
    {
        SegmentState ss(1, 10);
        StateUnion su(new SegmentState(ss), new SetState({ 50 }));
        ProbabilityTest pt(1001, 0, 100, 1000000);
        std::cout << pt(su);
    }
    // ---------------------------------------- //
#pragma endregion
}
