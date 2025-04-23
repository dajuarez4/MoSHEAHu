#---- This function create BCC, FCC and HCP crystal structures,
#---- Define your lattice parameter, cs= 'bcc' for BBC, and so on, and system
# size, which is a cubic with (100), (010) and (001) directions. 


# ------- equiatomic
def generate_atomic_positions(lattice_parameters, system_size, cs):
    if isinstance(lattice_parameters, (float, int)):
        lattice_parameters = [lattice_parameters]

    atomic_positions = {}

    for lattice_parameter in lattice_parameters:
        basis = np.eye(3) * lattice_parameter

        base_atoms_bcc = np.array([
            [0.0, 0.0, 0.0],
            [0.5, 0.5, 0.5]
        ]) * lattice_parameter

        base_atoms_fcc = np.array([
            [0.0, 0.0, 0.0],
            [0.5, 0.5, 0.0],
            [0.5, 0.0, 0.5],
            [0.0, 0.5, 0.5]
        ]) * lattice_parameter

        base_atoms_hcp = np.array([
            [0.0, 0.0, 0.0],
            [2/3, 1/3, 0.5]
        ]) * lattice_parameter

        positions = []

        for i in range(system_size):
            for j in range(system_size):
                for k in range(system_size):
                    base_position = np.array([i, j, k])

                    if cs == 'hcp':
                        basis = np.array([
                            [1,    0,                  0],
                            [-0.5, np.sqrt(3)/2,        0],
                            [0,    0,   np.round(np.sqrt(8/3), 4)]
                        ])

                    cart_position = np.dot(basis.T, base_position)

                    if cs == 'fcc':
                        for atom in base_atoms_fcc:
                            positions.append(cart_position + atom)
                    elif cs == 'bcc':
                        for atom in base_atoms_bcc:
                            positions.append(cart_position + atom)
                    elif cs == 'hcp':
                        for atom in base_atoms_hcp:
                            positions.append(cart_position + atom)

        atomic_positions[lattice_parameter] = positions

    return atomic_positions

