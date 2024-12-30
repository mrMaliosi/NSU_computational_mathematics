import numpy as np
import math
import matplotlib.pyplot as plt

# Определим исходную функцию
def f(x):
    return -np.exp(-x)

# Определим производную исходной функции
def df(x):
    return np.exp(-x)

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
    y.append(y[0] * (1 - h)) 

    # Поскольку индекс в Python начинается с 0, начнем с 1, так как y[0] уже задано.
    for i in range(1, len(x) - 1):  # Пробегаем по всем индексам, кроме последнего
        y.append(y[i - 1] - 2 * h * y[i])  # Формула для разности первого порядка

    return y

def forth_order_difference_operator_form(x, start_val, h):
    """Вычисление точек по операторной норме с использованием конечных разностей первого порядка"""
    y = []
    y.append(start_val)
    y.append((h**2 - 6)/(2*h**2 + 6 * h +6))
    """
    y.append(start_val*(1 - h))
    y.append(-2 * h * y[1] + y[0])
    y.append((3 / (1 + 3 * h)) * (11/6 * y[0] - 3 * y[1] + 1.5 * y[2]))
    y.append((3 / (2 - 3 * h)) * ((7/2) * y[1] - (28/3)*y[2] + 6.5 * y[3]))
    """
    
    for i in range(2, len(x)):  # Пробегаем по всем индексам, кроме последнего
        y.append((3/(3 + h)) * ((-4/3) * h * y[i - 1] + (1 - h / 3) * y[i - 2]))

    return y


#y.append((3 / (1 + 3 * h)) * (11/6 * y[2] - 3 * y[3] + 1.5 * y[4]))
    #y.append((3 / (2 - 3 * h)) * ((7/2) * y[3] - (28/3)*y[4] + 6.5 * y[5]))
    #y.append((3 / (1 + 3 * h)) * (11/6 * y[0] - 3 * (start_val*(1 - h)) + 1.5 * (-2 * h * (start_val*(1 - h)) + y[0])))


# Параметры
x_min = 0  # минимальное значение x
x_max = 1  # максимальное значение x
num_points = 201  # количество точек сетки (на самом деле это количество интервалов)
h = (x_max - x_min) / (num_points - 1)
h_runge = (x_max - x_min) / (2*(num_points-1))
print(h)
print(h_runge)

# Создаем массив точек сетки
x = np.linspace(x_min, x_max, num_points)
x_runge = np.linspace(x_min, x_max, 2*(num_points-1) + 1)
y = [-1]
y_exact = f(x)  # Значения производной функции

# Создаем фигуру для графиков
plt.figure(figsize=(12, 10))

# Для каждого оператора
operators = [first_order_difference_operator_form, second_order_difference_operator_form, forth_order_difference_operator_form]
operator_names = ['1st Order Difference', '2nd Order Difference', '4th Order Difference']

for i, operator in enumerate(operators):
    y_approx = operator(x, -1, h)
    y_runge = operator(x_runge, -1, h_runge)
    if (i == 0):
        print("Runge error on " + str(i + 1) + ":")
        #print(x_runge[81])
        #print((x[41] + x[40])/2)
        #error_runge = (y_approx[::2] - y_runge) / (2**(i+1) - 1)
        #print((2 / (2 - 1)) * (((y_approx[41] + y_approx[40])/2 + y_approx[40]) / 2 - (y_runge[81] + y_runge[80])/2))
    elif(i == 1):
        print("Runge error on " + str(i + 1) + ":")
        #print((x_runge[81] + x_runge[80])/2)
        #print(((x[41] + x[40])/2 + x[40])/2)
        #error_runge = (y_approx[::2] - y_runge) / (2**(i+1) - 1)
        #print((2 * 2 / (2 * 2 - 1)) * (((y_approx[41] + y_approx[40])/2 + y_approx[40]) / 2 - (y_runge[81] + y_runge[80])/2))
    else:
        print("Runge error on " + str(i + 2) + ":")
        #error_runge = (y_approx[::2] - y_runge) / (2**(i+2) - 1)
        #print((2 * 2 * 2 * 2 / (2 * 2 * 2 * 2 - 1)) * (((y_approx[41] + y_approx[40])/2 + y_approx[40]) / 2 - (y_runge[81] + y_runge[80])/2))
    
    
    print(math.log2((y_approx[10] - f(x[10]))/(y_runge[20] - f(x[10]))))
    #print(f"Runge error on {operator_names[i]}: {error_runge[40]}")  # Пример для индекса 40
    
    #print("Порядок сходимости: " + str(np.log2((operator(f, 0.001, h) - df(0.001))/(operator(f, 0.001, h/2) - df(0.001)))))
    plt.subplot(3, 1, i+1)  # 3 строки, 1 колонка, график i+1
    plt.plot(x, y_exact, label="Решение", color='blue')
    plt.plot(x, y_approx, label=f"Приближение по {operator_names[i]}", color='red')
    plt.title(f"График {operator_names[i]}")
    plt.legend()
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(True)

plt.tight_layout()  # Для лучшего расположения графиков
plt.show()
