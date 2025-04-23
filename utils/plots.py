import matplotlib.pyplot as plt

def plot_histogram(data, parameter_name, bins=10, ids=None):
    plt.figure(figsize=(6, 4))
    
    counts, bin_edges, patches = plt.hist(data, bins=bins, edgecolor='black')
    if ids is not None:
        id_index = 0  
        for count, left_edge, right_edge in zip(counts, bin_edges[:-1], bin_edges[1:]):
            bin_center = (left_edge + right_edge) / 2
            if count > 0:
                if id_index < len(ids):  
                    plt.text(bin_center, count/2, ids[id_index], ha='center', va='center',
                             fontsize=8, bbox=dict(facecolor='white', alpha=0.7))
                    id_index += 1
                    

    plt.xlabel(parameter_name)
    plt.ylabel('Frequency')
    plt.title(f'Histogram of {parameter_name}')
    plt.grid(True)
    plt.tight_layout()
    plt.show()


