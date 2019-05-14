#include "solveProblem.h"


const int N = 1e2 + 10;
typedef std::pair<int, int> pii;
typedef long long LL;

const int INF = 0x3f3f3f3f;
const LL INF64 = 0x3f3f3f3f3f3f3f3f;

int mx[N][N], mn[N][N];
int add(int a, int b) {
	return a + b;
}
int mul(int a, int b) {
	return a * b;
}
std::vector<NodeData> SolveProblem::getAnswer(std::vector<NodeData> node_datas,int &_ans)
{
	std::vector<node> g;
	int n = node_datas.size();
	auto inc = [&](int v, int i) {
		v += i;
		v %= n;
		if (v < 0) v += n;
		return v;
	};

	for (int i = 0; i < n; ++i) {
		int v, k;
		v = node_datas[i].v;
		k = node_datas[i].kind;
		g.push_back({ v, k, (k == 0 ? add : mul) });
	}
	int ans = -INF;
	std::vector<int> path;
	for (int i = 0; i < n; ++i) mn[i][i] = mx[i][i] = g[i].v;
	for (int len = 1; len < n; ++len) {
		for (int i = 0; i < n; ++i) {
			int j = inc(i, len);
			int tmx = -INF, tmn = INF;
			for (int k = i; k != j; k = inc(k, 1)) {
				const auto &op = g[k].op;
				for (auto x : { mx[i][k], mn[i][k] }) {
					for (auto y : { mx[inc(k, 1)][j], mn[inc(k, 1)][j] }) {
						tmx = std::max(tmx, op(x, y));
						tmn = std::min(tmn, op(x, y));
					}
				}
			}
			mn[i][j] = tmn; mx[i][j] = tmx;
		}
	}

	int ansl = -1, ansr = -1;
	for (int i = 0; i < n; ++i) {
		int j = inc(i, n - 1);
		if (mx[i][j] > ans) {
			ans = mx[i][j];
			ansl = i; ansr = j;
		}
	}

	assert(ansl != -1);

	std::vector<int> tmp{ ansr };
	std::function<void(int, int, int)> backtrack = [&](int l, int r, int v) {

		if (l == r) return;
		for (int k = l; k != r; k = inc(k, 1)) {
			const auto &op = g[k].op;
			for (auto i : { mx[l][k], mn[l][k] }) {
				for (auto j : { mx[inc(k, 1)][r], mn[inc(k, 1)][r] }) {
					if (op(i, j) == v) {
						backtrack(l, k, i); backtrack(inc(k, 1), r, j);
						tmp.push_back(k);
						return;
					}
				}
			}
		}
		assert(false);
	};
	backtrack(ansl, ansr, mx[ansl][ansr]);
	//assert(tmp.size() == n);
	path = tmp;
	//cout << ans << '\n';
	std::vector<NodeData> ans_datas;
	for (auto i : path)
	{
		ans_datas.push_back(node_datas[i]);
	}
	_ans = ans;
	return ans_datas;
}
