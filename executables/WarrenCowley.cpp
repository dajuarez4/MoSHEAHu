#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <set>

using namespace std;

class WarrenCowleyCalculator {
public:
    int n;
    double alat;
    double rcut;
    vector<vector<double>> positions;  // (n, 3)
    vector<string> types;               // atom types (Hf, Nb, etc.)
    map<string, int> type_count;        // atom type → number of atoms

    WarrenCowleyCalculator() {}

    void read_parameters(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open parameter file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.find("alat") != string::npos) {
                alat = stod(line.substr(line.find("=") + 1));
            }
            if (line.find("cutoff") != string::npos) {
                rcut = stod(line.substr(line.find("=") + 1));
            }
        }
        file.close();
    }

    void read_positions(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open positions file " << filename << endl;
            return;
        }

        string line;
        getline(file, line); // skip first line (number of atoms)
        getline(file, line); // skip comment line

        while (getline(file, line)) {
            istringstream iss(line);
            string type;
            double x, y, z;
            if (!(iss >> type >> x >> y >> z)) {
                cerr << "Error reading line: " << line << endl;
                continue;
            }
            types.push_back(type);
            positions.push_back({x, y, z});
            type_count[type]++;
        }

        n = positions.size();
        file.close();
    }

    void calculate_distances(vector<vector<double>>& distance_matrix) {
        distance_matrix.resize(n, vector<double>(n, 0.0));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (i != j) {
                    double dx = positions[j][0] - positions[i][0];
                    double dy = positions[j][1] - positions[i][1];
                    double dz = positions[j][2] - positions[i][2];

                    // Apply minimum image convention
                    if (dx > alat / 2.0) dx -= alat;
                    else if (dx < -alat / 2.0) dx += alat;

                    if (dy > alat / 2.0) dy -= alat;
                    else if (dy < -alat / 2.0) dy += alat;

                    if (dz > alat / 2.0) dz -= alat;
                    else if (dz < -alat / 2.0) dz += alat;

                    double dist = sqrt(dx * dx + dy * dy + dz * dz);
                    distance_matrix[i][j] = dist;
                }
            }
        }
    }

//    void calculate_warren_cowley(const vector<vector<double>>& distance_matrix) {
//        map<pair<string, string>, double> neighbor_count;
//        map<pair<string, string>, double> warren_cowley;
//
//        // Count neighbors
//        for (int i = 0; i < n; ++i) {
//            string type_i = types[i];
//            for (int j = 0; j < n; ++j) {
//                if (i != j && distance_matrix[i][j] <= rcut) {
//                    string type_j = types[j];
//                    neighbor_count[{type_i, type_j}]++;
//                }
//            }
//        }
//
//        // Calculate P_ij and alpha_ij
//        for (auto& it : neighbor_count) {
//            string type_i = it.first.first;
//            string type_j = it.first.second;
//
//            double nij = it.second;
//            double neighbors_total_i = 0.0;
//
//            // Sum total neighbors of type_i
//            for (auto& jt : neighbor_count) {
//                if (jt.first.first == type_i) {
//                    neighbors_total_i += jt.second;
//                }
//            }
//
//            double P_ij = nij / neighbors_total_i;
//            double cj = static_cast<double>(type_count[type_j]) / n;
//
//            double alpha_ij = 1.0 - (P_ij / cj);
//            warren_cowley[{type_i, type_j}] = alpha_ij;
//        }
//
//        // print results
//        cout << "\n--- Warren-Cowley parameters (alpha_ij) ---\n";
//        for (auto& it : warren_cowley) {
//            string type_i = it.first.first;
//            string type_j = it.first.second;
//            double alpha = it.second;
//
//            cout << "Alpha_" << type_i << "-" << type_j << " = " << alpha << endl;
//        }
//    }
    
    void calculate_warren_cowley(const vector<vector<double>>& distance_matrix) {
        map<pair<string, string>, double> neighbor_count;
        map<pair<string, string>, double> warren_cowley;
        set<string> unique_types;

        // Count neighbors
        for (int i = 0; i < n; ++i) {
            string type_i = types[i];
            unique_types.insert(type_i);

            for (int j = 0; j < n; ++j) {
                if (i != j && distance_matrix[i][j] <= rcut) {
                    string type_j = types[j];
                    unique_types.insert(type_j);

                    neighbor_count[{type_i, type_j}]++;
                }
            }
        }

        // Calculate Warren-Cowley parameters for all type pairs
        cout << "\n--- Warren-Cowley parameters (alpha_ij) ---\n";

        for (const auto& type_i : unique_types) {
            // First, calculate total neighbors for type_i
            double neighbors_total_i = 0.0;
            for (const auto& type_j : unique_types) {
                neighbors_total_i += neighbor_count[{type_i, type_j}];
            }

            for (const auto& type_j : unique_types) {
                double nij = neighbor_count[{type_i, type_j}];
                double P_ij = (neighbors_total_i > 0) ? (nij / neighbors_total_i) : 0.0;
                double cj = static_cast<double>(type_count[type_j]) / n;
                double alpha_ij;

                if (cj > 0) {
                    alpha_ij = 1.0 - (P_ij / cj);
                } else {
                    alpha_ij = 0.0; // No atoms of type_j
                }

                cout << "Alpha_" << type_i << "-" << type_j << " = " << alpha_ij << endl;
            }
        }
    }

};

int main() {
    WarrenCowleyCalculator calc;

    // Read parameters and positions
    calc.read_parameters("params.txt");
    calc.read_positions("HfNbTaTiZr.xyz");

    // Calculate distances
    vector<vector<double>> distance_matrix;
    calc.calculate_distances(distance_matrix);

    // Calculate Warren-Cowley parameters
    calc.calculate_warren_cowley(distance_matrix);

    return 0;
}

