// a pile of i stones; a move takes s stones for some s in S; who
// cannot move loses. win[i] = the player to move wins
vector<bool> winning(int n, const vi& S) {
    vector<bool> win(n + 1, false);   // win[0] = false: no move
    rep(i, 1, n + 1) for (int s : S)
        if (s <= i && !win[i - s]) win[i] = true;  // move to a loss
    return win;
}
