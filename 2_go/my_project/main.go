package main

import (
	"fmt"
	"math"

	"gonum.org/v1/plot"
	"gonum.org/v1/plot/plotter"
	"gonum.org/v1/plot/plotutil"
	"gonum.org/v1/plot/vg"
)

// Исходная функция
func f(x float64) float64 {
	return math.Abs(x)
}

// Интерполяционный полином Лагранжа
func lagrangeInterpolation(x, y, xi []float64) []float64 {
	n := len(x)
	L := make([]float64, len(xi))
	for i := 0; i < n; i++ {
		li := make([]float64, len(xi))
		for j := range li {
			li[j] = 1
		}
		for j := 0; j < n; j++ {
			if i != j {
				for k := range li {
					li[k] *= (xi[k] - x[j]) / (x[i] - x[j])
				}
			}
		}
		for k := range L {
			L[k] += y[i] * li[k]
		}
	}
	return L
}

// Оценка погрешности
func errorEstimation(f func(float64) float64, p []float64, x []float64) []float64 {
	errors := make([]float64, len(x))
	for i := range x {
		errors[i] = math.Abs(f(x[i]) - p[i])
	}
	return errors
}

func main() {
	// Точки для интерполяции
	x := []float64{-2, -1, 0, 1, 2}
	y := make([]float64, len(x))
	for i := range x {
		y[i] = f(x[i])
	}

	// Точки для построения графика
	xi := make([]float64, 100)
	for i := range xi {
		xi[i] = -2 + 4*float64(i)/99
	}
	yi := make([]float64, len(xi))
	for i := range xi {
		yi[i] = f(xi[i])
	}
	pi := lagrangeInterpolation(x, y, xi)

	// Построение графика
	p := plot.New()
	p.Title.Text = "Интерполяция функции |x| на интервале [-2, 2]"
	p.X.Label.Text = "x"
	p.Y.Label.Text = "f(x)"
	err := plotutil.AddLinePoints(p,
		"Исходная функция |x|", createPoints(xi, yi),
		"Интерполяционный полином Лагранжа", createPoints(xi, pi))
	if err != nil {
		panic(err)
	}
	err = plotutil.AddScatters(p, "Точки интерполяции", createPoints(x, y))
	if err != nil {
		panic(err)
	}
	if err := p.Save(6*vg.Inch, 6*vg.Inch, "plot.png"); err != nil {
		panic(err)
	}

	// Анализ сходимости
	errors := errorEstimation(f, pi, xi)
	maxError := 0.0
	for _, e := range errors {
		if e > maxError {
			maxError = e
		}
	}
	fmt.Printf("Максимальная погрешность: %v\n", maxError)
}

func createPoints(x, y []float64) plotter.XYs {
	pts := make(plotter.XYs, len(x))
	for i := range x {
		pts[i].X = x[i]
		pts[i].Y = y[i]
	}
	return pts
}
