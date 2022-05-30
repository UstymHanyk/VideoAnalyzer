from re import T
import os
import subprocess
import matplotlib.pyplot as plt

num = 1
results = []
ns = []
min = -1
min_n = 0

file = "joker_trailer.mp4"
f_threads = num
m_threads = 1
show = 0
save = 0
for i in range(10):
    ns.append(num)
    p = subprocess.Popen(["x64\Release\OCVT.exe", str(f_threads), str(m_threads), file, str(save), str(show)], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    num += 1
    f_threads = num

    if num % 4 == 0:
        m_threads += 1

    out, err = p.communicate()
    out = out.decode()

    try:
        n = int(out.split('\n')[0].split(": ")[1])
    except ValueError:
        print("Error occured")
        print(out)
        exit()
    if (min == -1):
        min = n
        min_n = num-2
    elif (min > n):
        min = n
        min_n = num-2
    
    results.append(n)

print("Minimal time: ", min)
print("Number of threads for minimal time", min_n)

plt.plot(ns, results)
plt.xlabel('number of threads')
plt.ylabel('time in microseconds')
plt.title('graph')
plt.show()


