# Euler’s Method & RK4 Method for Ordinary Differential Equation Calculator and Error Rate Simulator

## Euler's Method

$$
y_{n+1} = y_n + h \cdot f(x_n, y_n)
$$

---

## Modified Euler (Heun's Method)

$$
k_1 = f(x_n, y_n)
$$

$$
y^* = y_n + h \cdot k_1
$$

$$
k_2 = f(x_n + h, y^*)
$$

$$
y_{n+1} = y_n + \frac{h}{2} \cdot (k_1 + k_2)
$$

---

## Runge-Kutta 4th Order (RK4)

$$
k_1 = f(x_n, y_n)
$$
$$
k_2 = f\left(x_n + \frac{h}{2}, y_n + \frac{h}{2} k_1\right)
$$
$$
k_3 = f\left(x_n + \frac{h}{2}, y_n + \frac{h}{2} k_2\right)
$$
$$
k_4 = f(x_n + h, y_n + h \cdot k_3)
$$
$$
y_{n+1} = y_n + \frac{h}{6} \cdot (k_1 + 2k_2 + 2k_3 + k_4)
$$
