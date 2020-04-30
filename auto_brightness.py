from scipy.optimize import curve_fit
import matplotlib.pyplot as plt


class AutoLED:
    def __init__(self, data ="data.csv", params="params.csv"):
        self.data = data
        self.params = params
    def train(self):
        f = open(self.data, 'r')
        self.x = []
        self.y = []
        for line in f.readlines():
            l = line.split(';')
            self.x.append(float(l[0]))
            self.y.append(int(l[1]))
        print(self.x)
        print(self.y)
        f.close()
        popt, pcov = curve_fit(self.exponential_func, self.x, self.y)
        f = open(self.params, 'w')
        for p in popt:
            f.write("{};".format(p))
        f.close()
    def load(self):
        f = open(self.params, 'r')
        line = f.readlines()[0]
        line = line.split(';')
        self.a = float(line[0])
        self.b = float(line[1])
        self.c = float(line[2])
        print(self.a, self.b, self.c)
    def predict(self, x):
        value = self.a*pow(x, self.b) + self.c
        if value>255:
            value = 255
        elif value<0:
            value = 0
        v = int(value)
        return v
    @classmethod
    def exponential_func(cls, x, a, b, c):
        return a*pow(x, b) + c


if __name__ == '__main__':
    A = AutoLED()
    A.train()