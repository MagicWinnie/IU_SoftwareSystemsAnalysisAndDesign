import random

a = random.random() * 2 - 1
b = random.random() * 2 - 1
c = random.random() * 2 - 1
F = lambda x: a * x * x + b * c

output = ''
M = 32
N = 2
output += f"{M}\n"
for _ in range(M):
    x = random.random() * 10 - 5
    y = F(x) + random.random() * 6 - 3
    output += f"{round(x, 2)} {round(y, 2)}\n"
output += f"{N}"

with open("points.txt", "w") as outfile:
    outfile.write(output)
