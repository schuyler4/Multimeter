import matplotlib.pyplot as plt

xx = []
nn = []

data = open('logging.csv').read()

for i, line in enumerate(data.split('\n')):
    if line != '':
        xx.append(float(line))    
        nn.append(i)

plt.plot(nn, xx)
plt.show()
