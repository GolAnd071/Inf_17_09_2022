import matplotlib.pyplot as plt
import os


def main():
    g = [list() for _ in range(4)]

    while True:
        try:
            with open('ResultsTest1.txt') as f:
                for i in range(10, 10001, 10):
                    temp = list(map(float, f.readline().split()))
                    for i in range(3):
                        g[i].append(temp[i])
                    g[3].append(0.1)
            break
        except FileNotFoundError:
            pass
    os.remove('ResultsTest1.txt')
    plt.plot(g[0], g[1], color='r', label='SegmentState')
    plt.plot(g[0], g[2], color='g', label='SetState')
    plt.plot(g[0], g[3], color='b', label='AsymptoticValue')
    plt.show()


if __name__ == '__main__':
    main()
