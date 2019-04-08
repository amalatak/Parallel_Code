import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

# ------ Part 2 A1 ----- #

filename = "hw1_pt2_a.txt"

f = open(filename)


static100 = np.array([])
dynamic100 = np.array([])
static = np.array([])
dynamic = np.array([])

a = 0
startflag = 0

while True:
    x = f.readline()
    x = x.rstrip()

    if x == '':
        break
    if "STATIC," in x and startflag == 0:
        startflag = 1
        continue
    if "dynamic," in x and startflag == 1:
        startflag = 2
        continue
    if "STATIC" in x:
        startflag = 3
        continue
    if "dynamic,1000" in x and startflag == 3:
        startflag = 4
        continue
    if "prb_" in x:
        continue

    if startflag == 1:
        static100 = np.append(static100, x)
    if startflag == 2:
        dynamic100 = np.append(dynamic100, x)
    if startflag == 3:
        static = np.append(static, x)
    if startflag == 4:
        dynamic = np.append(dynamic, x)

static100 = static100.astype(np.float)
dynamic100 = dynamic100.astype(np.float)
static = static.astype(np.float)
dynamic = dynamic.astype(np.float)

static_no_init = static

ncores = range(len(static))


plt.figure()
plt.plot(ncores, static100)
plt.plot(ncores, dynamic100)
plt.plot(ncores, static)
plt.plot(ncores, dynamic)
plt.xlabel('Cores')
plt.ylabel('Time (s)')
plt.legend(['Static, 100', 'Dynamic, 100', 'Static', 'Dynamic, 100000'])
plt.title('Scheduling Effects on Execution Time')
plt.savefig('plots/Scheduling_Effects.jpg')

# ------ Part 2 B ----- #

filename = "hw1_pt2_a2.txt"

f = open(filename)


static100 = np.array([])
dynamic100 = np.array([])
static = np.array([])
dynamic = np.array([])

a = 0
startflag = 0

while True:
    x = f.readline()
    x = x.rstrip()

    if x == '':
        break
    if "STATIC," in x and startflag == 0:
        startflag = 1
        continue
    if "dynamic," in x and startflag == 1:
        startflag = 2
        continue
    if "STATIC" in x:
        startflag = 3
        continue
    if "dynamic,1000" in x and startflag == 3:
        startflag = 4
        continue
    if "prb_" in x:
        continue

    if startflag == 1:
        static100 = np.append(static100, x)
    if startflag == 2:
        dynamic100 = np.append(dynamic100, x)
    if startflag == 3:
        static = np.append(static, x)
    if startflag == 4:
        dynamic = np.append(dynamic, x)

static100 = static100.astype(np.float)
dynamic100 = dynamic100.astype(np.float)
static = static.astype(np.float)
dynamic = dynamic.astype(np.float)

static_init = static

static100a = static100
dynamic100a = dynamic100
dynamica = dynamic 
statica = static

ncores = range(len(static))


plt.figure(2)
plt.plot(ncores, static100)
plt.plot(ncores, dynamic100)
plt.plot(ncores, static)
plt.plot(ncores, dynamic)
plt.xlabel('Cores')
plt.ylabel('Time (s)')
plt.legend(['Static, 100', 'Dynamic, 100', 'Static', 'Dynamic, 100000'])
plt.title('Scheduling Effects on Execution Time')
plt.savefig('plots/Scheduling_Effects2.jpg')


plt.figure(3)
plt.plot(ncores, static_no_init)
plt.plot(ncores, static_init)
plt.xlabel('Cores')
plt.ylabel('Time (s)')
plt.legend(['ser_init', 'par_init'])
plt.title('Initialization Effects on Execution Time')
plt.savefig('plots/Initialization_Effects.jpg')

# ------ Part 2 A1 ----- #

filename = "hw1_pt2_b.txt"

f = open(filename)


static100 = np.array([])
dynamic100 = np.array([])
static = np.array([])
dynamic = np.array([])

a = 0
startflag = 0

while True:
    x = f.readline()
    x = x.rstrip()

    if x == '':
        break
    if "STATIC," in x and startflag == 0:
        startflag = 1
        continue
    if "dynamic," in x and startflag == 1:
        startflag = 2
        continue
    if "STATIC" in x:
        startflag = 3
        continue
    if "dynamic,1000" in x and startflag == 3:
        startflag = 4
        continue
    if "prb_" in x:
        continue

    if startflag == 1:
        static100 = np.append(static100, x)
    if startflag == 2:
        dynamic100 = np.append(dynamic100, x)
    if startflag == 3:
        static = np.append(static, x)
    if startflag == 4:
        dynamic = np.append(dynamic, x)

static100 = static100.astype(np.float)
dynamic100 = dynamic100.astype(np.float)
static = static.astype(np.float)
dynamic = dynamic.astype(np.float)

ncores = range(len(static))

static100b = static100
dynamic100b = dynamic100
dynamicb = dynamic 
staticb = static


plt.figure()
plt.plot(ncores, static100)
plt.plot(ncores, dynamic100)
plt.plot(ncores, static)
plt.plot(ncores, dynamic)
plt.xlabel('Cores')
plt.ylabel('Time (s)')
plt.legend(['Static, 100', 'Dynamic, 100', 'Static', 'Dynamic, 100000'])
plt.title('Scheduling Effects on Execution Time, prb_b')
plt.savefig('plots/Scheduling_Effects_b.jpg')

plt.figure()
plt.plot(ncores, static100b - static100a)
plt.plot(ncores, dynamic100b - dynamic100a)
plt.plot(ncores, staticb - statica)
plt.plot(ncores, dynamicb - dynamica)
plt.xlabel('Cores')
plt.ylabel('Time (s)')
plt.legend(['Static, 100', 'Dynamic, 100', 'Static', 'Dynamic, 100000'])
plt.title('One Parallel Directive vs Many')
plt.savefig('plots/Single_Par_vs_Many.jpg')