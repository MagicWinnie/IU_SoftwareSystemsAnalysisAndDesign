import random

output = ''
DEVIATION = 0.1
M = 32
N = 8

coeffs = [random.random() * 10 - 5 for _ in range(N + 1)]
F = lambda x: sum(coeffs[i] * x ** i for i in range(N + 1))

points = []
for _ in range(M):
    x = random.random() * random.random() * 2 - 1
    y = F(x)
    points.append((x, y))
max_x = max(points, key=lambda x: abs(x[0]))[0]
max_y = max(points, key=lambda x: abs(x[1]))[1]

output += f"{M}\n"
for i in range(M):
    x = points[i][0] + random.random() * max_x * DEVIATION - max_x * DEVIATION / 2
    y = points[i][1] + random.random() * max_y * DEVIATION - max_y * DEVIATION / 2
    output += f"{round(x, 2)} {round(y, 2)}\n"
output += f"{N}"

with open("points.txt", "w") as outfile:
    outfile.write(output)
