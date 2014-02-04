#include <vector>
#include <iostream>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
using namespace std;


void filterAdjacentRepeatPoint() {
	int data[] = {1,2,2,3,4,5,6};
	int indices[] = {2, 0, 2, 3, 2, 6};

	vector<int> points;
	vector<int> segmentColors;

	int dataSize = sizeof(data) / sizeof(data[0]);
	int indiceSize = sizeof(indices) / sizeof(indices[0]);

	// 先分散到segmentColors数组
	// segmentColors记录连点线段之间的颜色，最后一个元素没意义。
	int colorSec = 0;
	for (int cursor = 0; cursor < dataSize; cursor++) {
		int color = indices[colorSec * 3];
		// int begin = mFatColorsCache.get(colorSec * 3 + 1);
		int end = indices[colorSec * 3 + 2];
		if (end == cursor && colorSec < indiceSize / 3 - 1) {
			colorSec++;
			color = indices[colorSec * 3];
		}
		int var = data[cursor];
		if (points.size() == 0 || var != points[points.size() - 1]) {
			points.push_back(var);
			if (cursor == dataSize - 1 || var != data[cursor+1]) {
				segmentColors.push_back(color);	
			}
		}
	}

	// 聚合segmentColors数组！
	// 会改变原来颜色索引区间，FIXME 导致路况段跟索引段不匹配！

	vector<int> fatColorsCache;

	int idx = 0;
	while (idx < segmentColors.size()) {
		int color = segmentColors[idx];
		int begin = idx;

		// 不取最后一个元素
		for (; idx < segmentColors.size() - 1 && color == segmentColors[idx]; idx++)
			;

		fatColorsCache.push_back(color);
		fatColorsCache.push_back(begin);
		fatColorsCache.push_back(idx);

		if (idx == segmentColors.size() - 1) { // 取到末尾！
			break;
		}
	} // while loop

	// fatColorsCache 中存结果颜色
	// points中村结果顶点
	
	copy(segmentColors.begin(), segmentColors.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	copy(points.begin(), points.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	copy(fatColorsCache.begin(), fatColorsCache.end(), ostream_iterator<int>(cout, " "));
	cout << endl;
}

int main()
{	
	filterAdjacentRepeatPoint();

	getchar();

	return 0;
}