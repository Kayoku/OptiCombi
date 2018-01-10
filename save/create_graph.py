#!/usr/bin/python3

import matplotlib.pyplot as plt
import matplotlib.gridspec as gridpsec
import matplotlib.colors as mcolors
import numpy as np
import csv
import sys

cmap = plt.get_cmap('tab20')
rgba = [cmap(x) for x in np.arange(0, 1, 0.05)]
fields_names = ["Temps moyens en millisecondes", "Déviations moyennes", "Nombre de calculs du coût moyens", "Fiabilité", "Nombre de problèmes résolus"]
res = {}
order = []

with open(sys.argv[1], "r", newline='') as csvfile:
  spamreader = csv.reader(csvfile, delimiter=',')
  first = True
  for row in spamreader:

    # La première ligne représente les noms des champs
    if first:
      first = False
    else:
      order.append(row[0])
      res[row[0]] = [0 for _ in range(len(fields_names))] 
      for i in range(len(row)-1):
        res[row[0]][i] = row[i+1]

print(order)
barWidth = 0.5

# On récupère les données
temps = [np.int(float(res[v][0]))/1000 for v in order]
deviation = [float(res[v][1]) for v in order]
cost = [np.int(float(res[v][2])) for v in order]
rate = [float(res[v][3]) for v in order]
find = [np.int(float(res[v][4])) for v in order]
x = range(len(temps))

# Création des graphiques
fig = plt.figure(figsize=(12, 12))
gs = gridpsec.GridSpec(3, 2)

ax1 = plt.subplot(gs[0])
ax2 = plt.subplot(gs[1])
ax3 = plt.subplot(gs[2])
ax4 = plt.subplot(gs[3])
ax5 = plt.subplot(gs[2, :])

print(len([i for i in range(len(temps))]))
print(len([v for v in order]))
plt.setp((ax1, ax2, ax3, ax4), xticks=[i for i in range(len(temps))], xticklabels=["" for _ in order])
plt.setp((ax5), xticks=[i for i in range(len(temps))], xticklabels=[v for v in order])

rect1 = ax1.bar(x, temps, width = barWidth, color=rgba, linewidth = 1)
ax1.set_title(fields_names[0])

rect2 = ax2.bar(x, deviation, width = barWidth, color=rgba, linewidth = 1)
ax2.set_title(fields_names[1])

rect3 = ax3.bar(x, cost, width = barWidth, color=rgba, linewidth = 1)
ax3.set_title(fields_names[2])

rect4 = ax4.bar(x, rate, width = barWidth, color=rgba, linewidth = 1)
ax4.set_title(fields_names[3])
ax4.set_ylim((0, 110))

rect5 = ax5.bar(x, find, width = barWidth, color=rgba, linewidth = 1)
ax5.set_title(fields_names[4])
ax5.set_ylim((0, 130))

plt.setp(ax1.get_xticklabels(), rotation=45)
plt.setp(ax2.get_xticklabels(), rotation=45)
plt.setp(ax3.get_xticklabels(), rotation=45)
plt.setp(ax4.get_xticklabels(), rotation=45)
plt.setp(ax5.get_xticklabels(), rotation=45)

plt.setp(ax1.get_xticklabels(), ha='right')
plt.setp(ax2.get_xticklabels(), ha='right')
plt.setp(ax3.get_xticklabels(), ha='right')
plt.setp(ax4.get_xticklabels(), ha='right')
plt.setp(ax5.get_xticklabels(), ha='right')

def autolabels(rects, ax):
  for rect in rects:
    height = rect.get_height()
    ax.text(rect.get_x() + rect.get_width()/2., height, '%.1f' % float(height), ha='center', va='center', fontsize=8, fontweight='bold')

autolabels(rect1, ax1)
autolabels(rect2, ax2)
autolabels(rect3, ax3)
autolabels(rect4, ax4)
autolabels(rect5, ax5)

fig.tight_layout()
fig.savefig('test.png')

