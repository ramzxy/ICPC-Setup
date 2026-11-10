string s = "Hello World";
string w = s.substr(6, 5);              // "World": start, length
size_t at = s.find("lo");               // 3, or string::npos
bool found = s.find("xyz") != string::npos;   // false
int x = stoi("42");
ll y = stoll("12345678901");
double d = stod("3.5");
string t = to_string(17);
char ch = 'e';
int k = ch - 'a';                       // 'a' -> 0, ..., 'z' -> 25
char back = 'a' + k;                    // 'e'
bool dig = isdigit('7'), let = isalpha('x');
for (char& c : s) c = tolower(c);       // "hello world"
string stars(3, '*');                   // "***"
string r = s;
reverse(all(r));                        // "dlrow olleh"
