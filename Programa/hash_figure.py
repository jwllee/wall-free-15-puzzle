#!/usr/bin/env python


import pandas as pd
import numpy as np
import os, sys
import matplotlib.pyplot as plt

plt.style.use('seaborn-white')
plt.rcParams['font.size'] = 15.0
plt.rcParams['axes.labelsize'] = 15.0
plt.rcParams['xtick.labelsize'] = 5.0
plt.rcParams['ytick.labelsize'] = 15.0
plt.rcParams['legend.fontsize'] = 15.0

pd.options.display.max_colwidth = 1000
pd.options.mode.chained_assignment = None


if __name__ == '__main__':
    hash_dir = os.path.join('.', 'hash')
    figure_dir = os.path.join('.', 'figure')

    if not os.path.isdir(figure_dir):
        os.makedirs(figure_dir)

    for f in os.listdir(hash_dir):
        if '.txt' not in f:
            continue

        print('Processing {}'.format(f))

        name = f.replace('.txt', '')
        hash_f = os.path.join(hash_dir, f)
        figure_f = os.path.join(figure_dir, '{}.png'.format(name))

        with open(hash_f, 'r') as f:
            n_slots, ht_size = f.readline().split(' ')
            n_slots = int(n_slots)
            ht_size = int(ht_size)
            slot_count_list = list()

            for i in range(n_slots):
                slot_count = int(f.readline())
                slot_count_list.append(slot_count)

            slot_count_list = map(lambda i: int(i), slot_count_list)
            slot_count_list = list(slot_count_list)

        slot_count_df = pd.DataFrame(slot_count_list)
        slot_count_df = slot_count_df.reset_index()
        slot_count_df.columns = ['index', 'count']

        if len(slot_count_list) > 100:
            plt.rcParams['xtick.labelsize'] = 5.0
        else:
            plt.rcParams['xtick.labelsize'] = 15.0

        fig, ax = plt.subplots(figsize=(10, 8))
        slot_count_df.plot(kind='bar', x='index', y='count', ax=ax, color='tomato')
        ax.set_xlabel('index')
        ax.set_ylabel('count')
        fig.savefig(figure_f, dpi=900)
