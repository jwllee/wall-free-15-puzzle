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


def df_to_latex(df, f):
    pass


if __name__ == '__main__':
    hash_dir = os.path.join('.', 'hash')
    figure_dir = os.path.join('.', 'figure')

    if not os.path.isdir(figure_dir):
        os.makedirs(figure_dir)

    row_list = []
    colnames = [
        'name', 'n_slots', 'n_states', 'n_rehash', 'n_zero_slots', 'n_collisions',
        'min. slot length', 'max. slot length', 'avg. slot length', 'std. slot length'
    ]

    for f in os.listdir(hash_dir):
        if '.txt' not in f:
            continue

        print('Processing {}'.format(f))

        name = f.replace('.txt', '')
        hash_f = os.path.join(hash_dir, f)
        figure_f = os.path.join(figure_dir, '{}.png'.format(name))

        with open(hash_f, 'r') as f:
            n_slots, ht_size, n_rehash = f.readline().split(' ')
            n_slots = int(n_slots)
            ht_size = int(ht_size)
            n_rehash = int(n_rehash)
            slot_count_list = list()

            for i in range(n_slots):
                slot_count = int(f.readline())
                slot_count_list.append(slot_count)

            slot_count_list = map(lambda i: int(i), slot_count_list)
            slot_count_list = list(slot_count_list)

        slot_count_df = pd.DataFrame(slot_count_list)
        slot_count_df = slot_count_df.reset_index()
        slot_count_df.columns = ['index', 'count']

        n_zero_item_slots = slot_count_df[slot_count_df['count'] == 0].shape[0]
        # avg_slot_len = slot_count_df[slot_count_df['count'] > 0]['count'].mean()
        # sd_slot_len = slot_count_df[slot_count_df['count'] > 0]['count'].std()
        avg_slot_len = slot_count_df['count'].mean()
        sd_slot_len = slot_count_df['count'].std()
        min_slot_len = slot_count_df['count'].min()
        max_slot_len = slot_count_df['count'].max()
        print('Min: {}, Max: {}, Avg: {:.2f}, Sd: {:.2f}, Number of slots with 0 items: {}'.format(min_slot_len,
                                                                                                   max_slot_len,
                                                                                                   avg_slot_len,
                                                                                                   sd_slot_len,
                                                                                                   n_zero_item_slots))
        collision_df = slot_count_df.copy()
        collision_df['count'] = collision_df['count'] - 1
        n_collisions = collision_df[collision_df['count'] > 0]['count'].sum()
        print('Collisions: {}'.format(n_collisions))
        print('Rehash: {}'.format(n_rehash))

        row = [
            name, n_slots, ht_size, n_rehash, n_zero_item_slots, n_collisions,
            min_slot_len, max_slot_len,
            float('{:.2f}'.format(avg_slot_len)),
            float('{:.2f}'.format(sd_slot_len)),
        ]
        row_list.append(row)

        if len(slot_count_list) > 100:
            plt.rcParams['xtick.labelsize'] = 5.0
        else:
            plt.rcParams['xtick.labelsize'] = 15.0

        fig, ax = plt.subplots(figsize=(10, 8))
        slot_count_df.plot(kind='bar', x='index', y='count', ax=ax, color='tomato')
        ax.set_xlabel('index')
        ax.set_ylabel('count')
        # fig.savefig(figure_f, dpi=300)

    table_out = os.path.join('.', 'table.csv')
    table_df = pd.DataFrame({key: val for key, val in zip(colnames, zip(*row_list))})
    table_df = table_df.sort_values('name', ascending=True)
    table_df.to_csv(table_out, index=False)
