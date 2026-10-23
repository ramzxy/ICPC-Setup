auto sq = [](ll v) { return v * v; };
int calls = 0;
auto bump = [&]() { calls++; };   // [&]: can change outer vars
bump(); bump();                   // calls == 2
// recursive lambda: give it itself as the first argument
auto fib = [&](auto&& self, int i) -> ll {
    return i < 2 ? i : self(self, i - 1) + self(self, i - 2);
};
ll f10 = fib(fib, 10);            // 55
// or: function<ll(int)> f = [&](int i) -> ll { ... f(i - 1) ... };
