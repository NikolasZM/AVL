#include <iostream>
#include <Windows.h>
#include <string>
using namespace std;

//////////////////////////////////////----------------------------Pila

template<class T>
class NodoP {
public:
	T valor;
	NodoP<T>* next;

	NodoP(T v, NodoP<T>* n = nullptr) {
		valor = v;
		next = n;
	}
};

template<class T>
class Pila {
public:
	NodoP<T>* top;

	Pila() {
		top = nullptr;
	}
	void push(T v);
	bool pop(T& v);
};

template<class T>
void Pila<T>::push(T v) {
	top = new NodoP<T>(v, top);
}

template<class T>
bool Pila<T>::pop(T& v) {
	if (!top) {
		return false;
	}
	else {
		v = top->valor;
		NodoP<T>* tmp = top;
		top = top->next;
		delete tmp;
		return true;
	}
}

//////////////////////////////////////----------------------------Print

int auxX{ 0 };

void gotoxy(int x, int y) {
	HANDLE hcon;
	hcon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hcon, dwPos);
}

//////////////////////////////////////----------------------------Arbol

struct Nodo
{
	Nodo(int v)
	{
		val = v;
		nodes[0] = nodes[1] = 0;
	}
	int val;
	Nodo* nodes[2];
};

class Arbol
{
public:

	Nodo* root;
	bool m_b;
	Pila<Nodo**> pilaNodos;

	Arbol();
	bool Insert(int x);
	bool Remove(int x);
	bool Find(int x, Nodo**& p);
	Nodo** Rep(Nodo** p);
	void InOrder(Nodo* n);
	void InOrder();
	void Print(Nodo* ,int auxY);
	void Print(int auxY);
	void recoPila();
	int conteo(Nodo* n);
	int resta(Nodo*n);
	void identi(Nodo** &n, Pila<int> p);
	void balanceo(Nodo** &n, int opt);
	~Arbol();
};

Arbol::Arbol()
{
	root = 0;
	m_b = 0;
}

Arbol::~Arbol() {}

bool Arbol::Find(int x, Nodo**& p)
{
	for (p = &root;
		*p && (*p)->val != x;
		p = &((*p)->nodes[(*p)->val < x])) {
		pilaNodos.push(p);
	}
	return *p && (*p)->val == x;
}

bool Arbol::Insert(int x)
{
	Nodo** p;
	if (Find(x, p)) return 0;
	*p = new Nodo(x);
	pilaNodos.push(p);
	recoPila();
	return 1;
}
bool Arbol::Remove(int x)
{
	Nodo** p;
	if (!Find(x, p)) return 0;
	if ((*p)->nodes[0] && (*p)->nodes[1])
	{
		Nodo** q = Rep(p);
		(*p)->val = (*q)->val;
		p = q;
	}
	Nodo* t = *p;
	*p = (*p)->nodes[(*p)->nodes[0] == 0];
	delete t;
	return 1;
}

Nodo** Arbol::Rep(Nodo** q)
{
	m_b = !m_b;
	q = &((*q)->nodes[!m_b]);
	while ((*q)->nodes[m_b])
		q = &((*q)->nodes[m_b]);
	return q;
}

void Arbol::InOrder(Nodo* n)
{
	if (!n) return;
	InOrder(n->nodes[0]);
	cout << n->val << " ";
	InOrder(n->nodes[1]);
}

void Arbol::InOrder()
{
	InOrder(root);
	cout << endl;
}

void Arbol::Print(Nodo* n, int auxY) {
	if (n == NULL) {
		return;
	}
	auxX += 5;
	Print(n->nodes[0], auxY + 2);
	gotoxy(5 + auxX - auxY,  auxY);
	cout << n->val << endl;
	Print(n->nodes[1], auxY + 2);
}

void Arbol::Print(int auxY) {
	auxX = 0;
	Print(root, auxY);
	gotoxy(1, auxY*4);
}


void Arbol::recoPila() {
	int value;
	Pila<int> valores;
	for (Nodo** tmp; pilaNodos.top;) {
		if (pilaNodos.pop(tmp)) {
			value = resta(*tmp);
			//cout << "valor=" << value << "\n";
			valores.push(value);
		}
		if (value > 1 || value < -1) {
			identi(tmp,valores);
		}
	}
}

void Arbol::identi(Nodo** &n, Pila<int> p) {
	int v;
	string valores;
	for (int i{ 0 }; i < 2; i++) {
		p.pop(v);
		valores += to_string(v);
	}
	if (valores == "21") v = 0;
	if (valores == "-2-1") v = 1;
	if (valores == "2-1") v = 2;
	if (valores == "-21") v = 3;
	balanceo(n, v);
}

int Arbol::resta(Nodo*n) {
	int hi{ 0 }, hd{ 0 };
	hi = conteo(n->nodes[0])+1;
	hd = conteo(n->nodes[1])+1;
	return hi - hd;
}

int Arbol::conteo(Nodo* n) {
	if (n == nullptr) return -1;
	int hi{ 0 }, hd{ 0 };
	if (n->nodes[0]) hi = conteo(n->nodes[0]) + 1;
	if (n->nodes[1]) hd = conteo(n->nodes[1]) + 1;
	return max(hi,hd);
}

void Arbol::balanceo(Nodo** &n,int opt) {
	Nodo* tmp = *n;
	switch (opt) {
	case 0:
		*n = (*n)->nodes[0];
		tmp->nodes[0] = (*n)->nodes[1];
		(*n)->nodes[1] = tmp;
		break;
	case 1:
		*n = (*n)->nodes[1];
		tmp->nodes[1] = (*n)->nodes[0];
		(*n)->nodes[0] = tmp;
		break;
	case 2:
		*n = tmp->nodes[0]->nodes[1];
		tmp->nodes[0]->nodes[1] = (*n)->nodes[0];
		(*n)->nodes[0] = tmp->nodes[0];
		tmp->nodes[0] = (*n)->nodes[1];
		(*n)->nodes[1] = tmp;
		break;
	case 3:
		*n = tmp->nodes[1]->nodes[0];
		tmp->nodes[1]->nodes[0] = (*n)->nodes[1];
		(*n)->nodes[1] = tmp->nodes[1];
		tmp->nodes[1] = (*n)->nodes[0];
		(*n)->nodes[0] = tmp;
		break;
	default:
		break;
	}


}

////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	Arbol t;

	t.Insert(50); t.Insert(30); t.Insert(60);
	t.Insert(80); t.Insert(55); t.Insert(11);
	t.Insert(33); t.Insert(5); t.Insert(14);
	t.Insert(31); t.Insert(48);	 

	//t.Print(4);
	
	t.Insert(4); t.Insert(3); t.Insert(1);
	
	t.Print(4);
}