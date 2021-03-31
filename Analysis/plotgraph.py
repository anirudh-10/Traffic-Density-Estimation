import matplotlib.pyplot as plt
import math

def readfile (name):
    file = open(name,"r")
    l = file.readlines()
    l.pop(0)
    #print(l[0])
    m = list(map(lambda x: x.split(","),l))
    #print(m[0])
    m = list(map(lambda x: x[:3],m))
    # print("dsa")
    # print(m[1])
    # print("asd")
    m = list(map(lambda x: list(map(lambda y: float(y),x)),m))
    return m

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
    #print(m[2])
    x = list(map(lambda x: x[b-1],m))
    y = list(map(lambda x: x[c-1],m))
    plotgraph(x,y)
main()
    