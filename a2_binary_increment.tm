# Binary Increment (binary_increment.tmp)
# Increments the value of a binary number by 1
# States - Q
0 1 2 H
# Alphabet - Sigma
0 1 ^
# Input Word - word of I
^ 1 1 0 1 1 0 1 0 1
# ^ 1 1 1 1 1 1 1 1 1
# Initial State - q0
0
# Final States - F
H
# Instruction table (delta)
0 ^ 1 ^ +1
0 0 0 0 -1
0 1 0 1 -1
1 ^ 2 1 -1
1 0 2 1 +1
1 1 1 0 +1
2 ^ H ^ +1
2 0 2 0 -1
2 1 2 1 -1