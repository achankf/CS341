#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <limits>
#include <cassert>
using namespace std;

// typedefs and structures for code simplification
typedef double num_t;
struct coor_t : public pair<num_t,num_t>{
	size_t idx;
	coor_t(num_t x, num_t y, size_t idx) : pair<num_t,num_t>(x,y), idx(idx){}
};
typedef vector<coor_t> veccoor_t;
typedef list<coor_t> refcoor_t;
typedef vector<num_t> vecx_t;

// the return value type
struct retval_t : public pair<coor_t,coor_t> {
	num_t dist; // the distance
	retval_t(coor_t &a, coor_t &b, num_t dist) : pair<coor_t,coor_t>(a,b), dist(dist){}
};

inline num_t dist(coor_t &c1, coor_t &c2){
	return sqrt(pow(c2.first - c1.first, 2) + pow(c2.second - c1.second, 2));
}

// comparator for coor_t -- order by y only
struct coor_comp{
	bool operator()(coor_t &a, coor_t &b){ return a.second < b.second; }
};

// get coordinates
inline void get_coor(veccoor_t &veccoor, vecx_t &vecx){
	size_t size = 0;
	for (num_t x, y; cin >> x && cin >> y; size++){
		vecx.push_back(x);
		coor_t temp(x,y,size);
		veccoor.push_back(temp);
	}

	// heap sort - O(nlgn)
	make_heap(vecx.begin(),vecx.end());
	sort_heap(vecx.begin(),vecx.end());
	make_heap(veccoor.begin(),veccoor.end(),coor_comp());
	sort_heap(veccoor.begin(),veccoor.end(),coor_comp());
}

void select_candidate(refcoor_t &coors, veccoor_t &candidates, num_t min_dist, num_t xmid){
	refcoor_t::iterator it = coors.begin();
	for (; it != coors.end(); it++){
		if (abs(it->first - xmid) > min_dist) continue;
		candidates.push_back(*it);
	}
}

retval_t check_strip_and_print(veccoor_t &candidates, retval_t &min_cand){

	num_t new_min_dist = min_cand.dist;
	coor_t *candi = NULL;
	coor_t *candj = NULL;

	// inner loop is bounded by a constant -- O(n)
	for (size_t i = 0; i < candidates.size(); i++){
		size_t need_to_check= min(candidates.size(),i+7);
		for (size_t j = i+1; j < need_to_check; j++){
			double temp = dist(candidates[i], candidates[j]);

			if (temp <= new_min_dist){
				candi = &candidates[i];
				candj = &candidates[j];
				new_min_dist = temp;
			}
		}
	}

	if (candi != NULL){ // implicitly candi != NULL
		return retval_t(*candi, *candj, new_min_dist);
	} else {
		return min_cand;
	}
}

void partition(refcoor_t &coors, refcoor_t &L, refcoor_t &U, num_t x){
	refcoor_t::iterator it = coors.begin();
	size_t half = coors.size() / 2;
	for (; it != coors.end(); it++){
		if (L.size() < half && it->first <= x){ // make sure L doesn't exist n/2 elements
			L.push_back(*it);
		} else if (U.size() < half) { // make sure U doesn't exceed n/2 elements
			U.push_back(*it);
		} else { // U is full, so fill the element in L
			L.push_back(*it);
		}
	}
}

retval_t closest_pair_helper(refcoor_t &coors, vecx_t &vecx, size_t start){
	if (coors.size() <= 1){
		coor_t temp(0,0,0);
		return retval_t(temp,temp,numeric_limits<num_t>::max());
	} else if (coors.size() == 2){
		coor_t a = *coors.begin();
		coor_t b = *coors.rbegin();
		return retval_t(a,b,dist(a,b));
	}

	size_t pivot = start + coors.size() / 2;
	refcoor_t L,U;
	partition(coors, L, U, vecx[pivot]);
	retval_t dL = closest_pair_helper(L, vecx, start);
	retval_t dR = closest_pair_helper(U, vecx, start + L.size());

	retval_t *min_cand;
	if (dL.dist < dR.dist){
		min_cand = &dL;
	} else {
		min_cand = &dR;
	}
	veccoor_t candidates;
	select_candidate(coors, candidates, min_cand->dist, vecx[pivot]);
	return check_strip_and_print(candidates, *min_cand);
}

retval_t closest_pair(veccoor_t &veccoor, vecx_t &vecx){
	refcoor_t coors(veccoor.begin(),veccoor.end()); // copy the coor-vector into a linked list (easier)
	return closest_pair_helper(coors, vecx, 0);
}

#ifdef ALFRED_CHAN_DEBUG
// debugging functions can be ignored
retval_t brute_force_test(veccoor_t &veccoor){
	const size_t size = veccoor.size();
	size_t candi = 0;
	size_t candj = 0;
	num_t min_dist = numeric_limits<num_t>::max();
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			if (i == j) continue;
			num_t temp = dist(veccoor[i],veccoor[j]);
			if (temp >= min_dist) continue;
			candi = i;
			candj = j;
			min_dist = temp;
		}
	}
	return retval_t(veccoor[candi],veccoor[candj],min_dist);
}

void print_coor(coor_t c){
	cout << "(" << c.first << ',' << c.second <<  "," << c.idx << ')';
}
#endif

int main(){
	veccoor_t veccoor;
	vecx_t vecx; // x values reference for pivot values
	get_coor(veccoor, vecx);
	retval_t temp = closest_pair(veccoor, vecx);

	if (temp.first.idx < temp.second.idx){
		cout << temp.first.idx << endl;
		cout << temp.second.idx << endl;
	} else {
		cout << temp.second.idx << endl;
		cout << temp.first.idx << endl;
	}

#ifdef ALFRED_CHAN_DEBUG
	retval_t bft = brute_force_test(veccoor);
	cout << "D&C:";
	print_coor(temp.first);
	cout << " ";
	print_coor(temp.second);
	cout << endl << "BF:";
	print_coor(bft.first);
	cout << " ";
	print_coor(bft.second);
	cout << endl;
	cout << "D&C: " << temp.dist << " BF:" << bft.dist << endl;
	assert(temp.dist == bft.dist);
#endif
}
