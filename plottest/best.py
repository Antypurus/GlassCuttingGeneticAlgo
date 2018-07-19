import matplotlib.pyplot as plt

file = open("best.txt","r")
lines = file.readlines()
n = []
for i in lines:
    n.append(float(i))
plt.plot(n)
plt.ylabel('best fitness')
plt.xlabel('genetation')
plt.show()