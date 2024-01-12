import matplotlib.pyplot as plt 
import pandas as pd

data = pd.read_csv('cap2.csv').to_numpy()
time = data[:,0]
voltage = data[:,1]

plt.plot(time, voltage)
plt.show()
