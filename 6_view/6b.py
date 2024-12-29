import numpy as np
import matplotlib.pyplot as plt

# Определим исходную функцию
def f(x):
    return (np.exp(x) * (np.sin(x) - np.cos(x))) / 2 + 3 / 2

# Определим производную исходной функции
def g(x):
    return np.exp(x) * np.sin(x)

# Разностные операторы
def second_order_difference(f, x, h):
    """ Разностный оператор второго порядка """
    return (-f(x) + f(x + h)) / h - ((1/2) * g(x) + (1/2) * g(x + h))

def second_order_difference_operator_form(x, start_val, h):
    y = []
    y.append(start_val)
    y.append(start_val * (1 + h))
    y.append(2 * h * y[1] + y[0])
    y.append(2 * h * y[2] + y[1])

    for i in range(4, len(x)):
        y.append(y[i - 2] + (h / 3) * (g(x[i] + h) + g(x[i] - h)) + ((4 * h) / 3) * g(x[i]))

    return y

def forth_order_difference_operator_form(x, start_val, h):
    y = []
    y.append(start_val)
    y.append(start_val*(1 + h))
    y.append(2 * h * y[1] + y[0])
    y.append(4.5 * y[2] - 9 * y[1] + (11 / 2) * y[0])
    y.append((39/4)* y[3] - 14 * y[2] + 21/4 * y[1])

    for i in range(5, len(x)):
        y.append(y[i - 2] + (h / 3) * (g(x[i] + h) + g(x[i] - h)) + ((4 * h) / 3) * g(x[i]))

    return y


def forth_order_difference(f, x, h):
    """ Разностный оператор четвёртого порядка """
    return ((-0.5)*f(x - 0.5*h) + (0.5)*f(x + 0.5*h)) / h - ((1/6) * g(x + (1/6)*h) + (2/3) * g(x + (2/3)*h) + (1/6) * g(x + (1/6)*h))


# Параметры
x_min = 0  # минимальное значение x
x_max = 5  # максимальное значение x
num_points = 1001  # количество точек сетки
h = (x_max - x_min) / (num_points - 1) # шаг сетки

# Создаем массив точек сетки
x = np.linspace(x_min, x_max, num_points)
print(x[0])
print(x[-1])
print(h)
print(x[1] - x[0])
y_exact = f(x)  # Значения производной функции

# Создаем фигуру для графиков
plt.figure(figsize=(12, 10))

# Для каждого оператора
operators = [second_order_difference_operator_form, forth_order_difference_operator_form]
operator_names = ['2nd Order Difference', '4th Order Difference']

for i, operator in enumerate(operators):
    y_approx = []
    y_approx = operator(x, 1, h)  # Приближение для каждой точки
    
    plt.subplot(2, 1, i+1)  # 3 строки, 1 колонка, график i+1
    plt.plot(x, y_exact, label="Решение", color='blue')
    plt.plot(x, y_approx, label=f"Приближение по {operator_names[i]}", color='red')
    plt.title(f"График {operator_names[i]}")
    plt.legend()
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True)

plt.tight_layout()  # Для лучшего расположения графиков
plt.show()
