#include <string>
#include <algorithm>
#include <stack>
#include <iostream>

using namespace std;

class Solution {
public:
    string decodeString(string s) {
        string ret;
        stack<string> st_str;
        string tmp;
        stack<int> st_num;
        bool in = false;
        int i = 0, n = s.size();
        while(i < n) {
            if (s[i] >= '0' && s[i] <= '9') {
                if (in) {
                    st_str.emplace(tmp);
                    tmp = "";
                    in = false;
                }
                int j = i;
                int num = 0;
                while(j < n && (s[j] >= '0' && s[j] <= '9')) {
                    num = num * 10 + (s[j] - '0');
                    j++;
                }
                st_num.push(num);
                i = j;
            } else if (s[i] == '[') {
                in = true;
                ++ i;
            } else if (s[i] == ']') {
                int num = 0;
                if (!st_num.empty()) num = st_num.top(); st_num.pop();
                string str = "";
                if (!st_str.empty()) str = st_str.top(); st_str.pop();
                while(num --) ret.append(str);
                i++;
            } else {
                if (in) tmp.push_back(s[i]);
                else ret.push_back(s[i]);
                ++ i;
            }
        }
        if (!st_str.empty()) ret.append(st_str.top());
        return ret;
    }
};

int main() {
  Solution ans;
  string ret = ans.decodeString("3[a]2[bc]");
  std::cout << ret << "\n";
  return 0;
}