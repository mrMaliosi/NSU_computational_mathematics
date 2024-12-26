import numpy as np
import matplotlib.pyplot as plt

# Исходная функция
def f(x):
    return np.abs(x)

# Интерполяционный полином Лагранжа
def lagrange_interpolation(x, y, xi):
    n = len(x)
    L = np.zeros_like(xi)
    for i in range(n):
        li = np.ones_like(xi)
        for j in range(n):
            if i != j:
                li *= (xi - x[j]) / (x[i] - x[j])
        L += y[i] * li
    return L

# Оценка погрешности
def error_estimation(f, p, x):
    return np.abs(f(x) - p(x))

# Точки для интерполяции
x = np.linspace(-2, 2, 5)
y = f(x)

# Точки для построения графика
xi = np.linspace(-2, 2, 100)
yi = f(xi)
pi = lagrange_interpolation(x, y, xi)

# Построение графика
plt.plot(xi, yi, label='Исходная функция |x|')
plt.plot(xi, pi, label='Интерполяционный полином Лагранжа')
plt.scatter(x, y, color='red', label='Точки интерполяции')
plt.legend()
plt.xlabel('x')
plt.ylabel('f(x)')
plt.title('Интерполяция функции |x| на интервале [-2, 2]')
plt.grid(True)
plt.show()

# Анализ сходимости
errors = error_estimation(f, lambda x: lagrange_interpolation(x, y, x), xi)
print('Максимальная погрешность:', np.max(errors))
