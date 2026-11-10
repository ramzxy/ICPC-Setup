set<int> s = {5, 1, 8};             // sorted, no duplicates
s.insert(3);
s.erase(8);
bool has3 = s.count(3);             // count is 0 or 1
auto it = s.lower_bound(4);         // first element >= 4: 5
int first = *s.begin(), last = *s.rbegin();   // 1 and 5
multiset<int> ms = {2, 2, 7};       // like set, keeps duplicates
ms.erase(ms.find(2));               // removes ONE 2; erase(2): all
map<string, int> cnt;
cnt["apple"]++;                     // a new key starts at 0
bool pear = cnt.count("pear");      // check without adding it
for (auto& [key, val] : cnt) {}     // loops in sorted key order
priority_queue<int> maxpq;                  // biggest on top
priority_queue<int, vi, greater<>> minpq;   // smallest on top
maxpq.push(4); maxpq.push(9); minpq.push(4); minpq.push(9);
int big = maxpq.top(), small = minpq.top();   // 9 and 4
deque<int> dq = {1, 2};
dq.push_front(0); dq.pop_back();    // 0 1
vector<vector<int>> grid(3, vector<int>(4, 0));   // 3 x 4 of zeros
