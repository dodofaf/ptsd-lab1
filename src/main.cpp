#include "cmp_sorts.h"

int main() {
	vector<Sort> sorts = {{bubble_sort, "Bubble Sort"}, {quick_sort, "Quick Sort"}};
	cmp_sorts(sorts);
}
