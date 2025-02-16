#include <vector>
#include <algorithm>
#include <cassert>
#include <unordered_map>

using namespace std;


class Solution {

	typedef long long ll;

	void getCommonPoints(const vector<vector<int>>& paths, vector<int>& points) const {
		int mlen = paths[0].size();
		points = paths[0];
		std::sort(points.begin(), points.end());
		for (int i = 1; i < paths.size(); i++) {
			auto sorted = paths[i];
			std::sort(sorted.begin(), sorted.end());
			vector<int> tmp;
			std::set_intersection(points.begin(), points.end(), sorted.begin(), sorted.end(),
				std::back_inserter(tmp));
			std::swap(tmp, points);
			if (points.empty()) {
				return;
			}
		}
	}

	void makePrefixSumsAndIdxs(const vector<vector<int>>& paths, 
		vector<vector<ll>>& psums,
		vector<unordered_map<int, vector<int>>> & idxsMaps)const {
		for (const auto& path : paths) {
			ll sum = 0;
			vector<ll> psum;
			unordered_map<int, vector<int>> umap;
			for (int i = 0; i < path.size(); i++) {
				sum += path[i];
				psum.push_back(sum);
				umap[path[i]].push_back(i);
			}
			psums.push_back(std::move(psum));
			idxsMaps.push_back(std::move(umap));
		}
	}
	bool hasCommonPathOfLen(
		const vector<int>& path1,
		const vector<int>& path2,
		const vector<ll>& psums1,
		const vector<ll>& psums2,
		const unordered_map<int, vector<int>> & idxMap1, 
		const unordered_map<int, vector<int>> & idxMap2, 
		int point, int len)const
	{
		unordered_map<ll, vector<int>> umap1;
		auto f1 = idxMap1.find(point);
		for (int i : f1->second) {
			if (i + len <= path1.size()) {
				umap1[psums1[i + len - 1] - psums1[i]].push_back(i);
			}
			else {
				break;
			}
		}
		unordered_map<ll, vector<int>> umap2;
		auto f2 = idxMap2.find(point);
		for (int i : f2->second) {
			if (i + len <= path2.size()) {
				umap2[psums2[i + len - 1] - psums2[i]].push_back(i);
			}
			else {
				break;
			}
		}
		for (auto& entry : umap1) {
			auto f = umap2.find(entry.first);
			if (f != umap2.end()) {
				for (int idx1 : entry.second) {
					for (int idx2 : f->second) {
						if (std::equal(path1.begin() + idx1, path1.begin() + idx1 + len,
							path2.begin() + idx2))
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

public:
	int longestCommonSubpath(int n, vector<vector<int>>& paths) {
		vector<int>  commonPoints;
		getCommonPoints(paths, commonPoints);
		if (commonPoints.empty()) {
			return 0;
		}
		vector<vector<ll>> prefixSums;
		vector<unordered_map<int, vector<int>>> idxsMaps;
		makePrefixSumsAndIdxs(paths, prefixSums, idxsMaps);
		commonPoints.erase(std::unique(commonPoints.begin(), commonPoints.end()),
			commonPoints.end());
		int lcpath = 1;
		for (auto point : commonPoints) {
			int low = lcpath;
			int high = paths[0].size() - idxsMaps[0][point][0];
			while (low < high) {
				int mid = low + (high - low) / 2;
				if (mid == low) {
					mid++;
					low++;
				}
				bool found = true;
				for (int i = 1; i < paths.size(); i++) {
					if (!hasCommonPathOfLen(paths[0], paths[i],
						prefixSums[0], prefixSums[i],
						idxsMaps[0], idxsMaps[i],
						point, mid)) {
						found = false;
						break;
					}
				}
				if (found) {
					low = mid;
					if (mid > lcpath) {
						lcpath = mid;
					}
				}
				else {
					high = mid;
				}
				if (high <= lcpath) {
					break;
				}
			}
		}
		return lcpath;
	}
};

int main()
{
	Solution sol;
	vector<vector<int>> paths = { {0,1,2,3,4},{2,3,4},{4,0,1,2,3} };
	int res = sol.longestCommonSubpath(5, paths);
	assert(res == 2);

	paths = { {0},{1},{2 } };
	res = sol.longestCommonSubpath(3, paths);
	assert(res == 0);


	paths = { {0,1,2,3,4},{4,3,2,1,0} };
	res = sol.longestCommonSubpath(5, paths);
	assert(res == 1);

	paths = { {0,1,0,1,0,1,0,1,0},{0,1,3,0,1,4,0,1,0} };
	res = sol.longestCommonSubpath(5, paths);
	assert(res == 3);

	paths = { {2,1,4,0,3},{2,1,4,0,3} };
	res = sol.longestCommonSubpath(10, paths);
	assert(res == 5);

	paths = { {1,2,3,4,3,4,2,1},{1,2,5,2,1}, {2,1,3,4,3,4,3,4} };
	res = sol.longestCommonSubpath(10, paths);
	assert(res == 2);

	
	return 0;
}

