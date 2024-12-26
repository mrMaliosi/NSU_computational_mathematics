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
def plot_data(csv_file, points_number):
    data = pd.read_csv(csv_file)
    fig = plt.figure()
    fig.canvas.manager.set_window_title("Interpolation points number: " + str(points_number))
    plt.plot(data['x'], data['yi'], label='Исходная функция')
    plt.plot(data['x'], data['pi'], label='Интерполированная функция')
    plt.xlabel('x')
    plt.ylabel('yi / pi')
    plt.title('График функции и интерполяционного полинома')
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
    interation_points_number = int(args.exe_args[1])
    for i in range(int(args.exe_args[2])):
        csv_file = f"{csv_file_base}_{i}.csv"
        plot_data(csv_file, interation_points_number)
        interation_points_number += int(args.exe_args[3])
        
    plt.show()

if __name__ == '__main__':
    main()
