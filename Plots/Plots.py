import matplotlib.pyplot as plt
import os


def main():
    x = list()
    y1 = list()
    y2 = list()
    y3 = list()
    while True:
        try:
            with open('ResultsTest1.txt') as f:
                for i in range(10, 10001, 10):
                    temp = list(map(float, f.readline().split()))
                    x.append(temp[0])
                    y1.append(temp[1])
                    y2.append(temp[2])
                    y3.append(0.1)
            break
        except FileNotFoundError:
            pass
    os.remove('ResultsTest1.txt')
    plt.plot(x, y1)
    plt.plot(x, y2)
    plt.plot(x, y3)
    plt.show()


if __name__ == '__main__':
    main()
