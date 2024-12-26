#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <functional>
#include <string>

#define SCALE 0.01

typedef unsigned int ui;

struct section {
	double x0;
	double x1;
};

std::vector<std::function<double(double)>> functions;
std::vector<std::function<double(double, double)>> functions_der;
std::vector<section> sections{ {-2, 2}, {-0.585, 0.985} };

// Исходные функция
double f0(double x) {
	return std::abs(x);
}

double f0_der(double x, int n) {
	if (n == 0) {
		return x;
	}
	else if (n == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

double f1(double x) {
	return std::tan(0.5*x + 0.2) - x*x;
}

double f1_der(double x, int n) {
	std::vector <std::vector <double> > coef(n + 1);
	coef[0].push_back(0);
	coef[0].push_back(1);
	int j = 2;
	while (j <= n + 1) {
		coef[0].push_back(0);
		++j;
	}

	for (int i = 1; i <= n; ++i) {
		int m = n;
		coef[i].push_back(0 + 1 * coef[i - 1][1]);
		for (j = 1; j <= m; ++j) {
			coef[i].push_back((j - 1)*coef[i - 1][j - 1] + (j + 1)*coef[i - 1][j + 1]);
		}

		coef[i].push_back(0);
	}

	double ans = 0;
	double x_pow = 1;
	for (int i = 0; i <= n; ++i) {
		ans += coef[n][i] * x_pow;
		x_pow *= tan(x);
	}
	return ans;
}


// Интерполяционный полином Лагранжа
std::vector<double> lagrangeInterpolation(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& xi) {
	int n = x.size();
	std::vector<double> L(xi.size(), 0.0);
	for (int i = 0; i < n; ++i) {
		std::vector<double> li(xi.size(), 1.0);
		for (int j = 0; j < n; ++j) {
			if (i != j) {
				for (size_t k = 0; k < xi.size(); ++k) {
					li[k] *= (xi[k] - x[j]) / (x[i] - x[j]);
				}
			}
		}
		for (size_t k = 0; k < xi.size(); ++k) {
			L[k] += li[k] * y[i];
		}
	}
	return L;
}

// Оценка погрешности
void errorEstimation(std::function<double(double)> f, std::function<double(double, double)> f_der, const std::vector<double>& p, const std::vector<double>& x, const std::vector<double>& xi, int n, double step_size) {
	std::vector<double> errors(x.size());
	std::vector<double> errors_dec(xi.size());
	for (size_t i = 0; i < x.size(); ++i) {
		errors[i] = std::abs(f(x[i]) - p[i]);
	}

	for (size_t i = 0; i < xi.size(); ++i) {
		errors_dec[i] = f_der(xi[i], n + 1);
	}

	double h = step_size;
	for (int i = 1; i < n; ++i) {
		h *= step_size;
	}
	double maxError = *std::max_element(errors.begin(), errors.end());
	double maxError2 = *std::max_element(errors_dec.begin(), errors_dec.end());
	std::cout << "------------------------------------------------------------------------------" << std::endl;
	std::cout << "Количество точек: " << n << std::endl;
	std::cout << "Максимальная погрешность: " << maxError << std::endl;
	std::cout << "Максимальная погрешность согласно методичке: " << (maxError2/(n + 1)) * h << std::endl;
}

std::string add_file_numeration(std::string file_name, int number) {
	size_t string_length = file_name.size();
	size_t pos = file_name.rfind(".");
	std::string format_string;
	for (int i = pos; i < string_length; ++i) {
		format_string += file_name[i];
	}
	file_name.erase(pos);
	file_name = file_name + "_" + std::to_string(number) + format_string;
	return file_name;
}

int main(int argc, char * argv[]) {
	functions.push_back(f0);
	functions.push_back(f1);
	functions_der.push_back(f0_der);
	functions_der.push_back(f1_der);
	system("chcp 1251 > nul");
	if (argc != 6) {
		std::cerr << "Usage: " << argv[0] << " <output_file> <test_case> <zero_points_value> <iteration_number> <points_number>" << std::endl;
		return 1;
	}
	std::string output_file = argv[1];
	int test_case = std::atoi(argv[2]);
	int zero_points_value = std::atoi(argv[3]);
	int iteration_number = std::atoi(argv[4]);
	int steps_number = std::atoi(argv[5]);
	std::function<double(double)> f = functions[test_case];
	std::function<double(double, double)> f_der = functions_der[test_case];

	for (int o = 0; o < iteration_number; ++o) {
		// Точки для интерполяции
		std::vector<double> x(zero_points_value);
		std::vector<double> y(zero_points_value);
		double x0 = sections[test_case].x0;
		double x1 = sections[test_case].x1;

		double step_size = (x1 - x0) / (zero_points_value - 1);
		double x_point = x0;
		for (size_t i = 0; i < zero_points_value; ++i) {
			x[i] = x_point;
			x_point += step_size;
		}
		std::transform(x.begin(), x.end(), y.begin(), f);

		// Точки для построения графика
		std::vector<double> xi;
		for (double i = x0; i <= x1; i += SCALE) {
			xi.push_back(i);
		}
		std::vector<double> yi(xi.size());
		std::transform(xi.begin(), xi.end(), yi.begin(), f);
		std::vector<double> pi = lagrangeInterpolation(x, y, xi);

		// Запись данных в файл для построения графика
		std::string output_file_numeric = add_file_numeration(output_file, o);
		std::ofstream outFile(output_file_numeric);
		outFile << "x,yi,pi\n";
		for (size_t i = 0; i < xi.size(); ++i) {
			outFile << xi[i] << "," << yi[i] << "," << pi[i] << "\n";
		}
		outFile.close();

		// Анализ сходимости
		errorEstimation(f, f_der, pi, xi, x, zero_points_value, step_size);

		x.clear();
		y.clear();
		xi.clear();
		yi.clear();
		pi.clear();
		zero_points_value += steps_number;
	}
	

	return 0;
}
