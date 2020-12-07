#include <iostream>
#include <vector>
using namespace std;

#define SIZE 6
#define INF 10000
void Input(int a[][SIZE]);
void Output(int a[][SIZE]);

int Deixtra(int a[][SIZE]);
int Floyd(int a[][SIZE]);

int BellmanFord(int a[][SIZE]);

int main()
{
	int a[SIZE][SIZE]; // матрица связей
	
	Input(a);
	Output(a);
	//Deixtra(a);
	//Floyd(a);

	BellmanFord(a);


}
void Input(int a[][SIZE])
{
	int temp;
	// Инициализация матрицы связей
	for (int i = 0; i < SIZE; i++)
	{
		a[i][i] = 0;
		for (int j = i + 1; j < SIZE; j++) {
			printf("Input distance %d - %d: ", i + 1, j + 1);
			cin >> temp;
			a[i][j] = temp;
			a[j][i] = temp;
		}
	}
}
void Output(int a[][SIZE])
{
	// Вывод матрицы связей
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			printf("%5d ", a[i][j]);
		printf("\n");
	}
}
int min(int a, int b)
{
	return (a < b) ? a : b;
}


int Deixtra(int a[][SIZE])
{
	int d[SIZE]; // минимальное расстояние
	int v[SIZE]; // посещенные вершины
	int temp, minindex, min;
	int begin_index = 0;

	//Инициализация вершин и расстояний
	for (int i = 0; i < SIZE; i++)
	{
		d[i] = INF;
		v[i] = 1;
	}
	d[begin_index] = 0;
	// Шаг алгоритма
	do {
		minindex = INF;
		min = INF;
		for (int i = 0; i < SIZE; i++)
		{ // Если вершину ещё не обошли и вес меньше min
			if ((v[i] == 1) && (d[i] < min))
			{ // Переприсваиваем значения
				min = d[i];
				minindex = i;
			}
		}
		// Добавляем найденный минимальный вес
		// к текущему весу вершины
		// и сравниваем с текущим минимальным весом вершины
		if (minindex != INF)
		{
			for (int i = 0; i < SIZE; i++)
			{
				if (a[minindex][i] > 0)
				{
					temp = min + a[minindex][i];
					if (temp < d[i])
					{
						d[i] = temp;
					}
				}
			}
			v[minindex] = 0;
		}
	} while (minindex < INF);
	// Вывод кратчайших расстояний до вершин
	printf("\nThe shortest distance to vertices: \n");
	for (int i = 0; i < SIZE; i++)
		printf("%5d ", d[i]);

	// Восстановление пути
	int ver[SIZE]; // массив посещенных вершин
	int end = 5; // индекс конечной вершины = 6 - 1
	ver[0] = end + 1; // начальный элемент - конечная вершина
	int k = 1; // индекс предыдущей вершины
	int weight = d[end]; // вес конечной вершины

	while (end != begin_index) // пока не дошли до начальной вершины
	{
		for (int i = 0; i < SIZE; i++) // просматриваем все вершины
			if (a[i][end] != 0)   // если связь есть
			{
				int temp = weight - a[i][end]; // определяем вес пути из предыдущей вершины
				if (temp == d[i]) // если вес совпал с рассчитанным
				{                 // значит из этой вершины и был переход
					weight = temp; // сохраняем новый вес
					end = i;       // сохраняем предыдущую вершину
					ver[k] = i + 1; // и записываем ее в массив
					k++;
				}
			}
	}
	// Вывод пути (начальная вершина оказалась в конце массива из k элементов)
	printf("\nThe output of the shortest path\n");
	for (int i = k - 1; i >= 0; i--)
		printf("%3d ", ver[i]);
	
	return 0;
}
int Floyd(int a[][SIZE])
{
	for (int i = 0; i < SIZE-1; i++)
		for (int j = i + 1; j < SIZE; j++)
			if (a[i][j] == 0)
			{
				a[i][j] = INF;
				a[j][i] = INF;
			}

	int p[SIZE][SIZE];
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (a[i][j] == INF)
				p[i][j] = INF;
			else
				p[i][j] = j;

	for (int k = 0; k < SIZE; ++k)
		for (int i = 0; i < SIZE; ++i)
			for (int j = 0; j < SIZE; ++j)
				if (a[i][j] > a[i][k] + a[k][j])
				{
					a[i][j] = a[i][k] + a[k][j];
					p[i][j] = p[i][k];
				}
	cout << "The shortest distance to vertices:" << endl;
	for (int i = 0; i < SIZE-1; i++)
		cout << a[0][i] << "  ";
	cout << endl;


	int path[SIZE];
	for (int i = 0; i < SIZE; i++)
		path[i] = INF;


	cout << "The output of the shortest path:" << endl;
	if (a[0][5] == INF)
		cout << "No path found" << endl;             
	int c = 0;

	while (c != 5)
	{
		cout << c + 1 << "  ";
		c = p[c][5];
	}
	cout << SIZE << endl;
	return 0;
}

int BellmanFord(int a[][SIZE])
{
	struct edge {
		int a, b, cost;
		edge(int a, int b, int cost)
		{
			this->a = a;
			this->b = b;
			this->cost = cost;
		}
	};
	int v = 0;
	vector<edge> e;
	for (int i = 0; i < SIZE - 1; i++)
		for (int j = i + 1; j < SIZE; j++)
			e.push_back(edge(i, j, a[i][j]));

		vector<int> d(SIZE, INF);
		d[v] = 0;
		vector<int> p(SIZE, -1);
		for (int i = 0; i < SIZE - 1; ++i)
			for (int j = 0; j < SIZE-1; ++j)
				if (d[e[j].a] < INF)
				{
					d[e[j].b] = min(d[e[j].b], d[e[j].a] + e[j].cost);
					p[e[j].b] = e[j].a;
				}
		for (int i = 0; i < d.size(); i++)
		{
			cout << d[i] << "  ";
		}
		cout << endl;
		int t = SIZE - 1;
		if (d[t] == INF)
			cout << "No path from " << v << " to " << t << ".";
		else {
			vector<int> path;
			for (int cur = t; cur != -1; cur = p[cur])
				path.push_back(cur);
			reverse(path.begin(), path.end());

			cout << "Path from " << v << " to " << t << ": ";
			for (size_t i = 0; i < path.size(); ++i)
				cout << path[i] << ' ';
		}
		return 0;
}