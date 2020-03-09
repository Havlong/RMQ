#include <bits/stdc++.h>

using namespace std;

const int testSize = 1000000;
const int INF = INT_MAX;

vector<int> sqrtRMQ(const vector<int> &data, const vector<pair<int, int>> &queries) {
    const int sqrt = 1000;
    vector<int> sqrtDecomposition(((int) data.size() + sqrt - 1) / sqrt, INF);
    for (int i = 0; i < (int) data.size(); ++i) {
        sqrtDecomposition[i / sqrt] = min(sqrtDecomposition[i / sqrt], data[i]);
    }
    vector<int> answers(queries.size(), INF);
    for (int i = 0; i < (int) queries.size(); ++i) {
        int l = queries[i].first;
        int r = queries[i].second;
        int lSqrt = l / sqrt;
        int rSqrt = r / sqrt;
        if (lSqrt == rSqrt) {
            for (int j = l; j <= r; ++j) {
                answers[i] = min(answers[i], data[j]);
            }
        } else {
            for (int j = l; j < (lSqrt + 1) * sqrt; ++j) {
                answers[i] = min(answers[i], data[j]);
            }
            for (int s = lSqrt + 1; s <= rSqrt - 1; ++s) {
                answers[i] = min(answers[i], sqrtDecomposition[s]);
            }
            for (int j = rSqrt * sqrt; j <= r; ++j) {
                answers[i] = min(answers[i], data[j]);
            }
        }
    }
    return answers;
}

vector<int> tree;

void update(const int pos, const int ql, const int &qr, const int &index, const int &val) {
    if (ql == qr) {
        tree[pos] = val;
        return;
    }
    int mid = (ql + qr) / 2;
    if (index <= mid) {
        update(pos * 2, ql, mid, index, val);
    } else {
        update(pos * 2 + 1, mid + 1, qr, index, val);
    }
    tree[pos] = min(tree[pos * 2], tree[pos * 2 + 1]);
}

int query(const int pos, const int ql, const int &qr, const int &l, const int &r) {
    if (ql > r || qr < l) {
        return INF;
    }
    if (ql >= l && qr <= r) {
        return tree[pos];
    }
    int mid = (ql + qr) / 2;
    return min(query(pos * 2, ql, mid, l, r), query(pos * 2 + 1, mid + 1, qr, l, r));
}

vector<int> segmentRMQ(const vector<int> &data, const vector<pair<int, int>> &queries) {
    tree.clear();
    tree.resize(data.size() * 4 + 1);
    fill(tree.begin(), tree.end(), INF);
    for (int i = 0; i < (int) data.size(); ++i) {
        update(1, 0, (int) data.size() - 1, i, data[i]);
    }
    vector<int> answers(queries.size());
    for (int i = 0; i < (int) queries.size(); ++i) {
        int l = queries[i].first;
        int r = queries[i].second;
        answers[i] = query(1, 0, (int) data.size() - 1, l, r);
    }
    return answers;
}

int main() {
    mt19937 rnd(static_cast<unsigned>(chrono::steady_clock::now().time_since_epoch().count()));
    vector<int> randomData(testSize);
    for (int i = 0; i < testSize; ++i) {
        randomData[i] = rnd();
    }
    uniform_int_distribution<> intDistribution(0, testSize - 1);
    vector<pair<int, int>> randomRanges(testSize);
    for (int i = 0; i < testSize; ++i) {
        randomRanges[i].first = intDistribution(rnd);
        randomRanges[i].second = intDistribution(rnd);
        if (randomRanges[i].first > randomRanges[i].second)
            swap(randomRanges[i].first, randomRanges[i].second);
    }
    auto start = chrono::steady_clock::now();
    vector<int> sqrtResult = sqrtRMQ(randomData, randomRanges);
    auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();
    cout << "Duration of processing " << testSize << " elements with SQRT-Decomposition is " << duration << " ms\n";
    start = chrono::steady_clock::now();
    vector<int> segmentResult = segmentRMQ(randomData, randomRanges);
    duration = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - start).count();
    cout << "Duration of processing " << testSize << " elements with Segment Tree is " << duration << " ms\n";
    bool equal = true;
    for (int i = 0; i < (int) sqrtResult.size(); ++i) {
        if (sqrtResult[i] != segmentResult[i]) {
            equal = false;
            cout << "Difference in " << i << " test\n";
        }
    }
    if (equal)
        cout << "Success";
    return 0;
}