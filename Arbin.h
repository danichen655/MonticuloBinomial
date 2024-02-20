/** ADT Arbin: árboles binarios.
 @file Arbin.h */

#ifndef __ARBIN_H
#define __ARBIN_H

#include <cstddef>
#include "lista.h"

using namespace std;

/**
Implementación del TAD Arbol binario mediante nodos y punteros en
memoria dinámica

Las operaciones son:

- ArbolVacio: -> Arbin. Generadora que crea un árbol vacío.
Implementada mediante un constructor sin parámetros.
- ArbolSimple: Elem -> Arbin. Generadora que crea un árbol con
un único nodo. Implementada mediante un constructor con parámetro
el valor a poner en el nodo.
- Cons: Arbin, Elem, Arbin -> Arbin. Generadora que construye un 
árbol con un hijo izquierdo, un nodo raiz, y un hijo derecho.
Implementada mediante un constructor que toma como primer argumento
el hijo izquierdo, como segundo el valor a poner en la raiz, y como
tercer argumento el hijo derecho. 
- raiz: Arbin -> Elem. Observadora (parcial) que devuelve el valor
de la raiz (no definida para árboles vacíos)
- hijoIz: Arbin -> Arbin. Observadora (parcial) que devuelve el
hijo izquierdo (no definida para árboles vacíos)
- hijoDer: Arbin -> Arbin. Observadora (parcial) que devuelve el
hijo derecho (no definida para árboles vacíos)
- esVacio: Arbin -> bool. Observadora que determina si un árbol 
es vacío o no

@author Adaptada por Mercedes Gómez y José Luis Sierra a partir de una implementación de  Marco Antonio Gómez Martín, 2012
*/

/// Excepción generada por algunos métodos
class EArbolVacio{};

template <class T>
class Arbin {
   public:	  
       /** Constructor; operación ArbolVacío */
      Arbin() {
	    _ra = NULL;
      };
	  
       /** Constructor; operación ArbolSimple (construye un árbol con 
	       un único nodo)
	   
   	       @param elem Elemento a alojar en la raíz del árbol generado */
      Arbin(const T &elem) {
	    _ra = new Nodo(elem);
	    _ra->addRef();
      };

       /** Constructor; operación Cons (construye un árbol con un hijo izquierdo,
	       una raíz y un hijo derecho)
	       
   	       @param iz El hijo izquierdo 
    	   @param elem El valor a poner en la raíz	
 		   @param dr El hijo derecho
	    */	  
      Arbin(const Arbin &iz, const T &elem, const Arbin &dr) {
	    _ra = new Nodo(iz._ra,elem,dr._ra);
	    _ra->addRef();
      };
	  
	  /** Devuelve el valor alojado en la raíz del árbol    
	  
	      error: Falla si el árbol está vacío (excepción EArbolVacio)

          @return El valor en la raíz 
	   */	  
      const T& raiz() const { 
        if(_ra == NULL) {
	       throw EArbolVacio();   
        }
        return _ra->_elem;       
      }
	  
	  /** Devuelve el hijo izquierdo del árbol    
	  
	      error: Falla si el árbol está vacío (excepción EArbolVacio)

          @return El hijo izquierdo
	   */	  
      Arbin hijoIz() const {  
		if(_ra == NULL) {
	      throw EArbolVacio();   
        }
        return Arbin(_ra->_iz);
        }  
	  

	  /** Devuelve el hijo derecho del árbol    
	  
	      error: Falla si el árbol está vacío (excepción EArbolVacio)

          @return El hijo derecho
	   */	  
      Arbin hijoDer() const {  
		if(_ra == NULL) {
	      throw EArbolVacio();  
        }
        return Arbin(_ra->_dr);
        }  

	  /** Determina si el árbol está vacío.
	  
          @return true si el árbol está vacío; false en otro caso
	   */	  	  
      bool esVacio() const {
        return _ra == NULL; 	
      }

	  
	  /** Operador de igualdad entre árboles binarios

          @param el árbol cuya igualdad con el receptor quiere comprobarse
		  
          @return true si los árboles son iguales; false en otro caso
	   */	  	  	  	 	  	  
      bool operator==(const Arbin& a) const {  
        return sonIguales(_ra, a._ra); 
      }

	  /** Operador de asignación

          @param el árbol que quiere asignarse al receptor
		  
          @return una referencia al objeto receptor
	   */	  	  	  	 	  	  	  
      Arbin& operator=(const Arbin& a) {
		  if (this != &a) {   
			  libera(_ra);
			  if (a._ra != NULL) a._ra->addRef();
			  _ra = a._ra;
		  }
		  return *this;
      }

	  /** Constructor de copia

          @param el árbol que quiere copiarse
	   */	  	  	  	 	  	  	  	  
      Arbin(const Arbin& a) {
	   if(a._ra != NULL) a._ra->addRef();
	   _ra = a._ra;
      }

	  /** Destructor
	   */	  	  	  	 	  	  	  	  	  
      ~Arbin() {
        libera(_ra);	
      }
      Lista<T> preorden() {
          Lista<T> ret = Lista<T>();
          Arbin::preorden(this->_ra,ret);
          return ret;
      }
	  
   private:
       /** 
	    Clase Nodo que almacena internamente un nodo de un árbol. Para ello
		almacena: (i) el valor del nodo, (ii) un puntero al nodo raíz del árbol
		izquierdo, y (iii) un puntero al nodo raíz del árbol derecho.
		También se lleva la cuenta del número de punteros que apuntan a dicho
		nodo, para permitir la representación eficiente de árboles, permitiendo
		la compartición de estructura entre los mismos. 
		*/
      class Nodo {
		public:
		    /** Constructor por defecto
			*/
		   Nodo() : _iz(NULL), _dr(NULL), _nrefs(0) {}  
            /** Constructor; construye un nodo para un árbol unitario (sin
			    hijo izquierdo, sin hijo derecho)
				
				@param el valor a alojar en el nodo
             */ 				
           Nodo(const T &elem) : _elem(elem),_iz(NULL),_dr(NULL), _nrefs(0) {}
            /** Constructor; construye un nodo para un árbol con
			    hijo izquierdo e hijo derecho
				
				@param iz,  el hijo izquierdo
                @param elem, el valor a alojar en la raíz				
				@param dr, el hijo derecho
             */ 				
           Nodo(Nodo *iz, const T &elem, Nodo *dr) :
             _elem(elem), _iz(iz), _dr(dr), _nrefs(0) {
             if (iz != NULL) iz->addRef();
             if (dr != NULL) dr->addRef();
           }
            /** Contabiliza una nueva referencia al nodo
             */ 				
           void addRef() {_nrefs++;};
            /** Decrementa el contador de referencias al nodo
             */ 				
           void rmRef()  {_nrefs--;}
           T _elem;         // valor del nodo
           Nodo* _iz;       // hijo izquierdo
           Nodo* _dr;       // hijo derecho
           int _nrefs;      // número de referencias al nodo
	  };
	  	 /** Constructor privado; crea un nuevo árbol con una raíz dada
      
         @param ra, el nodo raíz del árbol
      */		 
	  Arbin(Nodo* ra) {
		  _ra = ra;
		  if (_ra != NULL) _ra->addRef();
	  }

	  /** Liberación de la estructura de nodos, teniendo
	      en cuenta la compartición.
      
         @param n, puntero a la raíz de la estructura a liberar
      */		 
     static void libera(Nodo *n) {
	    if (n != NULL) {
	      n->rmRef();
	      if (n->_nrefs == 0) {
	        libera(n->_iz);
            libera(n->_dr);
            delete n; 	  
	      }
	    }  
     }

     Lista<T> recorrido_por_niveles(Nodo * raiz) {
         Lista<Nodo *> Q = Lista<T>();
         Lista<T> R = Lista<T>();
         if(raiz == nullptr)return R;
         Q.pon_final(raiz);
         while (!Q.esVacia()){
             Nodo * prim = Q.primero();
             Q.quita_ppio();
             if(prim->_iz != nullptr) Q.pon_final(prim->_iz);
             if(prim->_dr != nullptr) Q.pon_final(prim->_dr);
             R.pon_final(prim->_elem);
         }
         return R;
     }
     static void preorden(Nodo * raiz,Lista<T> & ret) {
         if(raiz == nullptr) return;

         ret.pon_final(raiz->_elem);
         preorden(raiz->_iz,ret);
         preorden(raiz->_dr,ret);

     }


	  /** Comprobación de la igualdad entre dos estructuras de nodos.
      
         @param no, primera estructura
		 @param n1, segunda estructura
		 @return true si los punteros apuntan a estructuras que representan 
		         el mismo árbol, false en otro caso
      */		 


	  Lista<T> recorre_por_niveles() {

	      Lista<T> ret = Lista<T>();
	      Lista<T> Q = Lista<T>();
	      if(this->_ra == nullptr)
	          return ret;
	      Q.pon_ppio(this->_ra);
	      while(!Q.esVacia()) {
	          Nodo * act = Q.primero();
	          if(act->_iz != nullptr)
	              Q.pon_final(act->_iz);
	          if(act->_dr != nullptr)
	              Q.pon_final(act->_dr);
	          ret.pon_final(act->_elem);
	          Q.quita_ppio();
	      }
	      return ret;
	  }

	 static bool sonIguales(const Nodo *no, const Nodo* n1) {
		 return no == n1  
			       ||
			    no != NULL && n1 != NULL &&
			    no->_elem == n1->_elem &&
			    sonIguales(no->_iz, n1->_iz) &&
			    sonIguales(no->_dr, n1->_dr);
		
     }
         
         bool equilibradoAux(int & d)
         {
             if(this->esVacio())
             {d = 0;return true;}
             if(this->hijoIz().esVacio() && this->hijoDer().esVacio())
             {
                 d = 0;
                 return true;
             }
             
             int tallaiz;
             int tallaDerecha;
             
             if(this->hijoIz().equilibradoAux(tallaiz) && this->hijoDer().equilibradoAux(tallaDerecha))
             {
                 d = tallaiz + tallaDerecha +1;
                 return tallaiz == tallaDerecha;
             }
             
             return false;
             
         }
         
         bool equilibrado()
         {
             
             int d;
             this->equilibradoAux(d);
         }


         static void preorden(const Nodo * n)
         {
	        if(n != nullptr)
            {
	            std::cout << n->_elem;
	            preorden(n->_iz);
	            preorden(n->_dr);
            }
         }


	 Nodo* _ra; // puntero al nodo raíz
	 	  
};



#endif

