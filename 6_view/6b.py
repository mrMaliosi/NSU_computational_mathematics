import numpy as np
import matplotlib.pyplot as plt

# Определим исходную функцию
def f(x):
    return (np.exp(x) * (np.sin(x) - np.cos(x))) / 2 + 3 / 2;

# Определим производную исходной функции
def g(x):
    return np.exp(x) * np.sin(x)

# Разностные операторы
def second_order_difference(f, x, h):
    """ Разностный оператор второго порядка """
    return (-f(x) + f(x + h)) / h - ((1/2) * g(x) + (1/2) * g(x + h))

def forth_order_difference(f, x, h):
    """ Разностный оператор четвёртого порядка """
    return ((-0.5)*f(x - 0.5*h) + (0.5)*f(x + 0.5*h)) / h - ((1/6) * g(x + (1/6)*h) + (2/3) * g(x + (2/3)*h) + (1/6) * g(x + (1/6)*h))


# Параметры
h = 0.01  # шаг сетки
x_min = -5  # минимальное значение x
x_max = 5  # максимальное значение x
num_points = 1000  # количество точек сетки

# Создаем массив точек сетки
x = np.linspace(x_min, x_max, num_points)
y_exact = g(x)  # Значения производной функции

# Создаем фигуру для графиков
plt.figure(figsize=(12, 10))

# Для каждого оператора
operators = [second_order_difference, forth_order_difference]
operator_names = ['2nd Order Difference', '4th Order Difference']

for i, operator in enumerate(operators):
    y_approx = np.array([operator(f, xi, h) for xi in x])  # Приближение для каждой точки
    plt.subplot(2, 1, i+1)  # 3 строки, 1 колонка, график i+1
    plt.plot(x, y_exact, label="Производная функции", color='blue')
    plt.plot(x, y_approx, label=f"Приближение по {operator_names[i]}", color='red')
    plt.title(f"График {operator_names[i]}")
    plt.legend()
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True)

plt.tight_layout()  # Для лучшего расположения графиков
plt.show()
