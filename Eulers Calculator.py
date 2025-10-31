import math
from tabulate import tabulate
import funcSrc

# Calculator
def eulerMethod(func, y0, h, ll, ul):
    xVal = [ll]
    yVal = [y0]
    x = ll

    while True:
        # yn = yn-1 + h * f(xn-1, yn-1)
        y = yVal[-1] + h * func(xVal[-1],yVal[-1])
        x += h
        xVal.append(x)
        yVal.append(y)
        if x >= ul: # Chatgpt suggested cuz == might lead to error in case of float as 0.1 + 0.2 = 0.3
            break

    return xVal, yVal 

# Functions
def func(x, y):
    return eval(expr, {"x": x, "y": y, **math.__dict__})

def output(xVal,yVal):
    table = []

    for i in range(len(xVal)):
        table.append([round(xVal[i], 4), round(yVal[i], 4)])
    
    print(tabulate(table, headers=["x", "y"], tablefmt="fancy_grid"))


# Main
# if not os.path.exists("functiondb.txt") or os.path.getsize("functiondb.txt") == 0:
tempData = funcSrc.fetch_UDB("EULERCALC")
if len(tempData) == 0:
    expr = input(">> Enter function f(x, y): ")   # x + y | x**2 - y
else:
    op = input("<< Use the previous function? [y/N]: ")
    if op in ("y","Y"):
        expr = tempData[-1].strip()
    else:
        expr = input(">> Enter function f(x, y): ")

y0 = float(input(">> Enter y0: -"))
h = float(input(">> Enter step size h: "))
ll, ul = map(float, input(">> Enter Limits (lower upper): ").split())

xVal, yVal = eulerMethod(func, y0, h, ll, ul)
output(xVal, yVal)

funcSrc.save_to_UDB("EULERCALC",expr)

