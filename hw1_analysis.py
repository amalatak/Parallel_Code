import numpy as np
import matplotlib.pyplot as plt
# this file is for the analysis and plotting of homework 1
#
# the location folder will need to be changed depending on the computer being used
# it simply needs to be changed to the folder location of the analyzed files
location = "/home/amalatak/Documents/University/Sp19/Parallel_Computing/Homework/HW1"
filename = "HW1_part2a.txt"

f = open(filename)

data = []
while True:
    x = f.readline()
    if "CPU Smooth" in x:
        data = x
        break

ncores = np.array([1, 2, 4, 8, 16, 32, 64, 128, 256])
data = data[24:-1]
data = np.array(data.split())
data = data.astype(np.float)

time_on_one_proc = data[1]
speedup_actual = np.empty(len(data))
speedup_theoretical = ncores

for i in range(len(speedup_actual)):
    speedup_actual[i] = time_on_one_proc/data[i]

plt.figure(1)
plt.plot(ncores, speedup_actual)
plt.title('Parallel Execution Speedup')
plt.ylabel('Speedup')
plt.xlabel('Number of Cores')
plt.savefig(location + "/plots/Scaling.jpg")

plt.figure(2)
plt.plot(ncores, speedup_actual)
plt.plot(ncores, speedup_theoretical)
plt.legend(["actual", "ideal"])
plt.title('Parallel Execution Speedup')
plt.ylabel('Speedup')
plt.xlabel('Number of Cores')
plt.savefig(location + "/plots/Scaling_Comparison.jpg")

# ------------------------- Part 2 ------------------------------ #

