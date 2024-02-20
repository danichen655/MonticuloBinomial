/*
Autor: HongXiang Chen
*/

#ifndef monticulosBinomial_h
#define monticulosBinomial_h

#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <limits.h> 

using namespace std;

template <class T >
class monticuloBinomial {

public:

	class Nodo {
	public:
		int _elem; // elemento del monticulo
		Nodo* _parent;
		Nodo* _child;
		Nodo* _sibling;
		int _degree;

		// Constructor vacio
		Nodo(T const& e) :
			_elem(e), _parent(nullptr), _child(nullptr), _sibling(nullptr), _degree(0) {}

		// Constructor
		Nodo(T const& e, Nodo* const& p, Nodo* const& c, Nodo* const& s, size_t const& d) :
			_elem(e), _parent(p), _child(c), _sibling(s), _degree(d) {}
	};

	// Puntero al minimo del monticulo
	Nodo* _min;
	//Puntero a la cabeza del monticulo
	Nodo* _head;
	//Numero de elementos del monticulo
	int _nelems;

	/*Constructores*/
	monticuloBinomial() : _head(nullptr), _nelems(0) {}

	monticuloBinomial(T const& elem) {
		this->_head = new Nodo(elem);	
		this->_min = this->_head;
		this->_nelems = 1;
	}

	monticuloBinomial(Nodo* const h) : _head(h){}

protected:
	
	Nodo* insert(T const& e) {

		monticuloBinomial H = monticuloBinomial(e);
		Nodo* aux = H._head;
		this->binomialUnion(H);
		this->_nelems++;
		return aux;
	}

	T minimo() {
		if (this->_min == nullptr) {
			throw domain_error("No exite el puntero al minimo");
		}
		return this->_min->_elem;
	}

	Nodo* invertir(Nodo* y) {
		Nodo* next;
		Nodo* tail = nullptr;

		y->_parent = nullptr;
		while (y->_sibling != nullptr) {

			Nodo* next = y->_sibling;
			y->_sibling = tail;
			tail = y;
			y = next;
			y->_parent = nullptr;
		}
		y->_sibling = tail;

		return y;
	}

	void binomialLink(Nodo* const& y, Nodo* const& z) {
		y->_parent = z;
		y->_sibling = z->_child;
		z->_child = y;
		z->_degree++;
	}

	// busca el minimo elemento del monticulo
	// O(log n) ya que busca solo en las raices de los arboles binomiales.
	void binomialBuscarMin() {

		if (this->_head != nullptr) {

			Nodo* ptr = this->_head;
			//establecer el minimo para poder comparar luego
			if (this->_min == nullptr) {
				this->_min = this->_head;
			}

			ptr = ptr->_sibling;

			// comparar en cada elemento de la raiz
			while (ptr != nullptr) {

				if (ptr->_elem < this->_min->_elem) {
					this->_min = ptr;
				}
				ptr = ptr->_sibling;
			}
		}
	}

	T const& binomialExtraerMin() {

		if (this->_min == nullptr) {
			throw domain_error("No exite el puntero al minimo");
		}

		T ret = this->_min->_elem;

		Nodo* x = this->_head;
		Nodo* prev_x = nullptr;
		// apuntar el puntero min
		while (x != this->_min) {
			prev_x = x;
			x = x->_sibling;
		}

		// Eliminar el minimo
		if (prev_x == nullptr) {
			this->_head = x->_sibling;
		}
		else {
			prev_x->_sibling = x->_sibling;
			//this->_head = prev_x;
		}

		Nodo* z = nullptr;
		if (this->_min->_child != nullptr) {

			z = invertir(this->_min->_child);
		}
		monticuloBinomial H(z);

		// buscar el minimo para el nuevo heap creado (el heap donde tenia el minimo)
		H.binomialBuscarMin();

		// establecer a null ya que se ha eliminado
		this->_min = nullptr;
		this->binomialBuscarMin();

		this->binomialUnion(H);
		this->_nelems--;

		if (_nelems == 0) {
			this->_head = nullptr;
			this->_min = nullptr;
		}

		return ret;
	}

	void merge(Nodo* const& h) {
		
		Nodo* x = this->_head;
		Nodo* y = h;
		
		Nodo* ptr = nullptr;
		Nodo* aux = nullptr;

		if (x->_degree <= y->_degree) {
			ptr = x;
			x = x->_sibling;
		}
		else {
			ptr = y;
			y = y->_sibling;
		}

		aux = ptr;

		//Mezclar los dos monticulos por orden de grado
		while (x != nullptr && y != nullptr) {

			if (x->_degree < y->_degree) {
				aux->_sibling = x;
				x = x->_sibling;
			}
			else {
				aux->_sibling = y;
				y = y->_sibling;
			}

			aux = aux->_sibling;
		}

		// Si que resto de algun monticulo sin mezclar
		if (x != nullptr) {
			aux->_sibling = x;
		}
		else if(y != nullptr) {
			aux->_sibling = y;
		}
		else {
			aux->_sibling = nullptr;
		}

		this->_head = ptr;
	}

	void binomialUnion(monticuloBinomial const& h) {

		if (h._head != nullptr) {

			if (this->_head == nullptr) {

				this->_head = h._head;
				this->_min = h._min;
			}
			else {
				
				this->merge(h._head);

				if (this->_head != nullptr) {

					Nodo* prev_x = nullptr;
					Nodo* x = this->_head;
					Nodo* next_x = x->_sibling;

					while (next_x != nullptr) {

						if ((x->_degree != next_x->_degree) || ((next_x->_sibling != nullptr) 
							&& next_x->_sibling->_degree == x->_degree)) { // Caso1 y Caso2

							prev_x = x;
							x = next_x;
						}
						else{
							if (x->_elem <= next_x->_elem) { // Caso3
								x->_sibling = next_x->_sibling;
								binomialLink(next_x, x);
							}
							else {							// Caso 4
								if (prev_x == nullptr)
									this->_head = next_x;
								else
									prev_x->_sibling = next_x;
								binomialLink(x, next_x);
								x = next_x;
							}
						}

						next_x = next_x->_sibling;
					}
				}

				if (h._min->_elem < this->_min->_elem) { // actualizar el minimo
					this->_min = h._min;
				}
			}
		}
	}

	void binomialDecrementarClave(Nodo * const x, T const & k) {

		if (k > x->_elem) {
			throw domain_error("El nuevo valor es mas grande que el actual");
		}

		x->_elem = k;
		Nodo* y = x;
		Nodo* z = y->_parent;

		// flotarlo
		while (z != nullptr && y->_elem < z->_elem){

			T aux = y->_elem;
			y->_elem = z->_elem;
			z->_elem = aux;

			y = z;
			z = y->_parent;
		}

		if (this->_min == nullptr || this->_min->_elem > k) {
			this->_min = x;
		}

		if (this->_head == nullptr) {
			this->_head = x;
		}
	}

	bool binomialEliminarClave(Nodo* const& x) {

		this->binomialDecrementarClave(x, INT16_MIN);

		T minimo = this->_min->_elem;

		if (minimo == this->binomialExtraerMin()) {
			return true;
		}
		
		return false;
	}

	/*Nodo* buscarClave(const int& k) {

		return buscarClaveAux(this->_head, k);
	}

	Nodo* buscarClaveAux(Nodo* x, const int& k) {
		Nodo* p = nullptr;
		if (x->_elem == k) {
			return x;
		}
		if (x->_child != nullptr && p == nullptr)
			p = buscarClaveAux(x->_child, k);
		if (x->_sibling != nullptr && p == nullptr)
			p = buscarClaveAux(x->_sibling, k);

		return p;
	}*/
	
	unordered_map<int, monticuloBinomial<T>* > heaps;

	unordered_map <T, pair<int, Nodo*>> elemsMap;


public:

	//Insertar un elemento en un nuevo monticulo
	void insertarNuevoMonticulo(T const& e) {
		
		auto aux = elemsMap.find(e);
		if (aux == elemsMap.cend()) {
			monticuloBinomial<T>* H = new monticuloBinomial<T>();
			Nodo* aux = H->insert(e);
			heaps.insert({ heaps.size(), H });
			elemsMap.insert({ e, { heaps.size(), aux } });
		}
		else {
			throw domain_error("La clave ya exite");
		}
	}

	// insertar en un monticulo dado
	void insertar(int const& n, T const& e) {

		if (!monticuloValido(n)) {
			throw domain_error("Monticulo no existe\n");
		}
		auto aux = elemsMap.find(e);

		if (aux == elemsMap.cend()) {
			Nodo* aux = heaps[n]->insert(e);
			elemsMap.insert({ e, { heaps.size(), aux } });
			//heaps[n]->binomialBuscarMin();
		}
		else {
			throw domain_error("La clave ya exite");
		}
		
	}

	T const& minimo(int const& n) {

		if (!esValido(n)) {
			throw domain_error("Monticulo no valido\n");
		}

		return heaps[n]->minimo();
	}

	T extraerMin(int const& n) {
		if (!esValido(n)) {
			throw domain_error("Monticulo no valido o Vacio\n");
		}
		T aux = heaps[n]->binomialExtraerMin();
		elemsMap.erase(aux);

		if (heaps[n]->empty()) {
			heaps.erase(n);
		}

		return aux;
	}

	void unir(int const& n1, int const& n2) {
		if (!esValido(n1)) {
			throw domain_error("Monticulo 1 no valido\n");
		}
		if (!esValido(n2)) {
			throw domain_error("Monticulo 2 no valido\n");
		}

		if (n1 == n2) {
			throw domain_error("Monticulos iguales\n");
		}
		
		heaps[n1]->binomialUnion(*heaps[n2]);
		heaps.erase(n2);
	}

	void decrementarClave(int const& n, int const& e, T const& k) {
		if (!esValido(n)) {
			throw domain_error("Monticulo no valido\n");
		}
		
		auto aux = elemsMap.find(e);

		if (aux == elemsMap.cend()) {
			throw domain_error("No existe la clave\n");
		}

		auto aux2 = elemsMap.find(k);

		if (aux2 == elemsMap.cend()) {
			auto nodo = aux->second.second;
			heaps[n]->binomialDecrementarClave(nodo, k);
			auto a = elemsMap[e];
			elemsMap.erase(e);

			elemsMap.insert({ k,a });
		}
		else {
			throw domain_error("El valor que quieres decrementar ya existe\n");
		}
		
	}

	bool eliminarClave(int const& n, T const& e) {

		if (!esValido(n)) {
			throw domain_error("Monticulo no valido\n");
		}

		auto aux = elemsMap.find(e);

		if (aux == elemsMap.cend()) {
			throw domain_error("No existe la clave\n");
		}
		
		auto nodo = aux->second.second;
		elemsMap;
		T min = heaps[n]->minimo();
		int ret = heaps[n]->binomialEliminarClave(nodo);
		
		elemsMap[min] = {heaps.size(), heaps[n]->_min };
		elemsMap.erase(e);
		if (heaps[n]->empty()) {
			heaps.erase(n);
		}
		
		return ret;
	}

	void imprimirAux(Nodo* nodo, Nodo* prev, int direccion) {

		while (nodo != nullptr) {
			if (direccion == 1) {
				cout << nodo->_elem << "(" << nodo->_degree << ")" <<
					" es Hijo de " << prev->_elem <<'\n';
			}
			else {
				cout << nodo->_elem << "(" << nodo->_degree << ")" <<
					" es hermano de " << prev->_elem << '\n';
			}

			if (nodo != nullptr) {
				imprimirAux(nodo->_child, nodo, 1);
			}

			prev = nodo;
			nodo = nodo->_sibling;
			direccion = 2;
		}
	}

	void imprimir(const int& n) {
		if (!esValido(n)){
			throw domain_error("Monticulo no valido o Vacio\n");
		}
		Nodo* p;
		p = heaps[n]->_head;
		int i = 0;
		while (p != nullptr)
		{
			i++;
			cout << p->_elem << "("<< p->_degree<<  ")" << " es la raiz\n";
			imprimirAux(p->_child, p, 1);
			/*if (p->_sibling != nullptr)
				cout << "-->";*/
			p = p->_sibling;
		}
		cout << endl;
	}

	bool esValido(int const& n) {
		return (monticuloValido(n) && !heaps[n]->empty());
	}

	bool monticuloValido(int const& n) {
		return (n >= 0 && n < heaps.size());
	}

	bool empty() {
		return this->_head == nullptr;
	}

	int heapsSize() {
		return heaps.size();
	}
	
};

#endif