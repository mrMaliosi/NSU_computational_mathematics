import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider

# Параметры
L = 5.0  # Длина области
Nx = 500  # Число узлов
dx = L / Nx  # Шаг по пространству
dt = 0.01  # Шаг по времени
T = 1.0  # Время моделирования
a = 1.0  # Скорость переноса
b = 0.5  # Скорость для второй части области

# Параметры для функции f(u)
c = 1.0  # Константа для f(u) = cu

def f1(u):
    return c * u

def f2(u):
    return 0.5 * u**2

# Сетка по пространству
x = np.linspace(0, 5, Nx, endpoint=False)

# Начальное условие
# Начальное условие с использованием условия по частям
def initial_condition(x):
    if 0 <= x < 1:
        return 0
    elif 1 <= x < 4:
        return np.sin(np.pi * (x - 1) / 3)
    elif 4 <= x <= 5:
        return 0
    return 0  # Для всех других значений x (если вдруг выходим за пределы)

u_initial = np.array([initial_condition(xi) for xi in x])

# Функция для применения схемы Лакса-Вендорфа
def lax_wendroff(u, a, dx, dt, f):
    u_new = np.zeros_like(u)
    
    # Внутренние узлы
    for i in range(1, len(u) - 1):
        u_new[i] = u[i] - a * dt / (2 * dx) * (u[i+1] - u[i-1]) + 0.5 * (a * dt / dx)**2 * (u[i+1] - 2*u[i] + u[i-1]) + dt * f(u[i])
    
    # Граничные условия: периодические
    u_new[0] = u_new[-1]
    
    # Граничные условия: фиксируем значения на границах
    #u_new[0] = 0
    #u_new[-1] = 0
    
    return u_new

# Функция для обновления графиков с ползунком
def update(val):
    t_idx = int(val / dt)
    
    # Решение для f(u) = cu
    u_temp1 = u_initial.copy()
    for t in range(t_idx):
        u_temp1 = lax_wendroff(u_temp1, a, dx, dt, f1)
    
    # Решение для f(u) = u^2 / 2
    u_temp2 = u_initial.copy()
    for t in range(t_idx):
        u_temp2 = lax_wendroff(u_temp2, a, dx, dt, f2)
    
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
line2, = ax2.plot(x, u_initial, label='Решение для f(u) = u^2 / 2', color='red')

# Настройки для графиков
ax1.set_xlabel('x')
ax1.set_ylabel('u(x, t)')
ax1.set_title('Решение для f(u) = cu')
ax1.grid(True)
ax1.legend()

ax2.set_xlabel('x')
ax2.set_ylabel('u(x, t)')
ax2.set_title('Решение для f(u) = u^2 / 2')
ax2.grid(True)
ax2.legend()

# Добавляем текст для времени
time_text = ax2.text(0.8, 0.9, f't = 0.00', transform=ax2.transAxes)

# Добавляем ползунок
ax_slider = plt.axes([0.1, 0.01, 0.8, 0.03], facecolor='lightgoldenrodyellow')
slider = Slider(ax_slider, 'Time', 0.0, T, valinit=0, valstep=dt)
slider.on_changed(update)

plt.show()
