string line;
getline(cin >> ws, line);  // whole line; ws skips the old newline
stringstream ss(line);     // split that line into words
string word;
while (ss >> word) { /* use word */ }
int x;
while (cin >> x) { /* ... */ }  // read numbers until input ends
