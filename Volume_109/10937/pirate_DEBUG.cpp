#include <iostream>
#include <queue>
#include <utility>
#include <vector>
#include <set>
using namespace std;

#define LANDING '@'
#define WATER '~'
#define TREE '#'
#define SAND '.'
#define ANGRY '*'
#define TREASURE '!'
#define NO '#' 

typedef vector<vector<char>> Map;

bool angry_neighbor(const Map& imap, int h, int w) {
  int H = imap.size();
  int W = imap[0].size();
  for (int dh = -1; dh <= 1; dh++) {
    for (int dw = -1; dw <= 1; dw++) {
      int nh = h + dh;
      int nw = w + dw;
      if (0<= nh && nh < H && 0<=nw && nw < W) {
        if (imap[nh][nw] == ANGRY) {
          return true;
        }
      }
    }
  }
  return false;
}

typedef pair<int, int> II;
typedef vector<vector<int>> Graph;

int indexof(const vector<II>& points, const II& point) {
  for (int i = 0; i < points.size(); ++i) {
    if (points[i] == point) return i;
  }
  return -1;
}

void BFS(const Map& pmap, const vector<II>& points, int from, Graph& graph) {
  int H = pmap.size();
  int W = pmap[0].size();
  int n = points.size();
  Graph dist(H, vector<int>(W, -1));
  II p = points[from];
  dist[p.first][p.second] = 0;

  queue<II> Q;
  Q.push(p);
  while (!Q.empty()) {
    II u = Q.front();
    Q.pop();
    int dist_u = dist[u.first][u.second]; 
    int to = indexof(points, u);
    if (to != -1) {
      graph[from][to] = dist_u;
    }
    int dh[] = {-1, 0, 1, 0};
    int dw[] = {0, -1, 0, 1};
    for (int delta = 0; delta < 4; ++delta) {
      int nh = u.first + dh[delta];
      int nw = u.second + dw[delta];
      if (0 <= nh && nh < H && 0 <= nw && nw < W &&
          pmap[nh][nw] != NO && dist[nh][nw] == -1) {
        dist[nh][nw] = dist_u + 1;
        II v(nh, nw);
        Q.push(v);
      }
    }
  }
}

int rec_dp(const Graph& graph,
    const vector<II>& points,
    vector<vector<int>>& dp,
    vector<vector<int>>& next,
    int point_set,
    int cur_point) {
  cout << "point_set: " << point_set << endl;
  cout << "cur_point: " << cur_point << endl;
  int landing_point = graph.size() - 1;  // n is also landing point
  if (dp[point_set][cur_point] != -1) 
    return dp[point_set][cur_point];

  set<int> treasures;
  for (int i=0; i<graph.size()-1; ++i) {  // landing point is not treasure
    treasures.insert(i);
  }

  int tmp_set = point_set;
  int idx = 0;
  while (tmp_set) {
    if (tmp_set & 1) {
      treasures.erase(idx);
    }
    tmp_set = tmp_set >> 1;
    idx++;
  }

  // DEBUG
  cout << "treasures: ";
  for (int t : treasures) {
    cout << t << ",";
  }
  cout << endl;

  if (treasures.empty()) {
    dp[point_set][cur_point] = graph[cur_point][landing_point]; // + so_far
    next[point_set][cur_point] = landing_point;
    return dp[point_set][cur_point];
  } else {
    for (int new_point : treasures) {
      int new_point_set = point_set | (1<<new_point);
      int cost = graph[cur_point][new_point] +
          rec_dp(graph, points, dp, next, new_point_set, new_point);
      if (dp[point_set][cur_point] == -1 ||
          cost < dp[point_set][cur_point]) {
        dp[point_set][cur_point] = cost;
        next[point_set][cur_point] = new_point;
      }
    }
  }
  return dp[point_set][cur_point];
}

// DEBUG
void route(int n,
           const vector<vector<int>>& dp,
           const vector<vector<int>>& next) {
  int cur_point = n-1;
  int point_set = 0;
  cout << "route: @";
  do {
    int next_point = next[point_set][cur_point];
    cout << " " << next_point;
    point_set |= (1<<next_point);
    cur_point = next_point;

  } while (cur_point != n-1);
  cout << endl;
}

int solve(const Map& imap, int H, int W) {
  Map pmap(H, vector<char>(W, SAND));
  vector<II> points;
  II landing;
  for (int h = 0; h < H; ++h) {
    for (int w = 0; w < W; ++w) {
      int cell = imap[h][w];
      if (cell == WATER ||
          cell == TREE ||
          cell == ANGRY) {
        pmap[h][w] = NO;
      } else {
        if (angry_neighbor(imap, h, w)) {
          if (cell == LANDING || cell == TREASURE) {
            return -1;  // impossible to achieve
          } else {  // SAND
            pmap[h][w] = NO;
          }
        } else {
          pmap[h][w] = imap[h][w];
          if (cell == LANDING) {
            landing = II(h, w);
          } else if (cell == TREASURE) {
            points.push_back(II(h, w));
          }
        }
      }
    }
  }
  points.push_back(landing);  // points has landing

  // DEBUG
  for (int h = 0; h < H; ++h) {
    for (int w = 0; w < W; ++w) {
      if (pmap[h][w] == '!') {
        int idx= indexof(points, II(h, w));
        cout << idx;
      } else {
        cout << pmap[h][w];
      }
    }
    cout << endl;
  }

  int n = points.size();
  Graph graph(n, vector<int>(n, -1));
  for (int from = 0; from < points.size(); ++from) {
    BFS(pmap, points, from, graph);
  }
  // if any graph[i][j] == -1, impossible to achieve
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (graph[i][j] == -1) 
        return -1;
    }
  }

  // DEBUG
  cout << "points" << endl;
  for (II p : points) {
    cout << p.first << " " << p.second << endl;
  }
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      cout.width(5);
      cout << graph[i][j];
    }
    cout << endl;
  }
  
  
  // dp[points][i] means the minimum time starting from points[i],
  // collect all other treasures not in points (except landing point),
  // and finally return to landing point.
  int all_set = 1<<(n-1);
  vector<vector<int>> dp(all_set, vector<int>(n, -1));
  vector<vector<int>> next(all_set, vector<int>(n, -1));
  dp[all_set-1][n-1] = 0;
  next[all_set-1][n-1] = -1;

  int ans = rec_dp(
      graph, points, dp, next,
      0,  // point_set
      n-1);  // cur_point = landing point
                  
  route(n, dp, next);

  return ans;
}

int main() {
  int H, W;
  int round = 1;
  while (cin >> H >> W) {
    if (H == 0 && W == 0) break;
    Map tmap(H, vector<char>(W, SAND));
    for (int h = 0; h < H; ++h) {
      for (int w = 0; w < W; ++w) {
        char c;
        cin >> c;
        tmap[h][w] = c;
      }
    }
    cout << H << " " << W << endl;
    cout << solve(tmap, H, W) << endl;
    round++;
  }
}
