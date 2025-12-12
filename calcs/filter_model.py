import numpy as np
import matplotlib.pyplot as plt

class FirstOrderIIRFilt:
    def __init__(self, alpha):
        self.alpha = alpha 
        self.out = 0

    def __call__(self, sample):
        self.out = (1 - self.alpha)*sample + self.alpha*self.out  
        return self.out

N = 1000
FIR_LEN = 300

nn = np.arange(0, N, 1)
step = np.concatenate((np.zeros(N//2), np.ones(N//2)))

fir_filt = np.convolve(step, np.ones(FIR_LEN)*(1/FIR_LEN), mode='same')

my_iir = FirstOrderIIRFilt(0.9)
iir_filt = np.array([my_iir(s) for s in step])

plt.plot(nn, step)
plt.plot(nn, fir_filt)
plt.plot(nn, iir_filt)
plt.show()

