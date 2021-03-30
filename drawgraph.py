#!/usr/bin/env python3
import subprocess
import matplotlib.pyplot as plt

def gen_input(n):
    cmd = ('./a.out ' + str(n)).split()
    
    sp = subprocess.Popen(cmd,shell=False,stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True)
    
    rc=sp.wait()
    
    out,err=sp.communicate()

def run_closest():
    t = []

    cmd = './closest input.txt'.split()
    
    sp = subprocess.Popen(cmd,shell=False,stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True)

    rc=sp.wait()
    
    out,err=sp.communicate()

    print(out)

    return float(out.split()[0]),float(out.split()[6])

step = 10
i = 10

x = []
y1 = []
y2 = []

while i <= 100000:
    print(i)
    x.append(i)
    gen_input(i)
    bf, sq = run_closest()
    y1.append(bf)
    y2.append(sq)
    if i == 100:
        step = 100
    elif i == 1000:
        step = 1000
    elif i == 10000:
        step = 10000
    i += step

# Initialise the figure and axes.
fig, ax = plt.subplots(1, figsize=(8, 6))

# Set the title for the figure
fig.suptitle('Comparacao entre forca bruta e divisao e conquista', fontsize=15)

# Draw all the lines in the same plot, assigning a label for each one to be
# shown in the legend.
ax.plot(x, y1, color="red", label="Forca Bruta")
ax.plot(x, y2, color="green", label="Divisao e Conquista")

ax.set_xlabel('n')
ax.set_ylabel('tempo (s)')

# Add a legend, and position it on the lower right (with no box)
plt.legend(loc="upper left", title="Legenda", frameon=False)

plt.show()
