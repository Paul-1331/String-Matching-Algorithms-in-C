# String Matching Algorithms

A collection of classical string matching algorithms implemented in C, each beating the naïve brute-force O(n·m) bound.

---

## Table of Contents

- [What is String Matching?](#what-is-string-matching)
- [Why Does It Matter?](#why-does-it-matter)
- [The Brute-Force Baseline](#the-brute-force-baseline)
- [Algorithms](#algorithms)
  - [KMP — Knuth-Morris-Pratt](#1-kmp--knuth-morris-pratt)
  - [Z Algorithm](#2-z-algorithm)
  - [Rabin-Karp](#3-rabin-karp)
  - [Boyer-Moore (Bad Character)](#4-boyer-moore-bad-character-heuristic)
- [Complexity Summary](#complexity-summary)
- [Building & Running](#building--running)

---

## What is String Matching?

String matching is the problem of finding all occurrences of a **pattern** string `p` (length `m`) inside a **text** string `s` (length `n`). More formally:

> Find every index `i` such that `s[i..i+m-1] == p`.

It is one of the most fundamental problems in computer science, sitting at the intersection of algorithms, combinatorics, and information theory.

---

## Why Does It Matter?

String matching is not a niche academic exercise — it is everywhere:

| Domain | Example |
|---|---|
| **Text editors** | Ctrl+F / Find & Replace |
| **Bioinformatics** | Searching for a gene sequence inside a genome (billions of characters) |
| **Network security** | Deep packet inspection, intrusion detection signatures |
| **Search engines** | Substring and full-text indexing |
| **Compilers** | Lexical analysis / tokenisation |
| **Databases** | `LIKE` queries, `CONTAINS` predicates |

In many of these domains the text can be gigabytes long, so even a constant-factor improvement over brute force has measurable real-world impact.

---

## The Brute-Force Baseline

The naïve algorithm slides the pattern one position at a time and compares character by character at each position:

```
for i = 0 to n-m:
    for j = 0 to m-1:
        if s[i+j] != p[j]: break
    if j == m: report match at i
```

**Time complexity: O(n · m)**

In the worst case (e.g., text = `"aaaa...a"`, pattern = `"aaa...ab"`) almost every character in the text is compared against every character in the pattern. For large inputs this is unacceptably slow. All four algorithms below break this barrier.

---

## Algorithms

### 1. KMP — Knuth-Morris-Pratt

**File:** `KMP.c`

#### How it works

The key insight behind KMP is that when a mismatch occurs, we already know which prefix of the pattern matches a suffix of the text we just scanned. Instead of restarting the comparison from scratch, we can **reuse** that information.

KMP precomputes the **Longest Proper Prefix which is also a Suffix (LPS)** array for the pattern. `lps[i]` stores the length of the longest proper prefix of `p[0..i]` that is also a suffix of `p[0..i]`.

During matching, on a mismatch at pattern index `j`, instead of resetting `j` to 0, we jump to `j = lps[j-1]`. This ensures the text pointer `i` never moves backward — each character in the text is processed at most once.

**Example** — pattern `"AABAAB"`:

```
LPS: [0, 1, 0, 1, 2, 3]
```

On a mismatch at position 5, we fall back to position 3 instead of 0.

#### Time Complexity

| Phase | Complexity |
|---|---|
| LPS construction | O(m) |
| Matching | O(n) |
| **Total** | **O(n + m)** |

Space: O(m) for the LPS array.

---

### 2. Z Algorithm

**File:** `Z_algo.c`

#### How it works

For a string `s`, the **Z array** is defined such that `z[i]` is the length of the longest substring starting at `s[i]` that is also a prefix of `s`. (By convention `z[0]` is undefined / 0.)

The algorithm maintains a **Z-box** `[l, r]` — the rightmost interval `[l, r]` such that `s[l..r]` is a prefix of `s`. For each new index `i`:
- If `i` is inside the current Z-box (`i <= r`), we can initialise `z[i]` from the already-computed `z[i-l]` (the "mirror" position), capped at `r-i+1`.
- We then extend by brute force from the known boundary.
- If the new interval extends past `r`, we update the Z-box.

Each character is used to extend the Z-box at most once, giving linear time.

To use this for pattern matching, we form the **concatenated string** `t = p + '$' + s`, where `'$'` is a separator that does not appear in either string. A position `i` in `t` (with `i > m`) is a match if and only if `z[i] == m`.

#### Time Complexity

| Phase | Complexity |
|---|---|
| Z array construction | O(n + m) |
| Scan for matches | O(n) |
| **Total** | **O(n + m)** |

Space: O(n + m) for the concatenated string and Z array.

---

### 3. Rabin-Karp

**File:** `Rabin_Karp.c`

#### How it works

Rabin-Karp uses **polynomial hashing** to reduce string comparison to integer comparison. The hash of a string `s[0..k-1]` over a base `B` and modulus `MOD` is:

```
H = s[0]·B^(k-1) + s[1]·B^(k-2) + ... + s[k-1]·B^0  (mod MOD)
```

This implementation precomputes prefix hashes for the text, so the hash of any substring `s[l..r]` can be retrieved in O(1):

```
H(l, r) = hash[r] - hash[l-1] · B^(r-l+1)  (mod MOD)
```

The pattern hash is computed once (O(m)), and then for each of the n-m+1 windows in the text we do a single O(1) hash lookup and comparison.

**Note:** This implementation uses a single hash (BASE=31, MOD=10^9+7). Hash collisions are theoretically possible but extremely rare in practice. For cryptographically adversarial inputs, double hashing (two independent (base, mod) pairs) would eliminate false positives entirely.

#### Time Complexity

| Case | Complexity |
|---|---|
| Average / Expected | O(n + m) |
| Worst case (many collisions) | O(n · m) |

Space: O(n) for prefix hashes and powers.

---

### 4. Boyer-Moore (Bad Character Heuristic)

**File:** `Boyer_Moore.c`

#### How it works

Boyer-Moore is often the fastest algorithm in practice. Unlike KMP and Z, it scans the pattern **right to left** while sliding left to right across the text. On a mismatch between `p[j]` and `s[shift+j]`, it uses precomputed tables to skip ahead — potentially jumping many positions at once.

This implementation uses the **Bad Character heuristic**:

- Precompute `badchar[c]` = the rightmost index of character `c` in the pattern (or -1 if absent).
- On a mismatch at text position `shift+j`, shift the pattern right by `max(1, j - badchar[s[shift+j]])`.

This allows the algorithm to skip large chunks of the text when the mismatched character does not appear in the pattern (e.g., searching English text for a pattern with rare characters).

**Important caveat:** The full Boyer-Moore algorithm also uses a **Good Suffix heuristic**, which provides O(n/m) best-case performance and guarantees O(n+m) worst-case. This implementation omits the good suffix table, so the worst-case complexity remains O(n·m) on adversarial inputs (e.g., all-same-character strings). It performs excellently on natural language and random text.

#### Time Complexity

| Case | Complexity |
|---|---|
| Best case (large alphabet, rare chars) | O(n/m) |
| Average (natural text) | O(n) |
| Worst case (bad character only) | O(n · m) |

Space: O(m + σ) where σ = alphabet size (256 here).

---

## Complexity Summary

| Algorithm | Preprocessing | Matching | Worst Case | Space |
|---|---|---|---|---|
| Brute Force | O(1) | O(n · m) | O(n · m) | O(1) |
| KMP | O(m) | O(n) | **O(n + m)** | O(m) |
| Z Algorithm | O(n + m) | O(n) | **O(n + m)** | O(n + m) |
| Rabin-Karp | O(m) | O(n) avg | O(n · m) worst | O(n) |
| Boyer-Moore (bad char only) | O(m + σ) | O(n) avg | O(n · m) worst | O(m + σ) |

---

## Building & Running

Each file is a standalone C program. Compile with any C99-compatible compiler:

```bash
gcc -O2 -o kmp KMP.c
gcc -O2 -o z_algo Z_algo.c
gcc -O2 -o rabin_karp Rabin_Karp.c
gcc -O2 -o boyer_moore Boyer_Moore.c
```

Each program reads the **text** on the first line and the **pattern** on the second line from standard input, then prints the 0-indexed position(s) of each match, or `-1` if none found.

```bash
echo -e "abcabcabc\nabc" | ./kmp
# Output: 0 3 6

echo -e "abcabcabc\nabc" | ./z_algo
# Output: 0 3 6
```

> **Input constraints:** Text up to 10^6 characters, pattern up to 10^5 characters (10^7 for Boyer-Moore).
