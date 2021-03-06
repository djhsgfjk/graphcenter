#include<iostream>
#include<climits>
using namespace std;

struct center
{
	int a;
	int b;
	int e;
	int s0;
};

class Graph
{
	int m; // кол-во вершин
	int n; //кол-во рёбер
	int* v; // веса графов 
	int** g; // матрица смежности графа
	int** d; //матрица расстояний
public:
	Graph(int m);
	Graph(const Graph& ob); // конструктор копирования
	~Graph();
	void InputVertices();
	void Input();
	void InputTest1();
	void PrintGraph();
	void PrintVertices();
	void PrintDistances();
	int GetM() { return m; }
	int GetN() { return n; }
	// Алгоритм Флойда поиска кратчайших путей в графе
	void ShortestPaths();

	// Поиск центров на вершинах по матрице расстояний
	void CVertexes();
};

Graph::Graph(int m)
{
	this->m = m;
	n = 0;
	v = new int[m];
	g = new int* [m];
	d = new int* [m];
	for (int i = 0; i < m; i++)
	{
		v[i] = 1;
		g[i] = new int[m];
		d[i] = new int[m];
		for (int j = 0; j < m; j++)
		{
			g[i][j] = (i == j) ? 0 : INT_MAX;
			d[i][j] = (i == j) ? 0 : INT_MAX;
		}
	}
}

Graph::Graph(const Graph& ob)
{
	m = ob.m;
	n = ob.n;
	v = new int[m];
	g = new int* [m];
	d = new int* [m];
	for (int i = 0; i < m; i++)
	{
		v[i] = ob.v[i];
		d[i] = new int[m];
		g[i] = new int[m];
	}
	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
		{
			g[i][j] = ob.g[i][j];
			d[i][j] = ob.d[i][j];
		}
}

Graph::~Graph()
{
	for (int i = 0; i < m; i++)
	{
		delete[] g[i];
		delete[] d[i];
	}
	delete[] v;
	delete[] g;
	delete[] d;
}

void Graph::PrintGraph()
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
			if (g[i][j] != INT_MAX)
				cout << g[i][j] << "\t";
			else
				cout << "-\t";
		cout << endl;
	}
	cout << endl;
}

void Graph::PrintVertices()
{
	for (int i = 0; i < m; i++)
		cout << v[i];
	cout << endl << endl;
}

void Graph::PrintDistances()
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
			if (d[i][j] != INT_MAX)
				cout << d[i][j] << "\t";
			else
				cout << "-\t";
		cout << endl;
	}
	cout << endl;
}

void Graph::InputVertices()
{
	for (int i = 0; i < m; i++)
		cin >> v[i];
	cout << endl;
}

void Graph::Input()
{
	cout << "Если ребро отсутствует, ввeдите -1" << endl;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> g[i][j];
			if (g[i][j] == -1)
				g[i][j] = INT_MAX;
			if (g[i][j] != g[j][i])
				n++;
		}
		cout << endl;
	}

	ShortestPaths();
}

void Graph::ShortestPaths()
{
	int** paths = new int* [m];
	int** vertexes = new int* [m];
	for (int i = 0; i < m; i++)
	{
		paths[i] = new int[m];
		vertexes[i] = new int[m];
	}

	for (int i = 0; i < m; i++)
		for (int j = 0; j < m; j++)
		{
			paths[i][j] = g[i][j];
			if (g[i][j] != INT_MAX)
				vertexes[i][j] = j;
			else
				vertexes[i][j] = -1;
		}

	for (int k = 0; k < m; k++) 
		for (int i = 0; i < m; i++) 
			for (int j = 0; j < m; j++) 
				if (i != j && j != k && i != k)
					if (paths[i][k] != INT_MAX && paths[k][j] != INT_MAX)
						if (paths[i][k] + paths[k][j] < paths[i][j])
						{
							paths[i][j] = paths[i][k] + paths[k][j];
							vertexes[i][j] = vertexes[i][k];
						}
	d = paths;
}

// Поиск центров на вершинах по матрице расстояний
void Graph::CVertexes()
{
	//В массиве s храним числа разделений
	//В min - минимальное число разделений
	int* s = new int[m];
	for (int i = 0; i < m; i++)
		s[i] = 0;
	int min = INT_MAX;
	for (int i = 0; i < m; i++)
	{
		//Находим набольшее расстояние от вершины i до остальных вершин графа, то есть, находим чило разделения 
		for (int j = 0; j < m; j++)
		{
			if (d[i][j] > s[i])
			{
				s[i] = d[i][j];
			}

		}
		//Находим наименьшее число разделений
		if (s[i] < min)
			min = s[i];
	}

	//Находим и выводим центры
	for (int i = 0; i < m; i++)
	{
		if (s[i] == min)
			cout << "Center: " << i + 1 << endl << "Radius: " << s[i] << endl << endl;
	}

}

void Graph::InputTest1()
{
	g[0][1] = INT_MAX;
	g[0][2] = 8;
	g[0][3] = 6;
	g[0][4] = 2;
	g[1][0] = INT_MAX;
	g[1][2] = 2;
	g[1][3] = INT_MAX;
	g[1][4] = 6;
	g[2][0] = 8;
	g[2][1] = 2;
	g[2][3] = INT_MAX;
	g[2][4] = INT_MAX;
	g[3][0] = 6;
	g[3][1] = INT_MAX;
	g[3][2] = INT_MAX;
	g[3][4] = 4;
	g[4][0] = 2;
	g[4][1] = 6;
	g[4][2] = INT_MAX;
	g[4][3] = 4;

	n = 10;
	ShortestPaths();
}

int main()
{
	Graph gr(5);
	gr.InputTest1();
	gr.PrintGraph();
	gr.PrintDistances();
	gr.PrintVertices();
	gr.CVertexes();

	return 0;
}