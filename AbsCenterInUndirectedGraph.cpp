#include<iostream>
#include<climits>
#include<vector>

using namespace std;

struct center
{
	int a;
	int b;
	int e;
	int s;
};

struct center_on_vertex
{
	int a;
	int s;
};

class Graph
{
	int m; // кол-во вершин
	int n; // кол-во граней
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

	//Алгоритм Хакими поиска абсолютного центра
	//I этап
	vector<center_on_vertex> Centers_on_vertexes(); // поиск центров на вершинах
	int* Find_S_for_all_Y_on_AB(int a, int b); // поиск числе разделений для точек на ребре (a, b)
	vector<center> LocalCenters(int a, int b); // поиск локальных центров на ребер (a, b)
	vector<center> Centers_on_edges(); // поиск абсолютных центров на гранях, не включая вершины
	//II этап
	void AbsCenters(); // поиск и вывод абсолютных центров
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
	cout << "Если ребро отсутствует, вводите 0" << endl;
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < m; j++)
		{
			cin >> g[i][j];
			if (g[i][j] == 0 && i != j)
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

// Следующие функции осущестают алготритм Хакими посика абсолютных центров в неоригентрованном графе

// поиск центров на вершинах
vector<center_on_vertex> Graph::Centers_on_vertexes() 
{
	vector<center_on_vertex> Centers;

	// Ищем числа разделений для вершин и минимальное из них
	// В массиме s будем хранить числа разделений. s[x] - число разделений вершины x
	// в min будем хранить минимальное число разделений
	int* s = new int[m];
	for (int x = 0; x < m; x++)
		s[x] = 0;

	int T;
	int mins = INT_MAX; 
	for (int x = 0; x < m; x++)
	{
		// Находим максимальное расстояние от вершины x до остальных вершин графа, то есть, находим число разелений s[x]
		for (int i = 0; i < m; i++)
		{
			T = v[i] * d[x][i];
			if (T > s[x])
				s[x] = T;
		}
		// Ищем минимальное число разделений
		if (s[x] < mins)
			mins = s[x];
	}

	// Находим центры - вершины, с наименьшим числом разделений
	center_on_vertex c;
	for (int x = 0; x < m; x++)
		if (s[x] == mins)
		{
			c.a = x;
			c.s = s[x];
			Centers.push_back(c);
		}

	return Centers;
}

// поиск числе разделений для точек на ребре (a, b)
int* Graph::Find_S_for_all_Y_on_AB(int a, int b)
{
	// В массив s будем хранить числа разделений точек на ребре (a, b)
	int* s = new int[g[a][b]];
	for (int e = 0; e < g[a][b]; e++)
		s[e] = 0;

	//Будем рассматривать точки, находящиеся на расстоянии 1, по всей длине ребра
	int T1; int T2;
	for (int e = 1; e < g[a][b]; e++)
	{
		for (int i = 0; i < m; i++)
		{
			// Будем сравнивать пути в две стороны по ребру (a, b), выбирая нименьший
			T1 = v[i] * (e + d[b][i]);
			T2 = v[i] * (g[a][b] - e + d[a][i]);
			if (T1 < T2)
			{
				if (T1 > s[e])
					s[e] = T1;
			}
			else
				if (T2 > s[e])
					s[e] = T2;
		}
	}

	return s;
}

// поиск локальных центров на ребер (a, b)
vector<center> Graph::LocalCenters(int a, int b)
{

	// s - числа разделений точек на ребре (a, b)
	int* s = Find_S_for_all_Y_on_AB(a, b);

	int min = INT_MAX;
	center y;
	y.a = a; y.b = b;
	vector<center> ListOfY;
	// Находим наименьшее число разделений точек на ребре (a, b)
	for (int e = 1; e < g[a][b]; e++)
		if (s[e] < min)
			min = s[e];
	// Находим локальные центры на ребре (a, b)
	for (int e = 1; e < g[a][b]; e++)
		if (s[e] == min)
		{
			y.e = e;
			y.s = s[e];
			ListOfY.push_back(y);
		}

	return ListOfY;
}

// поиск абсолютных центров на гранях, не включая вершины
vector<center> Graph::Centers_on_edges()
{
	center y;
	vector<center> CentersOnAB;
	vector<center> Centers;
	int min = INT_MAX;
	// Поллучаем спиок всех локальных центров Centers на ребрах графа
	for (int a = 0; a < m; a++)
		for (int b = m - 1; b > a; b--)
			if (g[a][b] < INT_MAX)
			{
				CentersOnAB = LocalCenters(a, b);
				if (CentersOnAB.front().s < min)
					min = CentersOnAB.front().s;
				while (!CentersOnAB.empty())
				{
					y = CentersOnAB.back();
					Centers.push_back(y);
					CentersOnAB.pop_back();
				}
			}
	
	// Ищем наименьшие из локальных центров
	vector<center> Abs;
	int t = Centers.size();
	for (int i = 0; i < t; i++)
		if (Centers[i].s == min)
			Abs.push_back(Centers[i]);

	return Abs;
}

// поиск и вывод абсолютных центров
void Graph::AbsCenters()
{
	vector<center_on_vertex> Centers = Centers_on_vertexes(); // Центры на верщинах
	vector<center> AbsCenters = Centers_on_edges(); // Центры на гранях

	// Ищем абсолютные центры на врешинах и гранях
	center_on_vertex x;
	center y;
	x = Centers.front();
	y = AbsCenters.front();
	if (x.s > y.s)
		Centers.clear();
	else
		if (y.s > x.s)
			AbsCenters.clear();

	// Вывод
	while (!Centers.empty())
	{
		x = Centers.back();
		cout << "Center:";
		cout << x.a + 1 << endl;
		cout << "Radius:" << x.s << endl;
		Centers.pop_back();
	}

	while (!AbsCenters.empty())
	{
		y = AbsCenters.back();
		cout << "Absolut Center:";
		cout << "(" << y.a + 1 << ", " << y.b + 1 << ") " << y.e << endl;
		cout << "Radius:" << y.s << endl;
		AbsCenters.pop_back();
	}
}

int main()
{
	int m = 5;
	Graph gr(m);
	gr.InputTest1();
	gr.PrintGraph();
	gr.PrintDistances();
	gr.PrintVertices();

	gr.AbsCenters();

	return 0;
}

