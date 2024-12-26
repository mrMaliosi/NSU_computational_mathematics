#include <iostream>
#include <vector>

using namespace std;

#define EPS 0.0000000001
#define DELTA 0.01

struct equation {
	vector <double> coef;
	vector <double> ans;
	int n;

	double f(double x) {
		double ans = 0;
		double x_pow = 1;
		for (int i = n - 1; i >= 0; --i) {
			ans += coef[i] * x_pow;
			x_pow *= x;
		}
		return ans;
	}

	void dihotomia(double eps, equation * der_eq) {
		while ((n > 0)&&(coef[0] == 0)) {
			--n;
			for (int i = 0; i < n; ++i){
				coef[i] = coef[i + 1];
			}
		}

		if (n == 3) {
			square_equation_solver();
			return;
		}
		else if (n == 2) {
			ans.push_back(-coef[1]);
			return ;
		}
		else if (n == 2) {
			cout << "Тут даже считать нечего.." << endl;
		}
		else {
			return;
		}

		derivative(n - 1, der_eq);
		der_eq->square_equation_solver();
		double l = -DELTA, r = DELTA;
		double step = DELTA;
		switch(der_eq->ans.size()){
			case 0:
				step = DELTA;
				if (der_eq->f(0) > 0) {
					while ((((f(l) < 0) && (f(r) < 0)))) {
						r += step;
					}

					while ((((f(l) > 0) && (f(r) > 0)))) {
						l -= step;
					}
				}
				else {
					while ((((f(l) < 0) && (f(r) < 0)))) {
						l -= step;
					}

					while ((((f(l) > 0) && (f(r) > 0)))) {
						r += step;
					}
				}
				break;
			case 1:
				if (der_eq->f(der_eq->ans[0] + DELTA) > 0) {
					while ((((f(l) < 0) && (f(r) < 0)))) {
						r += step;
					}

					while ((((f(l) > 0) && (f(r) > 0)))) {
						l -= step;
					}
				}
				else {
					while ((((f(l) < 0) && (f(r) < 0)))) {
						l -= step;
					}

					while ((((f(l) > 0) && (f(r) > 0)))) {
						r += step;
					}
				}	
				break;
			case 2:
				l = der_eq->ans[0];
				r = der_eq->ans[1];
				if (((f(l) >= 0)&&(f(r) <= 0)) || ((f(l) <= 0) && (f(r) >= 0))) {

				}
				else if (der_eq->f(der_eq->ans[1] + DELTA) > 0){
					if (f(der_eq->ans[1]) > 0) {
						while (f(l) > 0) {
							l -= step;
						}

						while (f(r) < 0) {
							r += step;
						}
					}
					else {
						while (f(l) > 0) {
							l -= step;
						}

						while (f(r) < 0) {
							r += step;
						}
					}
				}
				else {
					if (f(der_eq->ans[1]) > 0) {
						while (f(r) > 0) {
							r += step;
						}

						while (f(l) < 0) {
							l -= step;
						}
					}
					else {
						while (f(r) > 0) {
							r += step;
						}

						while (f(l) < 0) {
							l -= step;
						}
					}
				}
				break;
		}

		
		double m;
		while (r - l > eps) {
			m = (l + r) / 2;
			double f_l = f(l);
			double f_r = f(r);
			double f_m = f(m);
			if (f_l == 0){
				m = l;
				break;
			}
			else if (f_r == 0) {
				m = r;
				break;
			}
			else if (((f_l < 0) && (f_m > 0)) || ((f_l > 0) && (f_m < 0))) {
				r = m;
			}
			else {
				l = m;
			}
		}
		ans.push_back(m);
		besu(m);
		square_equation_solver();
	}

	void derivative(int power, equation * derivation_equation) {
		derivation_equation->n = power;
		for (int i = 0; i < power; ++i) {
			derivation_equation->coef.push_back(coef[i] * (power - i));
		}
	}

	void besu(double x) {
		for (int i = 1; i < n - 1; ++i) {
			coef[i] = (coef[i] + x * coef[i - 1]);
		}
		coef[n - 1] = 1;
	}

	void square_equation_solver() {
		double disc = coef[1] * coef[1] - 4 * coef[0] * coef[2];
		if (disc > 0) {
			ans.push_back((-coef[1] - sqrt(disc)) / (2 * coef[0]));
			ans.push_back((-coef[1] + sqrt(disc)) / (2 * coef[0]));
		}
		else if (disc == 0) {
			ans.push_back((-coef[1]) / (2 * coef[0]));
		}
	}

	void print_ans() {
		int l = ans.size();
		if (l == 0) {
			cout << "Решений нет." << endl;
			return;
		}

		for (int i = 0; i < l; ++i) {
			printf("x%d: %lf\n", i + 1, ans[i]);
		}
	}
};

int main() {
	system("chcp 1251 > nul");
	equation q;
	equation q_dir;
	cout << "Введите число коэффициентов уравнения: ";
	cin >> q.n;
	cout << "Введите коэффициенты уравнения: ";
	double x;
	for (int i = 0; i < q.n; ++i) {
		cin >> x;
		q.coef.push_back(x);
	}
	q.dihotomia(EPS, &q_dir);
	q.print_ans();
	return 0;
}