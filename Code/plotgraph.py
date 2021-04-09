import matplotlib.pyplot as plt
import math

def readfile (name):
    file = open(name,"r")
    l = file.readlines()
    l.pop(0)
    m = list(map(lambda x: x.split(","),l))
    n = len(m[0])
    m = list(map(lambda x: x[:n-1],m))
    m = list(map(lambda x: list(map(lambda y: float(y),x)),m))
    return m

def compare(x):
    return x[0]

def plotgraph (x,y):
    plt.plot(x, y)
    plt.xlabel('x - axis')
    plt.ylabel('y - axis')
    plt.title('Graph')
    plt.show()

def main():
    a = input("Give filename: ")
    b = int(input("x-axis: "))
    c = int(input("y-axis: "))
    m = readfile(a)
    x = list(map(lambda x: x[b-1],m))
    y = list(map(lambda x: x[c-1],m))
    new_l = [(0,0) for i in  range(len(x))]
    for i in range(len(x)):
        z = (x[i],y[i])
        new_l[i] = z
    new_k = sorted(new_l,key = compare)
    x = list(map(lambda x: x[0],new_k))
    y = list(map(lambda x: x[1],new_k))
    plotgraph(x,y)
main()
    