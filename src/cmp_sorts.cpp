#include "sorts.h"
#include <functional>
#include <chrono>
#include <iostream>

struct Sort {
	function<void(vector<int>&)> func;
	string name;
};

vector<int> gen_rand_vec(int n) {
	vector<int> arr(n);
	
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<mt19937::result_type> dist(1, 1000);
	
	for (int i=0;i<n;++i)
		arr[i] = dist(rng);

	return arr;
}

void print_array(vector<int> &arr) {
	int n = arr.size();
	for (int i=0;i<n;++i)
		cout << arr[i] << ' ';
	cout << endl;
}

int time_sort(function<void(vector<int>&)> sort, vector<int> arr) {
	auto start = chrono::high_resolution_clock::now();
	sort(arr);
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop-start);
	
	return duration.count();
}


int main() {
	vector<Sort> sorts = {{bubble_sort, "Bubble Sort"}, {quick_sort, "Quick Sort"}};
	
	for (int n=10;n<=100'000;n*=10) {
		cout << "Size = " << n << ":" <<endl;
		vector<int> arr = gen_rand_vec(n);
		if (n == 10)
			print_array(arr);

		for (Sort s : sorts)
			cout << '\t' << s.name << ": " << (double)time_sort(s.func, arr)/1000.0 << " ms" << endl;
	}
}

