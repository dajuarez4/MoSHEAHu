simulation_path = './simulations'
potential_path  = './potentials/'
executable_path = './executables/'
pot_files = ['library_HfNbTaTiZr.meam','HfNbTaTiZr.meam']
name_hea = 'HfNbTaTiZr' #HfNbTaTiZr.xyz
lmp_mpi = 'lmp_mpi'
cuttof = 2.0
system_size = 20
cs = 'bcc'
atom_types = ['Hf', 'Nb', 'Ta', 'Ti', 'Zr']
num_types = len(atom_types)
initial_lattice = 3.38
n_latti_par = 1
n_sim = 2
lattice_parameters = [np.round(initial_lattice + ii*(0.01), 2) for ii in range(n_latti_par)]
simulation_paths = [simulation_path + f'{sim+1}/' for sim in range(n_sim)]

