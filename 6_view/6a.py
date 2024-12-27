import numpy as np
import matplotlib.pyplot as plt

# Определим исходную функцию
def f(x):
    return -np.exp(-x)

# Определим производную исходной функции
def df(x):
    return np.exp(-x)

# Разностные операторы
def first_order_difference(f, x, h):
    """ Разность первого порядка (назад) """
    return (f(x) - f(x - h)) / h

def first_order_difference_operator_form(x, start_val, h):
    """Вычисление точек по операторной норме с использованием конечных разностей первого порядка"""
    y = []
    y.append(start_val)

    # Поскольку индекс в Python начинается с 0, начнем с 1, так как y[0] уже задано.
    for i in range(len(x) - 1):  # Пробегаем по всем индексам, кроме последнего
        y.append(y[i] * (1 - h))  # Формула для разности первого порядка

    return y


def second_order_difference_operator_form(x, start_val, h):
    """Вычисление точек по операторной норме с использованием конечных разностей первого порядка"""
    y = []
    y.append(start_val)

    # Поскольку индекс в Python начинается с 0, начнем с 1, так как y[0] уже задано.
    for i in range(len(x) - 1):  # Пробегаем по всем индексам, кроме последнего
        y.append(y[i] * h * (4/(h*(h + 4)) - 1))  # Формула для разности первого порядка

    return y

def forth_order_difference_operator_form(x, start_val, h):
    """Вычисление точек по операторной норме с использованием конечных разностей первого порядка"""
    y = []
    y.append(start_val)
    y.append(h*y[0]*(4/(h*(h + 4)) - 1))
    y.append(h*y[1]*(4/(h*(h + 4)) - 1))
    y.append(1.5 * y[2] - 3 * y[1] + 11/6 * y[0])
    y.append(12 * y[3] - 24 * y[2] + 21 * y[1])
    y.append(25 * y[4] - 75 * y[3] + 51 * y[2])

    # Поскольку индекс в Python начинается с 0, начнем с 1, так как y[0] уже задано.
    for i in range(6, len(x)):  # Пробегаем по всем индексам, кроме последнего
        y.append((1/h) * (8 * y[i - 1] - 8 * y[i - 3] + y[i - 4]))  # Формула для разности первого порядка

    return y
 


def second_order_difference(f, x, h):
    """ Центральная разность второго порядка """
    return (f(x + h) - f(x - h)) / (2 * h)

def forth_order_difference(f, x, h):
    """ Разностный оператор четвёртого порядка """
    return ((1/12) * f(x - 2 * h) - (2 / 3) * f(x - h) + (2 / 3) * f(x + h) - (1 / 12) * f(x + 2 * h)) / (h)

# Параметры

x_min = 0  # минимальное значение x
x_max = 10  # максимальное значение x
num_points = 11  # количество точек сетки (на самом деле это количество интервалов)
h = (x_max - x_min) / num_points

# Создаем массив точек сетки
x = np.linspace(x_min, x_max, num_points)
y = [-1]
y_exact = df(x)  # Значения производной функции

# Создаем фигуру для графиков
plt.figure(figsize=(12, 10))

# Для каждого оператора
operators = [first_order_difference_operator_form, second_order_difference_operator_form, forth_order_difference]
operator_names = ['1st Order Difference', '2nd Order Difference', '4th Order Difference']

for i, operator in enumerate(operators):
    y_approx = []
    if (i == 0):
        y_approx = first_order_difference_operator_form(x, -1, h)
    elif (i == 1):
        y_approx = second_order_difference_operator_form(x, -1, h)
    elif (i == 2):
        y_approx = forth_order_difference_operator_form(x, -1, h)
    else:
        y_approx = np.array([operator(f, xi, h) for xi in x])  # Приближение для каждой точки
    
    #print("Порядок сходимости: " + str(np.log2((operator(f, 0.001, h) - df(0.001))/(operator(f, 0.001, h/2) - df(0.001)))))
    plt.subplot(3, 1, i+1)  # 3 строки, 1 колонка, график i+1
    plt.plot(x, y_exact, label="Производная функции", color='blue')
    plt.plot(x, y_approx, label=f"Приближение по {operator_names[i]}", color='red')
    plt.title(f"График {operator_names[i]}")
    plt.legend()
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True)

plt.tight_layout()  # Для лучшего расположения графиков
plt.show()
