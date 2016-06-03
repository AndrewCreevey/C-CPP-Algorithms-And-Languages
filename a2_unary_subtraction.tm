# Unary Subtraction (unary_subtraction.tm)
# Subtracts two unary numbers
# States - Q
0 1 2 3 4 5 6 7 H
# Alphabet - Sigma
0 1 ^
# Input Word - word of I
^ 1 1 1 ^ 1 1 1 1 1 1 1
# Initial State - q0
1
# Final States - F
H
# Instruction Table - Delta
0 ^ 1 ^ -1
0 0 0 0 -1
0 1 0 1 -1
1 ^ 2 ^ +1
1 0 1 0 -1
1 1 1 1 -1
2 ^ 4 ^ -1
2 0 2 0 +1
2 1 3 0 +1
3 ^ 7 ^ +1
3 0 3 0 +1
3 1 3 1 +1
4 ^ 5 ^ +1
4 0 4 0 -1
4 1 4 1 -1
5 ^ 6 ^ +1
5 0 5 ^ +1
5 1 5 1 +1
6 ^ H ^ -1
6 0 6 ^ +1
6 1 6 1 +1
7 ^ H ^ -1
7 0 7 0 +1
7 1 0 0 -1