import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('C:\\Users\\user\\Desktop\\m.txt')

plt.subplot(111)
plt.scatter(data[:, 0], data[:, 1])
plt.scatter(data[0, 0], data[0, 1], color='red')
plt.scatter(data[-1, 0], data[-1, 1], color='green')
plt.xlabel('X')
plt.ylabel('Y')

num_points = len(data)
plt.title(f"Кількість міст: {num_points}")

plt.tight_layout()
plt.show()