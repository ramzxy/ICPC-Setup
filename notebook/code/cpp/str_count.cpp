// cnt[c] = how often letter c occurs in s ('a' -> 0, 'z' -> 25).
// Upper case: ch - 'A'. Any character: int cnt[256] = {};
// and cnt[(unsigned char)ch]++
int cnt[26] = {};
for (char ch : s) cnt[ch - 'a']++;
// anagrams (same letters, other order): same cnt arrays
