import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
from scipy.linalg import solve

# Параметры
L = 2.0  # Длина области
Nx = 201  # Число узлов
dx = L / (Nx - 1)  # Шаг по пространству
print(dx)
dt = 0.01  # Шаг по времени
T = 2.0  # Время моделирования
a = 1.0  # Скорость переноса
b = 0.5  # Скорость для второй части области

# Параметры для функции f(u)
c = 1.0  # Константа для f(u) = cu

def f1(u):
    return c * u

# Сетка по пространству
x = np.linspace(-1, 1, Nx)
print(x[-1])
# Начальное условие
u_initial = np.where(x < 0, a, b)

# Функция для применения схемы Лакса-Вендорфа
def lax_wendroff(u, a, dx, dt, f):
    u_new = np.zeros_like(u)
    
    # Внутренние узлы
    for i in range(1, len(u) - 1):
        u_new[i] = u[i] - a * dt / (2 * dx) * (u[i+1] - u[i-1]) + 0.5 * (a * dt / dx)**2 * (u[i+1] - 2*u[i] + u[i-1])
    
    # Граничные условия: периодические
    #u_new[0] = u_new[-1]
    
    # Граничные условия: фиксируем значения на границах
    u_new[0] = a
    u_new[-1] = b
    
    return u_new


"""
    Неявная схема с центральной разностью для решения уравнения переноса u + f(u) = 0

    Parameters:
    u : numpy.ndarray
        Вектор значений функции на текущем шаге (t_n)
    v : float
        Скорость переноса
    dx : float
        Шаг по пространству
    dt : float
        Шаг по времени
    t_idx : int
        Число шагов по времени

    Returns:
    u_new : numpy.ndarray
        Вектор значений функции на следующем шаге (t_{n+1})
"""
def implicit_central_diff(u, v, dx, dt, t_idx):
    # Количество точек в пространстве
    N = len(u)

    # Формируем матрицу системы
    coef = np.diag(np.ones(N), 0) + np.diag(np.ones(N-1) * ((-v * dt) / (2 * dx)), -1) + np.diag(np.ones(N-1) * ((v * dt) / (2 * dx)), 1)

    u_new = np.copy(u)
    
    for t in range(t_idx):
        u_new[0] -= a
        u_new[-1] -= b
        u_new = solve(coef, u_new)
        

    return u_new

    """
    # Обычные коэффициенты центральной разности
    alpha = 0
    beta = 0  # Коэффициент для главной диагонали (в неявной схеме)
    gamma = -alpha         # Коэффициенты для соседних диагоналей (слева и справа)

    # Формируем матрицу системы
    A = np.diag(np.ones(N-2) * (1 + alpha**2), 0) + np.diag(np.ones(N-3) * -alpha, -1) + np.diag(np.ones(N-3) * alpha, 1)

    # Функция нелинейности
    u_new = np.copy(u)  # Начинаем с текущего значения для следующего шага
    rhs = np.copy(u)    # Правая часть уравнения для шага

    # Решаем систему линейных уравнений
    u_new[1:-1] = solve(A, rhs[1:-1])  # Решаем для внутренних точек

    # Вставляем граничные условия (например, для простоты, как если бы они были нулевыми)
    u_new[0] = a  # Левое граничное условие (например, u(0) = u(1))
    u_new[-1] = b  # Правое граничное условие (например, u(N) = u(N-1))
    """

"""
def implicit_central_diff(u, dx, dt):
    N = len(u)  # Количество точек в сетке
    v = np.zeros(N)  # Решение на следующем шаге

    alpha = dt / (2 * dx)  # Коэффициент для центральной разности

    # Прогоночные коэффициенты
    a = -alpha * np.ones(N - 1)  # Коэффициенты для u_{i-1}
    b = (1 + 2 * alpha) * np.ones(N)  # Коэффициенты для u_i
    c = -alpha * np.ones(N - 1)  # Коэффициенты для u_{i+1}

    # Вектор правой части (в данном случае f(x) = 0, но может быть изменен)
    rhs = np.copy(u)
    
    # Метод прогонки
    # Прямой ход
    for i in range(1, N):
        m = a[i-1] / b[i-1]
        b[i] = b[i] - m * c[i-1]
        rhs[i] = rhs[i] - m * rhs[i-1]

    # Обратный ход
    v[-1] = rhs[-1] / b[-1]
    for i in range(N - 2, -1, -1):
        v[i] = (rhs[i] - c[i] * v[i+1]) / b[i]

    return v
"""



# Функция для обновления графиков с ползунком
def update(val):
    t_idx = int(val / dt)
    
    # Решение для f(u) = cu
    # Через Лакса-Вендорфа
    u_temp1 = u_initial.copy()
    for t in range(t_idx):
        u_temp1 = lax_wendroff(u_temp1, a, dx, dt, f1)
    
    # Через неявную схему с центральной разностью
    u_temp2 = u_initial.copy()
    u_temp2 = implicit_central_diff(u_temp2, a, dx, dt, t_idx)
    
    # Обновляем графики
    line1.set_ydata(u_temp1)
    line2.set_ydata(u_temp2)
    
    # Обновляем оси y
    ax1.set_ylim(np.min(u_temp1) - 0.1, np.max(u_temp1) + 0.1)
    ax2.set_ylim(np.min(u_temp2) - 0.1, np.max(u_temp2) + 0.1)
    
    # Обновляем текст с текущим временем
    time_text.set_text(f"t = {val:.2f}")
    fig.canvas.draw_idle()

# Создание фигуры и осей
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6))
plt.subplots_adjust(hspace=0.4, bottom=0.25)  # hspace для отступа между графиками

# Графики для обоих решений
line1, = ax1.plot(x, u_initial, label='Решение для f(u) = cu', color='blue')
line2, = ax2.plot(x, u_initial, label='Решение для f(u) = cu центральное разностное', color='red')

# Настройки для графиков
ax1.set_xlabel('x')
ax1.set_ylabel('u(x, t)')
ax1.set_title('Решение для f(u) = cu')
ax1.grid(True)
ax1.legend()

ax2.set_xlabel('x')
ax2.set_ylabel('u(x, t)')
ax2.set_title('Решение для f(u) = cu через неявную схему с центральной разностью')
ax2.grid(True)
ax2.legend()

# Добавляем текст для времени
time_text = ax2.text(0.8, 0.9, f't = 0.00', transform=ax2.transAxes)

# Добавляем ползунок
ax_slider = plt.axes([0.1, 0.01, 0.8, 0.03], facecolor='lightgoldenrodyellow')
slider = Slider(ax_slider, 'Time', 0.0, T, valinit=0, valstep=dt)
slider.on_changed(update)

plt.show()
