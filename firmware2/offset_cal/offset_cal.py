import matplotlib.pyplot as plt
import numpy as np
data = open('data.csv').read()

def rms(signal:list[float]) -> float: return round(np.sqrt(np.sum(np.square(signal))/len(signal)), 4)

xx = []
nn = []

i = 0

for line in data.split('\n'):
    if line != '':
        xx.append(float(line))
        nn.append(i)
        i += 1

average = sum(xx)/len(xx)
print('average:', str(average))
print('rms:', str(rms(np.array(xx)-average)))
plt.plot(nn, xx)
plt.show()