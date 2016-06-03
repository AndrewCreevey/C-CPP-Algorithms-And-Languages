# Busy Beaver 2-state 3-symbol (busy_beaver.tm)
# 1-way infinite tape variant described by T.R.S Walsh
# States - Q
a b H
# Alphabet - Sigma
0 1 ^
# Input Word - word of I
^
# Initial State - q0
a
# Final States - F
H
# Instruction table (delta)
a ^ b 0 +1
a 0 b 0 +1
a 1 b 0 -1
b ^ a 0 -1
b 0 b 1 +1
b 1 H 1 +1 