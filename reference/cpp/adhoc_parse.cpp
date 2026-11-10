// split "a,b,,c" at ',' -> {"a", "b", "", "c"}
vector<string> split(const string& s, char sep) {
    vector<string> parts(1);
    for (char c : s) {
        if (c == sep) parts.push_back("");
        else parts.back() += c;
    }
    return parts;
}
// the numbers inside text: "12:30-14:05" -> {12, 30, 14, 5}.
// Minus signs are dropped
vi numbersIn(string s) {
    for (char& c : s) if (!isdigit(c)) c = ' ';
    stringstream ss(s);
    vi nums;
    int x;
    while (ss >> x) nums.push_back(x);
    return nums;
}
