#include <bits/stdc++.h>
using namespace std;

struct Transaction {
    int user;
    int merchant;
    int amount;
    int time;
};

int main() {
    vector<Transaction> txns;

    ifstream file("transactions.csv");
    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string temp;
        Transaction t;

        getline(ss, temp, ','); t.user = stoi(temp);
        getline(ss, temp, ','); t.merchant = stoi(temp);
        getline(ss, temp, ','); t.amount = stoi(temp);
        getline(ss, temp, ','); t.time = stoi(temp);

        txns.push_back(t);
    }

    unordered_map<int,int> risk;
    for (auto &t : txns) risk[t.user] = 0;

    unordered_map<int, vector<int>> userTime;
    for (auto &t : txns) userTime[t.user].push_back(t.time);

    for (auto &p : userTime) {
        auto times = p.second;
        sort(times.begin(), times.end());
        for (int i = 0; i + 2 < times.size(); i++) {
            if (times[i+2] - times[i] <= 2) {
                risk[p.first] += 40;
                break;
            }
        }
    }

    unordered_map<int, pair<int,int>> avg;
    for (auto &t : txns) {
        avg[t.user].first += t.amount;
        avg[t.user].second++;
    }

    for (auto &t : txns) {
        int average = avg[t.user].first / avg[t.user].second;
        if (t.amount > 5 * average) {
            risk[t.user] += 30;
        }
    }

    unordered_map<int, unordered_set<int>> graph;
    for (auto &t : txns) graph[t.user].insert(t.merchant);

    for (auto &p : graph) {
        if (p.second.size() >= 3) {
            risk[p.first] += 30;
        }
    }

    priority_queue<pair<int,int>> pq;
    for (auto &p : risk) pq.push({p.second, p.first});

    cout << "Top Risky Users:\n";
    while (!pq.empty()) {
        auto x = pq.top(); pq.pop();
        cout << "User " << x.second << " Risk Score = " << x.first << endl;
    }

    return 0;
}
