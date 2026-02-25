#include "../src/cmp_sorts.h"
#include <assert.h>
#include <sstream>
#include <string>

void bubble_sort_test() {
	vector<int> arr = {100, 1, 6, 67};
	vector<int> sorted_arr = {1, 6, 67, 100};
	bubble_sort(arr);
	assert(arr == sorted_arr);
}

void quick_sort_test() {
	vector<int> arr = {2, 4, 8, 16, 32};
	vector<int> sorted_arr = {2, 4, 8, 16, 32};
	bubble_sort(arr);
	assert(arr == sorted_arr);
}

void gen_rand_vec_test() {
	int n = 100;
	vector<int> arr = gen_rand_vec(n);
	assert(arr.size() == n);
}

void print_array_test() {
	vector<int> arr = {2, 3, 5, 7, 9, 11};
	streambuf* cout_buf = cout.rdbuf();
	stringstream printed_array;
	cout.rdbuf(printed_array.rdbuf());
	print_array(arr);
	cout.rdbuf(cout_buf);
	assert("2 3 5 7 9 11 \n" == printed_array.str());
}

void time_sort_test() {
	vector<int> arr = gen_rand_vec(10);
	int res = time_sort(quick_sort, arr);
	assert(res >= 0);
}

int main() {
	bubble_sort_test();	

	quick_sort_test();

	gen_rand_vec_test();

	print_array_test();

	time_sort_test();
}
