# Timetable Generator (Backtracking + CSP)

Timetable generation is harder than it looks.
You can't just assign subjects randomly — professors clash,
same subject shouldn't repeat in a day, and each subject
needs a fixed number of lectures per week.

This project treats it as a Constraint Satisfaction Problem
and solves it using backtracking in C++.

## How it works
Variables = time slots
Domain = subjects
Constraints = the scheduling rules

The algorithm tries to assign a subject to each slot.
If a constraint is violated, it backtracks and tries the next
option. Early pruning means it doesn't waste time exploring
paths that will obviously fail.

## Constraints it handles
- No professor double-booked in the same slot
- No subject repeating on the same day
- Weekly lecture count per subject met
- Takes dynamic input (you define subjects + requirements)

## Time complexity
Worst case: O(n^k) — n subjects, k slots
In practice much faster because of early constraint checks

## Tech
C++ — Maps, Sets, Vectors, Recursion
