import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('C:\\Users\\user\\Desktop\\m.txt')

plt.subplot(111)
plt.scatter(data[:, 0], data[:, 1])
plt.scatter(data[0, 0], data[0, 1], color='red')  # Змінено кольор першої точки на червоний
plt.xlabel('X')
plt.ylabel('Y')

plt.tight_layout()
plt.show()