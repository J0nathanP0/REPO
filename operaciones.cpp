#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;

// Enumeración para tipos de datos
enum TipoDato {
    ENTERO,
    CADENA_FIJA,
    CADENA_VARIABLE,
    FECHA
};

// Estructura para representar un atributo
struct Atributo {
    string nombre;
    TipoDato tipo;
    int tamano; // Para cadena fija
    
    Atributo(string n, TipoDato t, int tam = 0) : nombre(n), tipo(t), tamano(tam) {}
};

// Estructura para representar una fila de datos
struct Fila {
    vector<string> valores;
    
    Fila(vector<string> v) : valores(v) {}
    
    bool operator==(const Fila& otra) const {
        return valores == otra.valores;
    }
    
    bool operator<(const Fila& otra) const {
        return valores < otra.valores;
    }
};

// Clase para representar una tabla
class Tabla {
public:
    string nombre;
    vector<Atributo> atributos;
    vector<Fila> filas;
    
    // Constructor por defecto
    Tabla() : nombre("") {}
    
    // Constructor con nombre
    Tabla(string n) : nombre(n) {}
    
    void agregarAtributo(const Atributo& attr) {
        atributos.push_back(attr);
    }
    
    void agregarFila(const Fila& fila) {
        if (fila.valores.size() == atributos.size()) {
            filas.push_back(fila);
        }
    }
    
    void mostrar() const {
        // Mostrar encabezados
        cout << "\n=== Tabla: " << nombre << " ===\n";
        for (size_t i = 0; i < atributos.size(); i++) {
            cout << setw(15) << atributos[i].nombre;
            if (i < atributos.size() - 1) cout << " | ";
        }
        cout << "\n";
        
        // Línea separadora
        for (size_t i = 0; i < atributos.size(); i++) {
            cout << string(15, '-');
            if (i < atributos.size() - 1) cout << "-+-";
        }
        cout << "\n";
        
        // Mostrar filas
        for (const auto& fila : filas) {
            for (size_t i = 0; i < fila.valores.size(); i++) {
                cout << setw(15) << fila.valores[i];
                if (i < fila.valores.size() - 1) cout << " | ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
    
    int buscarIndiceAtributo(const string& nombreAttr) const {
        for (size_t i = 0; i < atributos.size(); i++) {
            if (atributos[i].nombre == nombreAttr) {
                return i;
            }
        }
        return -1;
    }
};

// Clase principal del sistema
class SistemaAlgebraRelacional {
private:
    map<string, Tabla> tablas;
    
public:
    void definirEsquema() {
        int numTablas;
        cout << "¿Cuántas tablas desea crear? ";
        cin >> numTablas;
        
        for (int t = 0; t < numTablas; t++) {
            string nombreTabla;
            cout << "\nNombre de la tabla " << (t + 1) << ": ";
            cin >> nombreTabla;
            
            Tabla tabla(nombreTabla);
            
            int numAtributos;
            cout << "¿Cuántos atributos tiene la tabla " << nombreTabla << "? ";
            cin >> numAtributos;
            
            for (int a = 0; a < numAtributos; a++) {
                string nombreAttr;
                int tipoInt;
                
                cout << "\nAtributo " << (a + 1) << ":\n";
                cout << "Nombre: ";
                cin >> nombreAttr;
                
                cout << "Tipo de dato:\n";
                cout << "1. Entero\n";
                cout << "2. Cadena fija\n";
                cout << "3. Cadena variable\n";
                cout << "4. Fecha\n";
                cout << "Seleccione: ";
                cin >> tipoInt;
                
                TipoDato tipo = static_cast<TipoDato>(tipoInt - 1);
                int tamano = 0;
                
                if (tipo == CADENA_FIJA) {
                    cout << "Tamaño de la cadena: ";
                    cin >> tamano;
                }
                
                tabla.agregarAtributo(Atributo(nombreAttr, tipo, tamano));
            }
            
            // Solicitar datos para la tabla
            int numFilas;
            cout << "\n¿Cuántas filas de datos desea ingresar para " << nombreTabla << "? ";
            cin >> numFilas;
            
            cin.ignore(); // Limpiar buffer
            
            for (int f = 0; f < numFilas; f++) {
                vector<string> valores;
                cout << "Fila " << (f + 1) << ":\n";
                
                for (const auto& attr : tabla.atributos) {
                    string valor;
                    cout << attr.nombre << ": ";
                    getline(cin, valor);
                    valores.push_back(valor);
                }
                
                tabla.agregarFila(Fila(valores));
            }
            
            tablas[nombreTabla] = tabla;
        }
    }
    
    // Operador Proyección
    Tabla proyeccion(const string& nombreTabla, const vector<string>& atributosSeleccionados) {
        if (tablas.find(nombreTabla) == tablas.end()) {
            cout << "Tabla no encontrada.\n";
            return Tabla("ERROR");
        }
        
        Tabla tablaOriginal = tablas[nombreTabla];
        Tabla resultado("Proyeccion_" + nombreTabla);
        
        vector<int> indices;
        for (const string& attr : atributosSeleccionados) {
            int indice = tablaOriginal.buscarIndiceAtributo(attr);
            if (indice != -1) {
                indices.push_back(indice);
                resultado.agregarAtributo(tablaOriginal.atributos[indice]);
            }
        }
        
        set<vector<string>> filasUnicas;
        for (const auto& fila : tablaOriginal.filas) {
            vector<string> nuevaFila;
            for (int indice : indices) {
                nuevaFila.push_back(fila.valores[indice]);
            }
            filasUnicas.insert(nuevaFila);
        }
        
        for (const auto& fila : filasUnicas) {
            resultado.agregarFila(Fila(fila));
        }
        
        return resultado;
    }
    
    // Operador Selección
    Tabla seleccion(const string& nombreTabla, const string& atributo, const string& operador, const string& valor) {
        if (tablas.find(nombreTabla) == tablas.end()) {
            cout << "Tabla no encontrada.\n";
            return Tabla("ERROR");
        }
        
        Tabla tablaOriginal = tablas[nombreTabla];
        Tabla resultado("Seleccion_" + nombreTabla);
        
        // Copiar estructura
        for (const auto& attr : tablaOriginal.atributos) {
            resultado.agregarAtributo(attr);
        }
        
        int indiceAttr = tablaOriginal.buscarIndiceAtributo(atributo);
        if (indiceAttr == -1) {
            cout << "Atributo no encontrado.\n";
            return resultado;
        }
        
        for (const auto& fila : tablaOriginal.filas) {
            bool cumpleCondicion = false;
            string valorFila = fila.valores[indiceAttr];
            
            if (operador == "=") {
                cumpleCondicion = (valorFila == valor);
            } else if (operador == "!=") {
                cumpleCondicion = (valorFila != valor);
            } else if (operador == ">") {
                cumpleCondicion = (valorFila > valor);
            } else if (operador == "<") {
                cumpleCondicion = (valorFila < valor);
            } else if (operador == ">=") {
                cumpleCondicion = (valorFila >= valor);
            } else if (operador == "<=") {
                cumpleCondicion = (valorFila <= valor);
            }
            
            if (cumpleCondicion) {
                resultado.agregarFila(fila);
            }
        }
        
        return resultado;
    }
    
    // Operador Producto Cruz
    Tabla productoCruz(const string& tabla1, const string& tabla2) {
        if (tablas.find(tabla1) == tablas.end() || tablas.find(tabla2) == tablas.end()) {
            cout << "Una o ambas tablas no encontradas.\n";
            return Tabla("ERROR");
        }
        
        Tabla t1 = tablas[tabla1];
        Tabla t2 = tablas[tabla2];
        Tabla resultado("ProductoCruz_" + tabla1 + "_" + tabla2);
        
        // Combinar atributos
        for (const auto& attr : t1.atributos) {
            resultado.agregarAtributo(Atributo(tabla1 + "." + attr.nombre, attr.tipo, attr.tamano));
        }
        for (const auto& attr : t2.atributos) {
            resultado.agregarAtributo(Atributo(tabla2 + "." + attr.nombre, attr.tipo, attr.tamano));
        }
        
        // Combinar filas
        for (const auto& fila1 : t1.filas) {
            for (const auto& fila2 : t2.filas) {
                vector<string> nuevaFila = fila1.valores;
                nuevaFila.insert(nuevaFila.end(), fila2.valores.begin(), fila2.valores.end());
                resultado.agregarFila(Fila(nuevaFila));
            }
        }
        
        return resultado;
    }
    
    // Operador Unión
    Tabla union_(const string& tabla1, const string& tabla2) {
        if (tablas.find(tabla1) == tablas.end() || tablas.find(tabla2) == tablas.end()) {
            cout << "Una o ambas tablas no encontradas.\n";
            return Tabla("ERROR");
        }
        
        Tabla t1 = tablas[tabla1];
        Tabla t2 = tablas[tabla2];
        
        // Verificar compatibilidad de esquemas
        if (t1.atributos.size() != t2.atributos.size()) {
            cout << "Las tablas no son compatibles para la unión.\n";
            return Tabla("ERROR");
        }
        
        Tabla resultado("Union_" + tabla1 + "_" + tabla2);
        
        // Copiar estructura de la primera tabla
        for (const auto& attr : t1.atributos) {
            resultado.agregarAtributo(attr);
        }
        
        set<vector<string>> filasUnicas;
        
        // Agregar filas de ambas tablas
        for (const auto& fila : t1.filas) {
            filasUnicas.insert(fila.valores);
        }
        for (const auto& fila : t2.filas) {
            filasUnicas.insert(fila.valores);
        }
        
        for (const auto& fila : filasUnicas) {
            resultado.agregarFila(Fila(fila));
        }
        
        return resultado;
    }
    
    // Operador Diferencia
    Tabla diferencia(const string& tabla1, const string& tabla2) {
        if (tablas.find(tabla1) == tablas.end() || tablas.find(tabla2) == tablas.end()) {
            cout << "Una o ambas tablas no encontradas.\n";
            return Tabla("ERROR");
        }
        
        Tabla t1 = tablas[tabla1];
        Tabla t2 = tablas[tabla2];
        Tabla resultado("Diferencia_" + tabla1 + "_" + tabla2);
        
        // Copiar estructura de la primera tabla
        for (const auto& attr : t1.atributos) {
            resultado.agregarAtributo(attr);
        }
        
        set<vector<string>> filasT2;
        for (const auto& fila : t2.filas) {
            filasT2.insert(fila.valores);
        }
        
        for (const auto& fila : t1.filas) {
            if (filasT2.find(fila.valores) == filasT2.end()) {
                resultado.agregarFila(fila);
            }
        }
        
        return resultado;
    }
    
    // Operador Intersección
    Tabla interseccion(const string& tabla1, const string& tabla2) {
        if (tablas.find(tabla1) == tablas.end() || tablas.find(tabla2) == tablas.end()) {
            cout << "Una o ambas tablas no encontradas.\n";
            return Tabla("ERROR");
        }
        
        Tabla t1 = tablas[tabla1];
        Tabla t2 = tablas[tabla2];
        Tabla resultado("Interseccion_" + tabla1 + "_" + tabla2);
        
        // Copiar estructura de la primera tabla
        for (const auto& attr : t1.atributos) {
            resultado.agregarAtributo(attr);
        }
        
        set<vector<string>> filasT2;
        for (const auto& fila : t2.filas) {
            filasT2.insert(fila.valores);
        }
        
        for (const auto& fila : t1.filas) {
            if (filasT2.find(fila.valores) != filasT2.end()) {
                resultado.agregarFila(fila);
            }
        }
        
        return resultado;
    }
    
    // Operador Reunión (Join Natural)
    Tabla reunion(const string& tabla1, const string& tabla2) {
        if (tablas.find(tabla1) == tablas.end() || tablas.find(tabla2) == tablas.end()) {
            cout << "Una o ambas tablas no encontradas.\n";
            return Tabla("ERROR");
        }
        
        Tabla t1 = tablas[tabla1];
        Tabla t2 = tablas[tabla2];
        Tabla resultado("Reunion_" + tabla1 + "_" + tabla2);
        
        // Encontrar atributos comunes
        vector<pair<int, int>> atributosComunes;
        vector<string> nombresComunes;
        
        for (size_t i = 0; i < t1.atributos.size(); i++) {
            for (size_t j = 0; j < t2.atributos.size(); j++) {
                if (t1.atributos[i].nombre == t2.atributos[j].nombre) {
                    atributosComunes.push_back(make_pair(i, j));
                    nombresComunes.push_back(t1.atributos[i].nombre);
                }
            }
        }
        
        // Crear estructura del resultado
        for (const auto& attr : t1.atributos) {
            resultado.agregarAtributo(attr);
        }
        for (const auto& attr : t2.atributos) {
            bool esComun = false;
            for (const string& nombreComun : nombresComunes) {
                if (attr.nombre == nombreComun) {
                    esComun = true;
                    break;
                }
            }
            if (!esComun) {
                resultado.agregarAtributo(attr);
            }
        }
        
        // Realizar el join
        for (const auto& fila1 : t1.filas) {
            for (const auto& fila2 : t2.filas) {
                bool coincide = true;
                
                for (const auto& par : atributosComunes) {
                    if (fila1.valores[par.first] != fila2.valores[par.second]) {
                        coincide = false;
                        break;
                    }
                }
                
                if (coincide) {
                    vector<string> nuevaFila = fila1.valores;
                    for (size_t i = 0; i < fila2.valores.size(); i++) {
                        bool esComun = false;
                        for (const auto& par : atributosComunes) {
                            if ((int)i == par.second) {
                                esComun = true;
                                break;
                            }
                        }
                        if (!esComun) {
                            nuevaFila.push_back(fila2.valores[i]);
                        }
                    }
                    resultado.agregarFila(Fila(nuevaFila));
                }
            }
        }
        
        return resultado;
    }
    
    void mostrarTablas() {
        cout << "\n=== TABLAS DISPONIBLES ===\n";
        for (const auto& par : tablas) {
            par.second.mostrar();
        }
    }
    
    void menuOperadores() {
        while (true) {
            cout << "\n=== MENÚ DE OPERADORES RELACIONALES ===\n";
            cout << "1. Proyección\n";
            cout << "2. Selección\n";
            cout << "3. Producto Cruz\n";
            cout << "4. Unión\n";
            cout << "5. Diferencia\n";
            cout << "6. Intersección\n";
            cout << "7. Reunión (Join Natural)\n";
            cout << "8. Mostrar todas las tablas\n";
            cout << "9. Salir\n";
            cout << "Seleccione una opción: ";
            
            int opcion;
            cin >> opcion;
            
            switch (opcion) {
                case 1: {
                    string nombreTabla;
                    cout << "Tabla para proyección: ";
                    cin >> nombreTabla;
                    
                    int numAttr;
                    cout << "¿Cuántos atributos proyectar? ";
                    cin >> numAttr;
                    
                    vector<string> atributos;
                    for (int i = 0; i < numAttr; i++) {
                        string attr;
                        cout << "Atributo " << (i + 1) << ": ";
                        cin >> attr;
                        atributos.push_back(attr);
                    }
                    
                    Tabla resultado = proyeccion(nombreTabla, atributos);
                    resultado.mostrar();
                    break;
                }
                case 2: {
                    string nombreTabla, atributo, operador, valor;
                    cout << "Tabla para selección: ";
                    cin >> nombreTabla;
                    cout << "Atributo: ";
                    cin >> atributo;
                    cout << "Operador (=, !=, >, <, >=, <=): ";
                    cin >> operador;
                    cout << "Valor: ";
                    cin >> valor;
                    
                    Tabla resultado = seleccion(nombreTabla, atributo, operador, valor);
                    resultado.mostrar();
                    break;
                }
                case 3: {
                    string tabla1, tabla2;
                    cout << "Primera tabla: ";
                    cin >> tabla1;
                    cout << "Segunda tabla: ";
                    cin >> tabla2;
                    
                    Tabla resultado = productoCruz(tabla1, tabla2);
                    resultado.mostrar();
                    break;
                }
                case 4: {
                    string tabla1, tabla2;
                    cout << "Primera tabla: ";
                    cin >> tabla1;
                    cout << "Segunda tabla: ";
                    cin >> tabla2;
                    
                    Tabla resultado = union_(tabla1, tabla2);
                    resultado.mostrar();
                    break;
                }
                case 5: {
                    string tabla1, tabla2;
                    cout << "Tabla 1 - Tabla 2: ";
                    cout << "Primera tabla: ";
                    cin >> tabla1;
                    cout << "Segunda tabla: ";
                    cin >> tabla2;
                    
                    Tabla resultado = diferencia(tabla1, tabla2);
                    resultado.mostrar();
                    break;
                }
                case 6: {
                    string tabla1, tabla2;
                    cout << "Primera tabla: ";
                    cin >> tabla1;
                    cout << "Segunda tabla: ";
                    cin >> tabla2;
                    
                    Tabla resultado = interseccion(tabla1, tabla2);
                    resultado.mostrar();
                    break;
                }
                case 7: {
                    string tabla1, tabla2;
                    cout << "Primera tabla: ";
                    cin >> tabla1;
                    cout << "Segunda tabla: ";
                    cin >> tabla2;
                    
                    Tabla resultado = reunion(tabla1, tabla2);
                    resultado.mostrar();
                    break;
                }
                case 8:
                    mostrarTablas();
                    break;
                case 9:
                    return;
                default:
                    cout << "Opción inválida.\n";
            }
        }
    }
    
    void ejecutar() {
        cout << "=== SISTEMA DE ÁLGEBRA RELACIONAL ===\n\n";
        cout << "Primero, definiremos el esquema de la base de datos.\n";
        
        definirEsquema();
        
        cout << "\nEsquema definido correctamente.\n";
        mostrarTablas();
        
        menuOperadores();
    }
};

int main() {
    SistemaAlgebraRelacional sistema;
    sistema.ejecutar();
    
    return 0;
}