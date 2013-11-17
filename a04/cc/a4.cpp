#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm> // sort
#include <limits> // max and min limits of integer
using namespace std;

#define VERTEX_MAX 0x8000

enum colour_t{
	BLACK, WHITE, GRAY
};

typedef int node_t;

struct stat{
	int discover;
	int finish;
	colour_t colour;
	node_t pre;
};

// originally implemented as adjacency list
// but too late to change the name now
typedef set<node_t> neighbour_t;
typedef pair<node_t,neighbour_t> adj_list_pair_t;
typedef vector<adj_list_pair_t> adj_list_t;
typedef vector<stat> statistics_t;
typedef vector<node_t> reverse_mapper;

#ifdef ALFRED_CHAN_DEBUG
void print_al(adj_list_t &al){
	for (size_t i = 0; i < al.size(); i++){
		cout << al[i].first << " : ";
		neighbour_t::iterator it = al[i].second.begin();
		for (; it != al[i].second.end(); it++){
			cout << *it << ' ';
		}
		cout << endl;
	}
}
#endif

void rpterr(const char *errmsg){
	cout << "ERROR:" << errmsg << endl;
	exit(1);
}

// generate adjacency forest from stdin
void generate_al(adj_list_t &al){
	string line;
	stringstream ss;
	node_t edge;
	char colon;

	node_t maxval = numeric_limits<int>::min();
	node_t minval = numeric_limits<int>::max();

	while(getline(cin, line)){

		if (line.size() == 0) break;

		adj_list_pair_t apair;

		ss.clear();
		ss << line;
		ss >> apair.first;
		ss  >> colon;
		while(ss >> edge){
			if (edge < 0) rpterr("some edge is out-of-range");
			apair.second.insert(edge);
			maxval = max(maxval, edge);
			minval = min(minval, edge);
		}

		al.push_back(apair);
	}

	sort(al.begin(), al.end());

	// check for invalid input
	if (minval < 0
		|| al.rbegin()->first < maxval 
		|| al.begin()->first > minval) rpterr("some edge is out-of-range");

	node_t prev = al[0].first;
	if (prev < 0) rpterr("smallest label cannot be less than 0");
	for (size_t i = 1; i < al.size(); i++){
		node_t next = al[i].first;
		if (prev != next - 1) rpterr("labels have a hole in between");
		prev = next;
	}
}

// similar to the slides
void DFS_visit(const adj_list_t &al, statistics_t &stats, const reverse_mapper &map, int i, int *time){

	stats[i].colour = GRAY;
	(*time)++;
	stats[i].discover = *time;

	neighbour_t::iterator it = al[i].second.begin();
	for (; it != al[i].second.end(); it++){
		if (stats[map[*it]].colour != WHITE) continue;
		// add predecessor
		stats[map[*it]].pre = al[i].first;
		// go deep into neighbours
		DFS_visit(al, stats, map, map[*it], time);
	}

	stats[i].colour = BLACK;
	(*time)++;
	stats[i].finish = *time;
}

// similar to the slides
void DFS(const adj_list_t &al, statistics_t &stats, const reverse_mapper &map){

	int time = 0;

	for (size_t i = 0; i < al.size(); i++){
		if (stats[i].colour != WHITE) continue;
		DFS_visit(al, stats, map, i, &time);
	}
}

// do processing on input edges
void query(const adj_list_t &al, statistics_t &stats, const reverse_mapper &map){
	node_t u, v;
	while(cin >> u && cin >> v){

		cout << u << ' ' << v << ' ';

		// query edges out-of-bound
		if (u < 0 || v > (int) al.size() + 1){
			cout << "no-such-edge" << endl;
			continue;
		}

		// turn node labels into indices
		const int ui = map[u];
		const int vi = map[v];
		const neighbour_t &nei = al[map[u]].second;

		// not valid edge
		if (nei.find(v) == nei.end()){
			cout << "no-such-edge" << endl;
			continue;
		}

		// by parenthesis theorem
		if (stats[ui].discover < stats[vi].discover
			&& stats[vi].discover < stats[vi].finish
			&& stats[vi].finish < stats[ui].finish){
			if (stats[vi].pre == u){
				cout << "tree-edge" << endl;
			} else {
				cout << "forward-edge" << endl;
			}
		} else if (stats[vi].discover < stats[ui].discover
			&&  stats[ui].discover < stats[ui].finish
			&& stats[ui].finish < stats[vi].finish){
			cout << "back-edge" << endl;
		} else {
			cout << "cross-edge" << endl;
		}
	}
}

int main(){
	adj_list_t al;
	al.reserve(VERTEX_MAX);

	generate_al(al);
	if (al.size() <= 0) return 0;

#ifdef ALFRED_CHAN_DEBUG
	print_al(al);
	cout << endl;
#endif

	stat temp = {-1,-1,WHITE,-1};
	statistics_t stats(al.size(), temp);

	// reserve and initialize with dummy value (-1)
	reverse_mapper map(al.size() + 1, -1);

	// assign the actual values
	for (size_t i = 0; i < al.size(); i++){
		map[al[i].first] = i;
	}

	// depth-first search and store results in stats
	DFS(al, stats, map);

	// take advantage of memoization and identify the type of edge
	query(al, stats, map);
}
