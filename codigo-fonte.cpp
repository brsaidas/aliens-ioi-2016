#include <bits/stdc++.h>

using namespace std;

#define N 100010
#define INF 1e18
#define SQ(x) ((x)*(x))

int qtd[N]; // Quantidade de retangulos usados pela PD
long long dp[N]; // PD

// Definindo a estrutura Ponto
#define X first
#define Y second
typedef pair<long long, long long> Ponto;

vector<Ponto> pts;

double slope(int i, int j)
{
	long long bi = SQ(pts[i].X) - 2ll * pts[i].X + dp[i - 1] - SQ(max(pts[i - 1].Y - pts[i].X + 1ll, 0ll));
	long long bj = SQ(pts[j].X) - 2ll * pts[j].X + dp[j - 1] - SQ(max(pts[j - 1].Y - pts[j].X + 1ll, 0ll));
	long long ai = -2ll * pts[i].X;
	long long aj = -2ll * pts[j].X;
	
	return (double) (bi - bj)/(aj - ai);
}

int solve(int n, long long C)
{	
	deque<long long> q;
	q.push_back(1);

	// Execucao da PD
	for (int i = 1; i <= n; i++)
	{
		while (q.size() > 1 && slope(q[0], q[1]) < pts[i].Y) 
			q.pop_front();
		
		long long j = q.front();
		
		dp[i] = SQ(pts[i].Y + 1) + C - 2ll * pts[j].X * pts[i].Y + SQ(pts[j].X) - 2ll * pts[j].X + dp[j - 1] - SQ(max(pts[j - 1].Y - pts[j].X + 1ll, 0ll));
		qtd[i] = 1 + qtd[j - 1];
			
		while (q.size() > 1 && slope(q[q.size() - 2], q.back()) > slope(q.back(), i + 1))
			q.pop_back();
		
		q.push_back(i + 1);
	}
	
	return qtd[n]; // Retorna quantos quadrados foram usados
}

long long take_photos(int n, int m, int k, int* r, int* c)
{
	// Ordenamos os pontos pela linha (desempate pela coluna)
	set<Ponto> pontos;
	
	for (int i = 0; i < n; i++)
		if (r[i] <= c[i]) 
			// Ponto acima ou dentro da diagonal principal
			pontos.insert({r[i], c[i]});
		else
			// Ponto abaixo da diagonal principal, entao espelhamos ele
			pontos.insert({c[i], r[i]}); 
	
	// Vector para armazenar os pontos que importam
	pts.push_back({-INF, -INF});
	
	// Selecionando apenas os pontos que realmente importam
	for (auto p : pontos)
	{
		Ponto ultimo = pts.back();
		if (ultimo.X == p.X) // Pontos na mesma linha
			pts[pts.size() - 1] = p;
		else if (ultimo.Y < p.Y) // Pontos em linha diferentes
				pts.push_back(p);
	}
	
	n = pts.size() - 1; // Atualizando o valor de n
	k = min(k, n); // Atualizando o valor de k
	
	// Busca binária no custo C
	long long ini = 0, fim = (long long) m * m;
	
	while (fim > ini)
	{
		long long mei = (ini + fim) / 2ll;
	
		if (solve(n, mei) <= k) 
			fim = mei;
		else
			ini = mei + 1;
	}
	
	solve(n, ini);
	
	return dp[n] - k * ini;
}
/*
int main() 
{
	ios_base::sync_with_stdio(0); cin.tie(0);
	
	freopen("30", "r", stdin);
	
	int n, m, k; cin >> n >> m >> k;
	
	int r[n], c[n];
	
	for (int i = 0; i < n; i++) cin >> r[i] >> c[i];
	
	cout << take_photos(n, m, k, r, c) << '\n';
}
*/
