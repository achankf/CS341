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

typedef double num_t;
#if 0
typedef pair<num_t,num_t> coor_t;
typedef vector<coor_t> veccoor_t;
typedef vector<size_t> refcoor_t;
typedef vector<num_t> vecx_t;
#endif
struct coor_t : public pair<num_t,num_t>{
	size_t idx;
	coor_t(num_t x, num_t y, size_t idx) : pair<num_t,num_t>(x,y), idx(idx){}
};
typedef vector<coor_t> veccoor_t;
typedef list<coor_t> refcoor_t;
typedef vector<num_t> vecx_t;

int i = 0;

inline num_t dist(coor_t &c1, coor_t &c2){
	return sqrt(pow(c2.first - c1.first, 2) + pow(c2.second - c1.second, 2));
}

// comparator for coor_t -- order by y only
struct coor_comp{
	bool operator()(coor_t &a, coor_t &b){ return a.second < b.second; }
};

size_t get_coor(veccoor_t &veccoor, vecx_t &vecx){
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

	return size;
}

void print_coor(coor_t c){
	cout << "(" << c.first << ',' << c.second <<  "," << c.idx << ')';
}

#if 0
#ifdef MY_DEBUG
void printall(veccoor_t &veccoor, vecx_t &vecx){
	for (size_t i = 0; i < vecx.size(); i++){
		cout << i << ". " << vecx[i] << " (" << veccoor[i].first << ',' << veccoor[i].second << ')' << endl;
	}
	cout << endl;
}

void print_coor(coor_t c){
	cout << "(" << c.first << ',' << c.second << ')';
}
#endif

// put the pivot element to the end and the do the partition (L,U)
size_t partition(veccoor_t &veccoor, vecx_t &vecx, size_t start, size_t size){
cout << "PARTITION start:" << start << " size:" << size << endl;

	if (size <= 1) return 0;

	size_t pivot = start + size / 2;
	size_t max_L_size = pivot - start;

	// the pivot's x-coordinate
	num_t x = vecx[pivot];

	// pushing the reference *in order* - O(n)
	refcoor_t L,U;
	L.reserve(size);
	U.reserve(size);
	for (size_t i = start; i < start+size; i++){
		if (L.size() <= max_L_size && veccoor[i].first < x){
			L.push_back(i);
		} else {
			U.push_back(i);
		}
	}
//	cout << "pivot:" << pivot << " Lsize:" << L.size() << " Usize:" << U.size() << endl;

	// backup the coordinates - O(n)
	veccoor_t temp(veccoor.begin() + start, veccoor.begin() + start + size);

	// write back the coordinates in one pass - O(n)
	for (size_t i = 0, it = start; i < L.size(); i++, it++){
		if (it == L[i]) continue;
		iter_swap(veccoor.begin() + it, veccoor.begin() - start + L[i]);
	}
	for (size_t i = 0, it = pivot; i < U.size(); i++, it++){
//cout << it << " "<< U[i] << endl;
		if (it == U[i]) continue;
//cout << it << " "<< U[i] -start << endl;
		iter_swap(veccoor.begin() + it, temp.begin() + U[i] - start);
	}

#ifndef MY_DEBUG
	cout << "Lsize:" << L.size() << " Usize:" << U.size() << endl;
	refcoor_t::iterator dit = L.begin();
	refcoor_t::iterator dit2 = U.begin();
	for (; dit != L.end(); dit++){
		cout << *dit << ' ';
	}
	cout << endl;
	for (; dit2 != U.end(); dit2++){
		cout << *dit2 << ' ';
	}
	cout << endl;
	cout << start << ' ' << size << ' ' << pivot << " x:" << x << endl;
	printall(veccoor,vecx);
#endif

	return pivot;
	// L, U, temp call destructor
}

void select_candidate(veccoor_t &veccoor, refcoor_t &candidates, size_t start, size_t size, num_t min_dist, num_t xmid){
	for (size_t i = start; i < size; i++){
		if (abs(veccoor[i].first - xmid) > min_dist) continue;
		candidates.push_back(i);
	}
}

num_t check_strip_and_print(veccoor_t &veccoor, refcoor_t &candidates, num_t min_dist){
	num_t new_min_dist = min_dist;
	for (size_t i = 0; i < candidates.size(); i++){
		size_t need_to_check= min(candidates.size(),i+7);
		for (size_t j = i+1; j < need_to_check; j++){
			new_min_dist = min(new_min_dist, dist(veccoor[i], veccoor[j]));
		}
	}
	return new_min_dist;
}

#if 0
num_t closest_pair(veccoor_t &veccoor, vecx_t &vecx, size_t start, size_t size){

	num_t distL, distR;

#ifdef MY_DEBUG
	if (++i > 1000){
		cout << "Too many recursion steps (infinite loop?)"<<endl;
		throw 1;
	}
#endif

	if (size <= 1){
		return numeric_limits<num_t>::max();
	} else if (size == 2){
	cout << "RET:" << start << " " << start + size -1 << endl;
		print_coor(veccoor[start]);
	cout << " ";
		print_coor(veccoor[size-1]);
	cout << endl; 
		return dist(veccoor[start], veccoor[start + size-1]);
	}
	size_t pivot = partition(veccoor, vecx, start, size);
#if 1
	cout << "pivot: " << pivot << " start:" << start << " size:"<<size<<endl;
		cout << "\t\tL start: " << start<< " size:" << pivot -start<<endl;
		cout << "\t\tR start: " << pivot << " size:" << start+size-pivot<<endl;
#endif
	distL = closest_pair(veccoor, vecx, start, pivot - start);
//	cout << "L: " << distL << endl;
	distR = closest_pair(veccoor, vecx, pivot, start + size - pivot);
//	cout << "R: " << distR << endl;

	refcoor_t candidates;
	candidates.reserve(size); // at most O(n) in total
cout << "C:"<<candidates.size() << endl;

	cout << "L: " << distL << " R: " << distR << endl;
if (distR == 0 ) throw 1;

	num_t min_dist = min(distL, distR);
cout << "Min_dist:" << min_dist << endl;
	select_candidate(veccoor,candidates,start,size,min_dist,vecx[pivot]);
	return check_strip_and_print(veccoor, candidates, min_dist);
}
#endif
#endif

void select_candidate(refcoor_t &coors, veccoor_t &candidates, num_t min_dist, num_t xmid){
	refcoor_t::iterator it = coors.begin();
	for (; it != coors.end(); it++){
		if (abs(it->first - xmid) > min_dist) continue;
		candidates.push_back(*it);
	}
}

#if 1
num_t check_strip_and_print(veccoor_t &candidates, num_t min_dist){
	num_t new_min_dist = min_dist;
	for (size_t i = 0; i < candidates.size(); i++){
		size_t need_to_check= min(candidates.size(),i+7);
		for (size_t j = i+1; j < need_to_check; j++){
			double temp = dist(candidates[i], candidates[j]);

			if (temp <= new_min_dist){
	print_coor(candidates[i]);
				new_min_dist = temp;
			}
		}
	}
	return new_min_dist;
}
#endif

void partition(refcoor_t &coors, refcoor_t &L, refcoor_t &U, num_t x){
	refcoor_t::iterator it = coors.begin();
	size_t half = coors.size() / 2;
//cout << "P"<< half << endl;
	for (; it != coors.end(); it++){
		if (L.size() < half && it->first <= x){
			L.push_back(*it);
		} else {
			U.push_back(*it);
		}
//print_coor(*it);
	}
//cout << " x:" << x << endl;
}

num_t closest_pair_helper(refcoor_t &coors, vecx_t &vecx, size_t start, char temp){
#ifdef MY_DEBUG
	if (++i > 1000){
		cout << "Too many recursion steps (infinite loop?)"<<endl;
		throw 1;
	}
#endif 

	if (coors.size() <= 1){
//cout << temp << " INFNTY"<<endl;
		return numeric_limits<num_t>::max();
	} else if (coors.size() == 2){
//cout << temp << " dist:" << dist(*(coors.begin()), *(coors.rbegin())) << endl;
		return dist(*(coors.begin()), *(coors.rbegin()));
	}

	size_t pivot = start + coors.size() / 2;
//cout << "PIVOT:" << pivot<< " START:" << start << " " << vecx[pivot]<< endl;
	refcoor_t L,U;
	partition(coors, L, U, vecx[pivot]);
//cout << temp << " "<< coors.size() << " " << start << " " << L.size() <<  " " << U.size() << endl;
	num_t dL = closest_pair_helper(L, vecx, start, 'L');
	num_t dR = closest_pair_helper(U, vecx, start + L.size(), 'R');

	num_t min_dist = min(dL,dR);
	veccoor_t candidates;
	select_candidate(coors, candidates, min_dist, vecx[pivot]);
	return check_strip_and_print(candidates, min_dist);
}

num_t closest_pair(veccoor_t &veccoor, vecx_t &vecx){
	refcoor_t coors(veccoor.begin(),veccoor.end());
	return closest_pair_helper(coors, vecx, 0, 'A');
}

int main(){
	veccoor_t veccoor;
	vecx_t vecx; // store all coordinates
	size_t size = get_coor(veccoor, vecx);

#ifndef MY_DEBUG
	printall(veccoor,vecx);
#endif

	cout << closest_pair(veccoor, vecx) << endl;
}
