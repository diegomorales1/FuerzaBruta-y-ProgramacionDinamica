#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <climits>
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;

// Declaración de matrices de costo
int** matriz_cost_insert;
int** matriz_cost_delete;
int** matriz_cost_replace;
int** matriz_cost_transpose;

// Función para cargar las matrices desde un archivo
int** cargar_matriz(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cerr << "Error al abrir el archivo: " << nombre_archivo << endl;
        exit(1);
    }

    int** matriz = new int*[26];
    for (int i = 0; i < 26; ++i) {
        matriz[i] = new int[26];
    }

    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            archivo >> matriz[i][j];
        }
    }

    archivo.close();
    return matriz;
}

char abecedario[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

int obtener_indice(char letra) {
    for (int i = 0; i < 26; ++i) {
        if (abecedario[i] == letra) {
            return i;
        }
    }
    return 0;
}

int costo_ins(char b) { return matriz_cost_insert[obtener_indice(b)][0]; }
int costo_del(char a) { return matriz_cost_delete[obtener_indice(a)][0]; }
int costo_sub(char a, char b) { return matriz_cost_replace[obtener_indice(a)][obtener_indice(b)]; }
int costo_trans(char a, char b) { return matriz_cost_transpose[obtener_indice(a)][obtener_indice(b)]; }

int FuerzaBruta(const string &S1, const string &S2, int i, int j) {
    if (i == 0) return j * costo_ins(S2[j - 1]);
    if (j == 0) return i * costo_del(S1[i - 1]);

    int costo = INT_MAX;
    costo = min(costo, FuerzaBruta(S1, S2, i - 1, j - 1) + costo_sub(S1[i - 1], S2[j - 1]));
    costo = min(costo, FuerzaBruta(S1, S2, i, j - 1) + costo_ins(S2[j - 1]));
    costo = min(costo, FuerzaBruta(S1, S2, i - 1, j) + costo_del(S1[i - 1]));

    if (i > 1 && j > 1 && S1[i - 1] == S2[j - 2] && S1[i - 2] == S2[j - 1]) {
        costo = min(costo, FuerzaBruta(S1, S2, i - 2, j - 2) + costo_trans(S1[i - 1], S1[i - 2]));
    }

    return costo;
}

int distanciaMinimaDinamica(const string &S1, const string &S2) {
    int n = S1.size(), m = S2.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));

    for (int i = 0; i <= n; ++i) dp[i][0] = i * costo_del(S1[i - 1]);
    for (int j = 0; j <= m; ++j) dp[0][j] = j * costo_ins(S2[j - 1]);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            dp[i][j] = dp[i - 1][j - 1] + costo_sub(S1[i - 1], S2[j - 1]);
            dp[i][j] = min(dp[i][j], dp[i][j - 1] + costo_ins(S2[j - 1]));
            dp[i][j] = min(dp[i][j], dp[i - 1][j] + costo_del(S1[i - 1]));
            if (i > 1 && j > 1 && S1[i - 1] == S2[j - 2] && S1[i - 2] == S2[j - 1]) {
                dp[i][j] = min(dp[i][j], dp[i - 2][j - 2] + costo_trans(S1[i - 1], S1[i - 2]));
            }
        }
    }
    return dp[n][m];
}

void actualizarResultadosGenerales(const string& metodo, const string& tipo, const vector<int>& largos, const vector<long long>& resultados) {
    ofstream archivo("resultados_generales.txt", ios::app);
    if (!archivo) {
        cerr << "Error: No se pudo abrir el archivo resultados_generales.txt" << endl;
        return;
    }

    archivo << metodo << ", largo = ";
    for (size_t i = 0; i < largos.size(); ++i) {
        archivo << largos[i];
        if (i < largos.size() - 1) archivo << ", ";
    }

    archivo << ", resultado (ns) = ";
    for (size_t i = 0; i < resultados.size(); ++i) {
        archivo << resultados[i];
        if (i < resultados.size() - 1) archivo << ", ";
    }

    archivo << "\n";

    archivo.close();
}

int main() {
    matriz_cost_insert = cargar_matriz("cost_insert.txt");
    matriz_cost_delete = cargar_matriz("cost_delete.txt");
    matriz_cost_replace = cargar_matriz("cost_replace.txt");
    matriz_cost_transpose = cargar_matriz("cost_transpose.txt");

    string S1, S2;
    ifstream file1("s1.txt"), file2("s2.txt");
    if (!file1 || !file2) {
        cerr << "Error: No se pudo abrir uno de los archivos de entrada." << endl;
        return 1;
    }
    getline(file1, S1); getline(file2, S2);

    vector<int> largos = { (int)S1.size() };
    vector<long long> resultados;

    auto inicio = high_resolution_clock::now();
    int resultadoFB = FuerzaBruta(S1, S2, S1.size(), S2.size());
    auto fin = high_resolution_clock::now();
    resultados.push_back(duration_cast<nanoseconds>(fin - inicio).count());
    actualizarResultadosGenerales("fuerza bruta", "transposicion", largos, resultados);

    inicio = high_resolution_clock::now();
    int resultadoDP = distanciaMinimaDinamica(S1, S2);
    fin = high_resolution_clock::now();
    resultados[0] = duration_cast<nanoseconds>(fin - inicio).count();
    actualizarResultadosGenerales("programacion dinamica", "transposicion", largos, resultados);

    return 0;
}
