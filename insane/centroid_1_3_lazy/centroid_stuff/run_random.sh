make brute_force
make $1
echo 10000 10000 | ./query_gen > giant_test.in 
./brute_force < giant_test.in 1> giant_test.out.bf
./$1 < giant_test.in 1> giant_test.out
diff giant_test.out giant_test.out.bf