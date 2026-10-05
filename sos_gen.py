import numpy as np
from scipy import signal


sos = signal.butter(3, 100, output='sos', fs=1000)

print(sos)

z, p, k = signal.sos2zpk(sos)

radii = np.abs(p)
stable = np.all(radii < 1)

print("Устойчив:", stable)
print("Максимальный модуль полюса:", radii.max())
