import subprocess
import pandas as pd
import csv
import matplotlib.pyplot as plt
import argparse
import os

# Функция для запуска exe программы
def run_exe(exe_path, csv_file, exe_args):
    command = [exe_path, csv_file]  + exe_args
    print("Executing command:", command)
    subprocess.run(command, shell=True)

def plot_data(csv_file_func, csv_file_newton, csv_file_secant):
    # Чтение данных из CSV файлов с помощью pandas
    data_base = pd.read_csv(csv_file_func)
    data_newton = pd.read_csv(csv_file_newton)
    data_secant = pd.read_csv(csv_file_secant)

    # Построение первого окна: график с исходными точками и точками метода Ньютона
    fig1, ax1 = plt.subplots(figsize=(8, 6))  # Используем subplots для получения ax
    ax1.plot(data_base['x'], data_base['y'], label="Исходная функция", color='blue', alpha=0.5)  # Исходная функция
    ax1.scatter(data_newton['x'], data_newton['y'], color='red', zorder=5, label="Метод Ньютона", linewidth=3)  # Точки Ньютона
    ax1.set_title("График с методами Ньютона")
    ax1.set_xlabel("x")
    ax1.set_ylabel("y")
    ax1.legend()
    ax1.grid(True)

    # Построение второго окна: график с исходными точками и секущими
    fig2, ax2 = plt.subplots(figsize=(8, 6))  # Используем subplots для получения ax
    ax2.plot(data_base['x'], data_base['y'], label="Исходная функция", color='blue', alpha=0.5)  # Исходная функция
    
    # Ограничение координат для рисования секущих
    min_x, min_y = -100, -100  # Минимальные пределы для x и y
    max_x, max_y = 100, 100    # Максимальные пределы для x и y

    # Рисуем все секущие линии между текущими и предыдущими точками
    for i in range(1, len(data_secant)):  # Начнем с 1, чтобы избежать попытки нарисовать секущую с первым элементом
        x_prev = data_secant['x_prev'][i]
        y_prev = data_secant['y_prev'][i]
        x_curr = data_secant['x_curr'][i]
        y_curr = data_secant['y_curr'][i]
        
        # Проверяем, если обе точки секущей внутри заданных лимитов (x, y от -100 до 100)
        if (min_x <= x_prev <= max_x and min_y <= y_prev <= max_y) and (min_x <= x_curr <= max_x and min_y <= y_curr <= max_y):
            ax2.plot([x_prev, x_curr], [y_prev, y_curr], color='green', linestyle='--', label="Секущая" if i == 1 else "")

    ax2.set_title("График с секущими")
    ax2.set_xlabel("x")
    ax2.set_ylabel("y")
    ax2.legend()
    ax2.grid(True)

    # Показываем оба окна
    plt.show()

# Основная функция
def main():
    parser = argparse.ArgumentParser(description='Запуск exe программы и построение графика.')
    parser.add_argument('exe_path', type=str, help='Путь к exe программе')
    parser.add_argument('csv_file', type=str, help='Путь к CSV файлу')
    parser.add_argument('exe_args', nargs=argparse.REMAINDER, help='Аргументы для exe программы')
    
    args = parser.parse_args()
    
    run_exe(args.exe_path, args.csv_file, args.exe_args)
    
    csv_file_base = os.path.splitext(args.csv_file)[0]
    print(csv_file_base)
    for i in range(0, 2):
        csv_file_func = f"{csv_file_base}_base_{i}.csv"
        csv_file_newton = f"{csv_file_base}_newton_{i}.csv"
        csv_file_secant = f"{csv_file_base}_secant_{i}.csv"
        plot_data(csv_file_func, csv_file_newton, csv_file_secant)
        
    plt.show()

if __name__ == '__main__':
    main()
