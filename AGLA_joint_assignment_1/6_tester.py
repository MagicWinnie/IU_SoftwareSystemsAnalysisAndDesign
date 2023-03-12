import subprocess
import numpy as np
from tqdm import tqdm

for N in tqdm(range(1, 13)):
    for _ in tqdm(range(500)):
        A = np.random.randint(-100, 100, (N, N))
        b = np.random.randint(-100, 100, N)
        inp = f'{N}\n'
        inp += '\n'.join(' '.join(str(x) for x in y) for y in A)
        inp += f'\n{N}\n'
        inp += ' '.join(str(y) for y in b)
        try:
            solution = np.linalg.solve(A, b)
        except np.linalg.LinAlgError:
            # singular
            # tqdm.write(f"[ERROR]\ninput:\n{inp}\nsolution:\n{solution}")
            continue
        result = subprocess.run(
            ['6.exe'],
            input=inp.encode(),
            stdout=subprocess.PIPE
        )
        output = result.stdout.decode().split()
        output = list(map(float, output))
        for i in range(len(output)):
            if abs(solution[i] - output[i]) > 1e-4:
                tqdm.write(f"[WRONG]\ninput:\n{inp}\noutput:\n{output}\ncorrect:\n{solution}")
                break
