import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import argparse
import os

# Функция для запуска exe программы
def run_exe(exe_path, csv_file, exe_args):
    command = [exe_path, csv_file]  + exe_args
    print("Executing command:", command)
    subprocess.run(command, shell=True)

# Функция для чтения данных из CSV файла и построения графика
def plot_data(csv_file_func, csv_file_trapezoidal, csv_file_simpson, points_number):
    data_base = pd.read_csv(csv_file_func)
    data_trapezoidal = pd.read_csv(csv_file_trapezoidal)
    data_simpson = pd.read_csv(csv_file_simpson)
    fig = plt.figure()
    fig.canvas.manager.set_window_title("Interpolation points number: " + str(points_number))
    plt.plot(data_base['x'], data_base['y'], alpha=0.5, label='f(x)')
    plt.plot(data_trapezoidal['x'], data_trapezoidal['y'], label='Интерполированная трапецией')
    plt.plot(data_trapezoidal['x'], data_trapezoidal['y'], 'o', markersize=5)
    plt.plot(data_simpson['x'], data_simpson['y'], label='Интерполированная Симпсоном')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.vlines(data_trapezoidal['x'], 0, data_trapezoidal['y'], colors='gray', linestyles='dashed')
    plt.vlines(data_simpson['x'], 0, data_simpson['y'], colors='orange', linestyles='dashed')
    plt.title('Графики первообразной и методов численного интегрирования')
    plt.legend()
    plt.grid(True)

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
    interation_points_number = int(args.exe_args[1])
    for i in range(int(args.exe_args[2])):
        csv_file_func = f"{csv_file_base}_base.csv"
        csv_file_trapezoidal = f"{csv_file_base}_trapezoidal{i}.csv"
        csv_file_simpson = f"{csv_file_base}_simpson{i}.csv"
        plot_data(csv_file_func, csv_file_trapezoidal, csv_file_simpson, interation_points_number)
        interation_points_number += int(args.exe_args[3])
        
    plt.show()

if __name__ == '__main__':
    main()
