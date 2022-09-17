// Lab001.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <set>
#include <random>

class State {
public:
    virtual bool contains(int s) const {
        return true;
    }
};

class DiscreteState : public State {
private:
    int const state;

public:
    DiscreteState(int state) : state(state) { }

    bool contains(int s) const override {
        return s == state;
    }
};

class SegmentState : public State {
private:
    int const beg, end;

public:
    SegmentState() : beg(0), end(-1) { }
    SegmentState(int beg, int end) : beg(beg), end(end) { }

    bool contains(int s) const override {
        return s >= beg && s <= end;
    }
};

class SetState : public State {
private:
    std::set<int> const states;

public:
    SetState() : states() { }
    SetState(std::set<int> const& src) : states(src) { }

    bool contains(int s) const override {
        return states.count(s) > 0;
    }
};

class SegmentStateGaps : public State {
private:
    SegmentState const main;
    SetState const gaps;

public:
    SegmentStateGaps() : main(), gaps() { }
    SegmentStateGaps(SegmentState s, SetState ss) : main(s), gaps(ss) { }

    bool contains(int s) const override {
        return main.contains(s) && !gaps.contains(s);
    }
};

class SegmentStateAdds : public State {
private:
    SegmentState const main;
    SetState const adds;

public:
    SegmentStateAdds() : main(), adds() { }
    SegmentStateAdds(SegmentState s, SetState ss) : main(s), adds(ss) { }

    bool contains(int s) const override {
        return main.contains(s) || adds.contains(s);
    }
};

class SegmentStateGapsAdds : public State {
private:
    SegmentState const main;
    SetState const gaps, adds;

private:
    SegmentStateGapsAdds() : main(), gaps(), adds() { }
    SegmentStateGapsAdds(SegmentState s, SetState ss1, SetState ss2) : main(s), gaps(ss1), adds(ss2) { }

    bool contains(int s) const override {
        return main.contains(s) && !gaps.contains(s) || adds.contains(s);
    }
};

class StateUnion : public State {
private:
    State const state1, state2;

public:
    StateUnion() : state1(), state2() { }
    StateUnion(State s1, State s2) : state1(s1), state2(s2) { }

    bool contains(int s) const override {
        return state1.contains(s) || state2.contains(s);
    }
};

class StateIntersection : public State {
private:
    State const state1, state2;

public:
    StateIntersection() : state1(), state2() { }
    StateIntersection(State s1, State s2) : state1(s1), state2(s2) { }

    bool contains(int s) const override {
        return state1.contains(s) && state2.contains(s);
    }
};

class ProbabilityTest {
private:
    unsigned seed;
    int test_min, test_max;
    unsigned test_count;

public:
    ProbabilityTest(unsigned seed, int test_min, int test_max, unsigned test_count) : seed(seed), test_min(test_min), test_max(test_max), test_count(test_count) { }

    float operator()(State const& s) const {
        std::default_random_engine rng(seed);
        std::uniform_int_distribution<int> dstr(test_min, test_max);
        unsigned good = 0;
        for (unsigned cnt = 0; cnt != test_count; ++cnt)
            if (s.contains(dstr(rng))) ++good;

        return static_cast<float>(good) / static_cast<float>(test_count);
    }
};

int main(int argc, const char* argv[]) {
    DiscreteState d(2);
    SegmentState s(0, 10);
    SetState ss({ 1, 3, 5, 7, 23, 48, 57, 60, 90, 99 });
    StateUnion su(ss, d);
    ProbabilityTest pt(10, 0, 100, 100000);
    std::cout << pt(d) << std::endl;
    std::cout << pt(s) << std::endl;
    std::cout << pt(ss) << std::endl;
    std::cout << pt(su) << std::endl;
    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
