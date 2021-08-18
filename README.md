# BitRecipeSolver
This finds the smallest values such that all given recipes of the form `a && b = c` and `b || c = a` hold.
You can interpret this as the integers such that `a&b = c` and `b|c = a` with bitwise operators, or the sets where `c` is the intersection of `a` and `b`, and `a` is the union of `b` and `c`.
You can *also* interpret it as alchemy recipes from the webcomic Homestuck, making the result captchalogue codes.

Since this is an NP-hard problem (it can be used to solve 3-SAT), the running time is technically speaking exponential. However, with "normal" recipes the program runs fine (and definitely better than a brute force approach).
