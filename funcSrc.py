import os
import math

def fetch_UDB(keyword, filename="unifiedDB.txt"):
    data = []
    tag = f"$${keyword}$$"
    
    if not os.path.exists(filename) or os.path.getsize(filename) == 0:
        print("No data found in DB.")
        return data

    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if line.startswith(tag):
                # Remove tag part and store only the data
                entry = line.replace(tag, "").strip()
                data.append(entry)
    
    return data

def save_to_UDB(keyword, data, filename="unifiedDB.txt"):
    with open(filename, "a") as f:
        f.write(f"$${keyword}$$ {data}\n")

def f(x, y, expr):
    return eval(expr, {"x": x, "y": y, **math.__dict__})