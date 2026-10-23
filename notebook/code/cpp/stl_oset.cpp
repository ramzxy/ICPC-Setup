// set with "k-th smallest" and "how many are < x", both O(log n)
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
template <class T>
using oset = tree<T, null_type, less<T>, rb_tree_tag,
                  tree_order_statistics_node_update>;
// oset<int> s; s.insert(5); s.erase(5);
// *s.find_by_order(k): k-th smallest, k from 0
// s.order_of_key(x):   how many elements are < x
// duplicates: store pairs {value, unique index}
