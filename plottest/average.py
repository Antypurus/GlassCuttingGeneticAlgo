import matplotlib.pyplot as plt

file = open("average.txt","r")
lines = file.readlines()
n = []
for i in lines:
    n.append(float(i))
plt.plot(n)
plt.ylabel('population fitness')
plt.xlabel('genetation')
plt.show()