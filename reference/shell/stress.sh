# Stress test: runs sol and brute on random tests from gen until
# their outputs differ. Usage: bash stress.sh
for f in gen sol brute; do
    g++ -std=c++17 -O2 $f.cpp -o $f || exit 1
done
for i in $(seq 1 1000); do
    ./gen $i > in.txt
    ./sol < in.txt > sol.out || { echo "sol: crash"; exit 1; }
    ./brute < in.txt > brute.out || { echo "brute: crash"; exit 1; }
    if ! diff -w sol.out brute.out > /dev/null; then
        echo "Test $i is different. Input:"; cat in.txt
        echo "sol:"; cat sol.out
        echo "brute:"; cat brute.out
        exit 1
    fi
done
echo "1000 tests OK"
