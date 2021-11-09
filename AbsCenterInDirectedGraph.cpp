#include<iostream>
#include<climits>
#include<vector>

using namespace std;

struct center
{
	int a;
	int b;
	int e;
	int s0;
	int st;
};

struct center_on_vertex
{
	int a;
	int s0;
	int st;
};

class Graph
{
	int m; // кол-во вершин
	int n;
	int* v; // веса графов 
	int** g; // матрица смежности графа
	int** d; //матрица расстояний
public:
	Graph(int m);
	Graph(const Graph& ob); // конструктор копирования
	~Graph();
	void InputVertices();
	void Input();
	void InputTest0(); //неориентированный граф с центром на грани
	void InputTest1(); //ориентированный граф с центром в вершине
	void PrintGraph();
	void PrintDistances();
	void PrintVertices();
	int GetM() { return m; }
	int GetN() { return n; }
	// Алгоритм Флойда поиска кратчайших путей в графе
	void ShortestPaths();

	//Метод Хакими для нахождения центров
	//Находим числа центры среди вершин
	void Centers_on_vertexes(vector<center_on_vertex>& InCenters, vector<center_on_vertex>& ExCenters, vector<center_on_vertex>& Centers);
	//Находим внешние и внутренние числа разделений для всех точек Y на грани (a, b)
	int* Find_S0_for_all_Y_on_AB(int a, int b, bool undirected);
	int* Find_St_for_all_Y_on_AB(int a, int b, bool undirected);
	//Находим локальные центры на грани (a, b)
	void Local_centers(int a, int b, vector<center>& mins0, vector<center>& minst, vector<center>& min);
	//Находим центры среди граней графа
	void Centers_on_edges(vector<center>& InAbsCenters, vector<center>& ExAbsCenters, vector<center>& AbsCenters);
	//Находим абсолютные центры 
	void Abs_centers();
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

void Graph::PrintVertices()
{
	for (int i = 0; i < m; i++)
		cout << v[i];
	cout << endl << endl;
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

void Graph::InputTest0()
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

void Graph::InputTest1()
{
	g[0][1] = 7;
	g[1][2] = 14;
	g[1][4] = 4;
	g[2][4] = 12;
	g[3][2] = 5;
	g[4][3] = 9;
	g[4][5] = 10;
	g[5][0] = 6;
	g[5][1] = 11;

	v[0] = 5;
	v[1] = 3;
	v[2] = 1;
	v[3] = 7;
	v[4] = 4;
	v[5] = 6;

	n = 9;
	ShortestPaths();
}

void Graph::Centers_on_vertexes(vector<center_on_vertex>& InCenters, vector<center_on_vertex>& ExCenters, vector<center_on_vertex>& Centers)
{
	// Ищем числа разделений для вершин и минимальное из них
	// В массиме s0 будем хранить внешние числа разделений, в st - внутренние.
	int* s0 = new int[m];
	int* st = new int[m];
	for (int x = 0; x < m; x++)
	{
		s0[x] = 0; 
		st[x] = 0;
	}

	int T;
	// в mins0 будем хранить минимальное число внешних разделений, в minst - внутренних, 
	// в mins - минимальную сумму внешнего и внутреннего числа разделений для одной точки, то есть, минимальное расстояние в одну сторону и обратно 
	int mins0 = INT_MAX; 
	int minst = INT_MAX; 
	int mins = INT_MAX;
	for (int x = 0; x < m; x++)
	{
		// Находим максимальное расстояние от вершины x до остальных вершин графа, то есть, находим числа разелений.
		for (int i = 0; i < m; i++)
		{
			T = v[i] * d[x][i];
			if (T > s0[x])
				s0[x] = T;
			T = v[i] * d[i][x];
			if (T > st[x])
				st[x] = T;
		}
		// Ищем минимальные числа разделений
		if (s0[x] < mins0)
			mins0 = s0[x];
		if (st[x] < minst)
			minst = st[x];
		if (s0[x]+st[x] < mins)
			mins = s0[x]+st[x];
	}
	
	// Находим центры 
	center_on_vertex c;
	for (int x = 0; x < m; x++)
	{
		//Внешние центры
		if (s0[x] == mins0)
		{
			c.a = x;
			c.s0 = s0[x];
			c.st = st[x];
			InCenters.push_back(c);
		}
		//Внутрениие центры
		if (st[x] == minst)
		{
			c.a = x;
			c.s0 = s0[x];
			c.st = st[x];
			ExCenters.push_back(c);
		}
		//Центры в две стороны
		if (s0[x]+st[x] == mins)
		{
			c.a = x;
			c.s0 = s0[x];
			c.st = st[x];
			Centers.push_back(c);
		}
	}
	
}


//Находим внешние числа разделений для всех точек Y на грани (a, b)
int* Graph::Find_S0_for_all_Y_on_AB(int a, int b, bool undirected)
{
	// В массив s0 будем хранить внешние числа разделений точек на ребре (a, b)
	int* s0 = new int[g[a][b]];
	for (int e = 0; e < g[a][b]; e++)
		s0[e] = 0;

	//Поиск на неориентированном ребре
	if (undirected)
	{
		int T;
		for (int e = 1; e < g[a][b]; e++)
		{
			for (int i = 0; i < m; i++)
			{
				T = v[i] * (e + d[b][i]);
				if (T > s0[e])
					s0[e] = T;
			}
		}
	}
	//Поиск на ориентированном ребре
	else
	{
		int T1; int T2;
		for (int e = 1; e < g[a][b]; e++)
		{
			for (int i = 0; i < m; i++)
			{
				T1 = v[i] * (e + d[b][i]);
				T2 = v[i] * (g[a][b] - e + d[a][i]);
				if (T1 < T2)
				{
					if (T1 > s0[e])
						s0[e] = T1;
				}
				else
					if (T2 > s0[e])
						s0[e] = T2;
			}
		}
	}
	
	return s0;
}

//Находим внутренние числа разделений для всех точек Y на грани (a, b)
int* Graph::Find_St_for_all_Y_on_AB(int a, int b, bool undirected)
{
	// В массив st будем хранить внутренние числа разделений точек на ребре (a, b)
	int* st = new int[g[a][b]];
	for (int e = 0; e < g[a][b]; e++)
		st[e] = 0;

	//Поиск на неориентированном ребре
	if (undirected)
	{
		int T;
		for (int e = 1; e < g[a][b]; e++)
		{
			for (int i = 0; i < m; i++)
			{
				T = v[i] * (g[a][b] - e + d[i][a]);
				if (T > st[e])
					st[e] = T;
			}
		}
	}
	//Поиск на ориентированном ребре
	else
	{
		int T1; int T2;
		for (int e = 1; e < g[a][b]; e++)
		{
			for (int i = 0; i < m; i++)
			{
				T1 = v[i] * (e + d[b][i]);
				T2 = v[i] * (g[a][b] - e + d[a][i]);
				if (T1 < T2)
				{
					if (T1 > st[e])
						st[e] = T1;
				}
				else
					if (T2 > st[e])
						st[e] = T2;
			}
		}
	}

	return st;
}

// поиск локальных центров на ребер (a, b)
void Graph::Local_centers(int a, int b, vector<center>& mins0, vector<center>& minst, vector<center>& mins)
{
	int* s0 = Find_S0_for_all_Y_on_AB(a, b, g[a][b] != g[b][a]); //Внешние числа разделений ребра (a, b)
	int* st = Find_St_for_all_Y_on_AB(a, b, g[a][b] != g[b][a]); //Внутренние числа разделений ребра (a, b)

	center y;
	y.a = a; y.b = b;
	int min0 = INT_MAX;
	int mint = INT_MAX;
	int min = INT_MAX;
	// Находим наименьшие числа разделений точек на ребре (a, b)
	for (int e = 1; e < g[a][b]; e++)
	{
		if (s0[e] < min0)
			min0 = s0[e];
		if (st[e] < mint)
			mint = st[e];
		if ((s0[e] + st[e]) < min)
			min = (s0[e] + st[e]);
	}
	// Находим локальные центры на ребре (a, b)
	for (int e = 1; e < g[a][b]; e++)
	{
		//Внешние центры
		if (s0[e] == min0)
		{
			y.e = e;
			y.s0 = s0[e];
			y.st = st[e];
			mins0.push_back(y);
		}
		//Внутренние центры
		if (st[e] == mint)
		{
			y.e = e;
			y.s0 = s0[e];
			y.st = st[e];
			minst.push_back(y);
		}
		//Двусторонние центры
		if (s0[e] + st[e] == min)
		{
			y.e = e;
			y.s0 = s0[e];
			y.st = st[e];
			mins.push_back(y);
		}
	}
}

// поиск абсолютных центров на гранях, не включая вершины
void Graph::Centers_on_edges(vector<center>& InAbsCenters, vector<center>& ExAbsCenters, vector<center>& AbsCenters)
{
	vector<center> LocalInAbsCenters; // Локальные внешние центры на ребрах
	vector<center> LocalExAbsCenters; // Локальные внутренние центры на ребрах
	vector<center> LocalAbsCenters; // Локальные дусторонние центры на ребрях

	vector<center> Localmins0;
	vector<center> Localminst;
	vector<center> Localmin;

	int mins0 = INT_MAX;
	int minst = INT_MAX;
	int min = INT_MAX;

	center y;
	// Поллучаем спиок всех локальных центров на ребрах графа
	for (int a = 0; a < m; a++)
		for (int b = 0; b < m; b++)
			if (g[a][b] < INT_MAX)
				if (a < b || g[a][b] != g[b][a])
				{
					Local_centers(a, b, Localmins0, Localminst, Localmin);

					if (Localmins0.front().s0 <= mins0)
						mins0 = Localmins0.front().s0;
					if (Localminst.front().st < minst)
						minst = Localminst.front().st;
					if ((Localmin.front().s0 + Localmin.front().st) < min)
						min = Localmin.front().s0 + Localmin.front().st;

					while (!Localmins0.empty())
					{
						y = Localmins0.back();
						LocalInAbsCenters.push_back(y);
						Localmins0.pop_back();
					}
					while (!Localminst.empty())
					{
						y = Localminst.back();
						LocalExAbsCenters.push_back(y);
						Localminst.pop_back();
					}
					while (!Localmin.empty())
					{
						y = Localmin.back();
						LocalAbsCenters.push_back(y);
						Localmin.pop_back();
					}
				}

	// Ищем наименьшие из локальных центров
	//Внутренние
	while (!LocalInAbsCenters.empty())
	{
		if (LocalInAbsCenters.back().s0 == mins0)
		{
			InAbsCenters.push_back(LocalInAbsCenters.back());
		}
		LocalInAbsCenters.pop_back();
	}
	//Внешние
	while (!LocalExAbsCenters.empty())
	{
		if (LocalExAbsCenters.back().st == minst)
		{
			ExAbsCenters.push_back(LocalExAbsCenters.back());
		}
		LocalExAbsCenters.pop_back();
	}
	//Двусторонние
	while (!LocalAbsCenters.empty())
	{
		if ((LocalAbsCenters.back().s0 + LocalAbsCenters.back().st) == min)
		{
			AbsCenters.push_back(LocalAbsCenters.back());
		}
		LocalAbsCenters.pop_back();
	}
}

// поиск и вывод абсолютных центров
void Graph::Abs_centers()
{
	vector<center> InAbsCenters; 
	vector<center> ExAbsCenters;
	vector<center> AbsCenters;

	vector<center_on_vertex> InCenters;
	vector<center_on_vertex> ExCenters;
	vector<center_on_vertex> Centers;

	Centers_on_vertexes(InCenters, ExCenters, Centers); //Центры среди вершин
	Centers_on_edges(InAbsCenters, ExAbsCenters, AbsCenters); // Центры среди граней

	center_on_vertex x;
	center y;

	// Ищем абсолютные центры: сравниваем центры на врешинах и гранях
	x = InCenters.front();
	y = InAbsCenters.front();
	if (x.s0 > y.s0)
		InCenters.clear();
	else
		if (y.s0 > x.s0)
			InAbsCenters.clear();

	x = ExCenters.front();
	y = ExAbsCenters.front();
	if (x.s0 > y.s0)
		ExCenters.clear();
	else
		if (y.s0 > x.s0)
			ExAbsCenters.clear();

	x = Centers.front();
	y = AbsCenters.front();
	if (x.s0 > y.s0)
		Centers.clear();
	else
		if (y.s0 > x.s0)
			AbsCenters.clear();

	//Вывод
	while (!InCenters.empty())
	{
		x = InCenters.back();
		cout << "Internal Center:";
		cout << x.a+1 << endl;
		cout << "Internal radius:" << x.s0 << endl;
		cout << "External radius:" << x.st << endl << endl;
		InCenters.pop_back();
	}

	while (!ExCenters.empty())
	{
		x = ExCenters.back();
		cout << "External Center:";
		cout << x.a+1 << endl;
		cout << "Internal radius:" << x.s0 << endl;
		cout << "External radius:" << x.st << endl << endl;
		ExCenters.pop_back();
	}

	while (!Centers.empty())
	{
		x = Centers.back();
		cout << "Center:";
		cout << x.a+1 << endl;
		cout << "Internal radius:" << x.s0 << endl;
		cout << "External radius:" << x.st << endl;
		cout << "Diameter:" << x.s0 + x.st << endl << endl;
		Centers.pop_back();
	}

	while (!InAbsCenters.empty())
	{
		y = InAbsCenters.back();
		cout << "Absolut Internal Center:";
		cout << "(" << y.a + 1 << ", " << y.b + 1 << ") " << y.e << endl;
		cout << "Internal radius:" << y.s0 << endl;
		cout << "External radius:" << y.st << endl << endl;
		InAbsCenters.pop_back();
	}

	while (!ExAbsCenters.empty())
	{
		y = ExAbsCenters.back();
		cout << "Absolut External Center:";
		cout << "(" << y.a + 1 << ", " << y.b + 1 << ") " << y.e << endl;
		cout << "Internal radius:" << y.s0 << endl;
		cout << "External radius:" << y.st << endl << endl;
		ExAbsCenters.pop_back();
	}

	while (!AbsCenters.empty())
	{
		y = AbsCenters.back();
		cout << "Absolut Center:";
		cout << "(" << y.a + 1 << ", " << y.b + 1 << ") " << y.e << endl;
		cout << "Internal radius:" << y.s0 << endl;
		cout << "External radius:" << y.st << endl;
		cout << "Diameter:" << y.s0 + y.st << endl << endl;
		AbsCenters.pop_back();
	}
}

int main()
{
	int m = 6;
	Graph gr(m);
	gr.InputTest1();
	gr.PrintGraph();
	gr.PrintDistances();
	gr.PrintVertices();

	gr.Abs_centers();

	return 0;
}

