# MoSHEAHu

**MoSHEAHu** stands for **Most Stable High Entropy Alloy (HEA) Hunting**. This framework is a lightweight project designed to explore HEAs with the most stable crystal structures.

---

## 📚 Overview  

The workflow starts by generating a pool of randomized atomic configurations, placing five different atom types into an "egg carton" arrangement. This "egg carton" can represent the following crystal structures:  

- Face-Centered Cubic (**FCC**)  
- Body-Centered Cubic (**BCC**)  
- Hexagonal Close-Packed (**HCP**)  

In this example, the system under study is **HfNbTaTiZr** using a **BCC** structure. A set of **16 lattice parameters** ranging from **3.30 Å to 3.45 Å** (in increments of **0.01 Å**) is used. For each lattice parameter, **four simulations** are performed using different random configurations.  

---

## 📈 Warren-Cowley Parameter Calculation  

The Warren-Cowley short-range order (SRO) parameters are calculated using the `WarrenCowley.cpp` code, located in the `executables` directory. Compile it using:

```bash
g++ WarrenCowley.cpp -std=c++11 -o wc_calc
```

This code requires:  

- An `.xyz` file generated in the last step.  
- A `params.txt` file specifying the cutoff radius and lattice parameter.  

Unlike conventional methods that analyze only nearest neighbors, this code considers all atoms within the cutoff radius. It calculates the probability of finding an atom of type **A** around an atom of type **B**, which is especially useful for amorphous systems where nearest neighbors are not well-defined.

---

## 🛠️ Simulation Pipeline  

1. Randomized configurations are saved as `.data` files (e.g., `HfNbTaTiZr.data`) to be read by the LAMMPS input script.  
2. A common LAMMPS input file is copied to each simulation directory, designed to calculate the **total energy** for **1000** and **2000** timesteps.  
3. The MEAM potential described in [DOI:10.1016/j.matdes.2021.109560](https://doi.org/10.1016/j.matdes.2021.109560) is used, and the corresponding potential files are automatically copied into each directory.  
4. Simulations are executed in batch, and the results (energies and Warren-Cowley parameters) are collected.  
5. Each random structure is assigned a unique ID.  

---

## 📊 Analysis  

- The final output includes a plot of **Energy vs. Configuration ID**, which helps identify the most stable configurations.  
- The behavior of the Warren-Cowley parameters is analyzed for these stable systems, providing insight into their local atomic arrangements and stability mechanisms.  

---
