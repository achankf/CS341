#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include <stack>
#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>
using namespace std;

enum colour_t{
	BLACK, WHITE, GRAY
};

typedef short node_t;

struct stat{
	int discover;
	int finish;
	colour_t colour;
	node_t pre;
};

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
	cout << errmsg << endl;
	exit(1);
}

void generate_al(adj_list_t &al){
	string line;
	stringstream ss;
	node_t edge;
	char colon;

	node_t maxval = -1;
	node_t minval = 0x7fff;

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
	if (minval < 0
		|| al.rbegin()->first < maxval 
		|| al.begin()->first > minval) rpterr("some edge is out-of-range");

	node_t prev = al[0].first;
	if (prev < 0) rpterr("smallest label cannot be less than 0");
	for (size_t i = 1; i < al.size(); i++){
		node_t next = al[i].first;
		if (prev != next - 1) rpterr("label have a hole in between");
		prev = next;
	}
}

void DFS_visit(const adj_list_t &al, statistics_t &stats, const reverse_mapper &map, int i, int *time){

	stats[i].colour = GRAY;
	(*time)++;
	stats[i].discover = *time;

	neighbour_t::iterator it = al[i].second.begin();
	for (; it != al[i].second.end(); it++){
		if (stats[map[*it]].colour != WHITE) continue;
		stats[map[*it]].pre = al[i].first;
		DFS_visit(al, stats, map, map[*it], time);
	}

	stats[i].colour = BLACK;
	(*time)++;
	stats[i].finish = *time;
}

void DFS(const adj_list_t &al, statistics_t &stats, const reverse_mapper &map){

	int time = 0;

	if (al.size() <= 0) return;

	// set up stats for DFS
	for (size_t i = 0; i < al.size(); i++){
		stat temp = {-1,-1,WHITE,-1};
		stats.push_back(temp);
	}

	for (size_t i = 0; i < al.size(); i++){
		if (stats[i].colour != WHITE) continue;
		DFS_visit(al, stats, map, i, &time);
	}
}

int main(){
	adj_list_t al;
	al.reserve(0x7fff);

	generate_al(al);

#ifdef ALFRED_CHAN_DEBUG
	print_al(al);
	cout << endl;
#endif

	statistics_t stats;
	stats.reserve(al.size());
	reverse_mapper map;
	map.reserve(al.size());

	for (size_t i = 0; i < al.size() + 1; i++){
		map[al[i].first] = i;
	}

	DFS(al, stats, map);

	node_t u, v;
	while(cin >> u && cin >> v){
		// turn node labels into indices
		const int ui = map[u];
		const int vi = map[v];
		
		cout << u << ' ' << v << ' ';
		neighbour_t &nei = al[map[u]].second;

		// by parenthesis theorem
		if (nei.find(v) == nei.end()){
			cout << "no-such-edge" << endl;
		} else if (stats[ui].discover < stats[vi].discover
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
