# Build-your-own-regex
https://en.wikipedia.org/wiki/Regular_expression
https://en.wikipedia.org/wiki/Finite-state_machine
https://en.wikipedia.org/wiki/Deterministic_finite_automaton

Lexer → Abstract syntax tree → Thompson construction

## Lexer
https://en.wikipedia.org/wiki/Lexical_analysis

## Abstract syntax tree
https://en.wikipedia.org/wiki/Abstract_syntax_tree

## Thompson construction
https://en.wikipedia.org/wiki/Thompson%27s_construction

## Backtracking
https://en.wikipedia.org/wiki/Regular_expression#Implementations_and_running_times
https://en.wikipedia.org/wiki/Backtracking

## Features
| Type            | Symbol  | Description                                               | Example                                                         |
|-----------------|---------|-----------------------------------------------------------|-----------------------------------------------------------------|
| Literal         | `a`     | Matches the exact character                               | `cat` matches "cat"                                             |
| Alternation     | `a\|b`  | Matches either the left or right                          | `cat\|dog` matches "cat" or "dog"                               |
| Concatenation   | `ab`    | Matches expressions in sequence                           | `ca` matches "ca" in "cat"                                      |
| Zero or More    | `a*`    | Matches zero or more repetitions of the preceding element | `ba*` matches "b", "ba", "baa", etc.                            |
| Zero or One     | `a?`    | Matches zero or one occurrence of the preceding element   | `colou?r` matches "color" or "colour"                           |
| One or More     | `a+`    | Matches one or more repetitions of the preceding element  | `ba+` matches "ba", "baa", but not "b"                          |
| Wildcard        | `.`     | Matches any single character                              | `c.t` matches "cat", "cot", "c9t", etc.                         |
| Character Range | `[a-z]` | Matches any character within the specified range          | `[0-9]` matches any digit, `[a-z]` matches any lowercase letter |

## Challenges
- Memory management with `unique_ptr` and `const`