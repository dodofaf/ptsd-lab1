#include <random>
#include <vector>
#include <utility>

using namespace std;

void bubble_sort(vector<int> &arr) {
	for (int i=arr.size();i>1;--i)
		for (int j=0;j<i-1;++j)
			if (arr[j] > arr[j+1])
				swap(arr[j], arr[j+1]);
}

void quick_sort_rec(vector<int> &arr, int l, int r) {
	if (r <= l)
		return;
	
	int p = (rand() % (r-l)) + l;
	int i = l, j = r;

	while (true) {
		do ++i;
		while (i < arr.size() && arr[i] < arr[p]);
		
		do --j;
		while (j >= 0 && arr[j] > arr[p]);
	
		if (i >= j) break;

		swap (arr[i], arr[j]);
	}

	quick_sort_rec(arr, l, p-1);
	quick_sort_rec(arr, p+1, r);
}

void quick_sort(vector<int> &arr) {
	quick_sort_rec(arr, 0, arr.size()-1);
}
