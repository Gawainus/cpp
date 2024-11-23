#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Solution {
public:

    bool match(const vector<size_t>& bag, const unordered_map<size_t, size_t>& id_to_count)
    {
        unordered_map<size_t, size_t> counts;
        for (size_t i = 0; i < bag.size(); ++i) {
            counts[i] = bag[i];
        }

        cout << "Bag:" << endl;
        for (size_t b: bag) {
            cout << b << " ";
        }
        cout << endl;


        cout << "id_to_count:" << endl;
        for(auto[id, count]: id_to_count) {
            cout << id << ": " << count << " ";
        }
        cout << endl;

        return  counts == id_to_count;
    }

    vector<int> findSubstring(string s, vector<string>& words)
    {
        const size_t n = s.size();
        const size_t ws = words.size();
        const size_t wl = words[0].size();
        const size_t bag_len = ws * wl;

        if (bag_len > n) {
            return {};
        }


        unordered_map<string, size_t> word_to_id;
        unordered_map<size_t, size_t> id_to_count;
        for (size_t i = 0; i < words.size(); ++i) {
            const string& w = words[i];
            if (!word_to_id.contains(w)) {
                word_to_id[w] = i;
                id_to_count[word_to_id[w]] = 0;
            }

            id_to_count[word_to_id[w]] += 1;
        }


        vector<int> sol;
        for (size_t i = 0; i <= n - bag_len; ++i) {
            vector<size_t> bag(word_to_id.size(), 0);
            bool bad_word = false;
            for (size_t j = 0; j < bag_len; j += wl) {
                const string& w = s.substr(i+j, wl);
                cout << "processing word: " << w << endl;

                if (word_to_id.contains(w)) {
                    int id = word_to_id[w];
                    cout << id << ": " << w << endl;
                    bag[id] += 1;

                } else {
                    cout << "bad word: " << w << endl;
                    bad_word = true;
                    break;
                }

            }

            if (bad_word) {
                cout << "bad word continue" << endl;

                continue;
            }

            cout << "i: " << i << " n - bag_len: " << n - bag_len << "\n" << endl;

            bool is_cs = match(bag, id_to_count);
            if (is_cs) {
                sol.push_back(i);
            }
        }

        return sol;
    }
};

int main()
{
    Solution sol;
    vector<string> words {"a","a","b","b","c"};
    auto s = sol.findSubstring("aaabbbc", words);

    cout << "Final solution: ";

    for (auto i: s) {
        cout << i << "\n";

    }

    cout << endl;
    return 0;
}