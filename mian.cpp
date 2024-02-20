#include <iostream>
#include <chrono>
#include <time.h>
#include "monticuloBinomial.h"

using namespace std;

int menu() {

    cout << "---------------------------------" << endl;
    cout << "| Operaciones con Binomial heap |" << endl;
    cout << "---------------------------------" << endl;
    cout << "1) Insertar un elemento en un NUEVO heap" << endl;
    cout << "2) Insertar un elemento en un heap" << endl;
    cout << "3) Extraer el minimo de un heap" << endl;
    cout << "4) Decrementar la clave de un nodo de un heap" << endl;
    cout << "5) Eliminar un node de un heap" << endl;
    cout << "6) Minimo de un heap" << endl;
    cout << "------------------" << endl;
    cout << "|Funciones extras|" << endl;
    cout << "------------------" << endl;
    cout << "7) Unir dos Heaps" << endl;
    cout << "8) Representar Heaps" << endl;
    cout << "0) Salir" << endl;
    cout << "Elige una opcion: ";

    int opcion = 0;
    cin >> opcion;
    return opcion;
}

int main() {

    monticuloBinomial<int> H;
    char a = ' ';
    cout << "Quieres entra en modo (p)rueba o en modo (n)ormal ? ";
    cin >> a;
    if (a == 'p') {
        try
        {
            int vuelta = 50000;

            H.insertarNuevoMonticulo(1);
            /*Insertar*/
            auto start = chrono::steady_clock::now();
            for (int i = 2; i < vuelta; i++) {
                H.insertar(0, i);
            }
            cout << "Tiempo de insercion";
            auto end = chrono::steady_clock::now();
            cout << " en millisegundos : "
                << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << " ms" << endl;


            /*Extraer minimo*/
            /*for (int i = 2; i < vuelta; i++) {
                H.insertar(0, i);
            }*/

            auto startExtraerMin = chrono::steady_clock::now();
            for (int i = 1; i < vuelta; i++) {
                H.extraerMin(0);
            }
            cout << "Tiempo de extraer el minimo en";
            auto endExtraerMin = chrono::steady_clock::now();
            cout << " en millisegundos : "
                << chrono::duration_cast<chrono::milliseconds>(endExtraerMin - startExtraerMin).count()
                << " ms" << endl;


            H.insertarNuevoMonticulo(1);
            for (int i = 2; i < vuelta; i++) {
                H.insertar(0, i);
            }

            /*Eliminar clave*/
            auto startElimi = chrono::steady_clock::now();
            for (int i = 1; i < vuelta; i++) {
                H.eliminarClave(0, i);
            }
            cout << "Tiempo de eliminar clave en";
            auto endElimi = chrono::steady_clock::now();
            cout << " en millisegundos : "
                << chrono::duration_cast<chrono::milliseconds>(endElimi - startElimi).count()
                << " ms" << endl;

            /*decrementar clave*/
            H.insertarNuevoMonticulo(vuelta);
            for (int i = vuelta+1; i < vuelta*2; i++) {
                H.insertar(0, i);
            }

            auto startDecre = chrono::steady_clock::now();
            for (int i = vuelta*2 - 1; i > vuelta; i--) {
                H.decrementarClave(0, i, i - (vuelta));
            }
            cout << "Tiempo de decrementar clave en";
            auto endDecre = chrono::steady_clock::now();
            cout << " en millisegundos : "
                << chrono::duration_cast<chrono::milliseconds>(endDecre - startDecre).count()
                << " ms" << endl;

        }
        catch (domain_error e) {
            cout << e.what();
        }
        
    }
    else {
        int opcion = menu();
        while (opcion != 0) {

            int e, k;
            int h1 = 0, h2 = 1, h = 0;

            switch (opcion) {

            case 1:
                cout << "Introduzca el elemento: ";
                cin >> e;
                H.insertarNuevoMonticulo(e);
                break;

            case 2:
                if (H.heapsSize() > 1) {

                    cout << "Introduzca el heap a insertar: ";
                    cin >> h;
                    h--;
                }
                cout << "Introduzca el elemento: ";
                cin >> e;
                try {

                    H.insertar(h, e);
                }
                catch (domain_error e) {
                    cout << e.what();
                }
                break;

            case 3:
                if (H.heapsSize() > 1) {
                    cout << "Introduzca el heap a buscar: ";
                    cin >> h;
                    h--;
                }
                try
                {
                    cout << "El minimo extraido es: " << H.extraerMin(h) << "\n";
                }
                catch (domain_error e)
                {
                    cout << e.what();
                }
                break;

            case 4:
                if (H.heapsSize() > 1) {
                    cout << "Introduzca el heap: ";
                    cin >> h;
                    h--;
                }
                cout << "Introduzca el elemento: ";
                cin >> e;
                cout << "Introduzca el valor a decrementar: ";
                cin >> k;
                try
                {
                    H.decrementarClave(h, e, k);
                }
                catch (domain_error e)
                {
                    cout << e.what();
                }
                break;
            case 5:
                if (H.heapsSize() > 1) {
                    cout << "Introduzca en que heap quiere eliminar: ";
                    cin >> h;
                    h--;
                }
                cout << "Introduzca el elemento: ";
                cin >> e;
                try
                {
                    if (H.eliminarClave(h, e)) {
                        cout << "Se ha eliminado\n";
                    }
                    else {
                        cout << "No se ha eliminado\n";
                    }
                }
                catch (domain_error e)
                {
                    cout << e.what();
                }
                break;
            case 6:
                if (H.heapsSize() > 1) {
                    cout << "Introduzca el heap: ";
                    cin >> h;
                    h--;
                }
                try
                {
                    cout << "El minimo es: " << H.minimo(h) << '\n';
                
                }
                catch (domain_error e)
                {
                    cout << e.what();
                }

                break;
            case 7:
                if (H.heapsSize() > 1) {
                    if (H.heapsSize() > 2) {
                        cout << "Introduzca el primer heap: ";
                        cin >> h1;
                        cout << "Introduzca el segundo heap: ";
                        cin >> h2;
                        h1--;
                        h2--;
                    }

                    try
                    {
                        H.unir(h1, h2);
                    }
                    catch (domain_error e)
                    {
                        cout << e.what();
                    }

                }
                else {
                    cout << "No tienes suficiente heap\n";
                }

                break;
            case 8:
                for (int i = 0; i < H.heapsSize(); i++) {
                    try
                    {
                        cout << "Monticulo " << i + 1 << "\n";
                        H.imprimir(i);
                    }
                    catch (domain_error e)
                    {
                        cout << e.what();
                    }
                }

                break;
            case 0:
                break;
            default:
                cout << "Introduzca una opcion valida\n";
                break;
            }

            opcion = menu();
        }
    }

   
	return 0;
}