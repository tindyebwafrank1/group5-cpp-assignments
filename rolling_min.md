# rolling_min

**Group 10 — Rolling Operations**
Source file: `rolling_min.cpp`

Re-implementation of the rolling minimum operation provided by dataframe engines such as [Polars](https://docs.pola.rs/api/python/stable/reference/dataframe/index.html) and Pandas.

Given a numeric column and a window size *w*, the operation returns a column of the same length where position *i* holds the smallest value among the *w* most recent observations (positions *i−w+1* through *i*). Positions that do not yet have enough observations are filled with `NaN`.

```
data      :  4    8    6    1    9    3    7    2    5    10
window = 3:  nan  nan  4    1    1    1    3    2    2    2
                       ^
                       min(4, 8, 6) = 4
```

---

## 1. OOP Concepts Used

### Abstraction

`RollingOperation` is an **abstract base class**. It declares *what* every rolling operation must be able to do without committing to *how*:

```cpp
virtual vector<double> apply(const vector<double>& data) const = 0;
virtual string name() const = 0;
```

The `= 0` makes these **pure virtual functions**, so `RollingOperation` cannot be instantiated directly — it exists purely as an interface. A caller can work with "some rolling operation" without knowing or caring which one.

### Inheritance

`RollingMin` derives from `RollingOperation`:

```cpp
class RollingMin : public RollingOperation { ... };
```

It inherits the shared state (`window`, `min_periods`), the validation logic, and the accessor methods. It only has to supply the one thing that is genuinely specific to it: the sliding-window minimum algorithm. If the other four Group 10 operations (`rolling_max`, `rolling_std`, `moving_average`, `exponential_moving_average`) are added later, they reuse the same base class rather than duplicating the parameter handling and error checking.

### Encapsulation

The fields `window` and `min_periods` are `protected`, not `public`. Outside code cannot reach in and set `window = -5` after construction, which would silently corrupt every subsequent result. Read access is granted deliberately through `const` accessors:

```cpp
int get_window() const { return window; }
int get_min_periods() const { return min_periods; }
```

All validation happens once, in the constructor, so **an object that exists is an object whose parameters are valid** — a class invariant. `apply()` never has to re-check them.

### Polymorphism

Because `apply()` and `name()` are virtual, a base-class pointer dispatches to the derived implementation at runtime:

```cpp
RollingOperation* op = &rm3;
op->apply(sample);   // calls RollingMin::apply
```

This is what allows a future driver program to hold a `vector<RollingOperation*>` of mixed operations and apply all of them to a column in a single loop.

### Constructor overloading / default arguments

```cpp
RollingMin(int window_size, int min_obs = -1);
```

`RollingMin(3)` and `RollingMin(3, 1)` are both valid. The default `-1` is a **sentinel value** meaning "use the window size", which reproduces the Pandas/Polars default of `min_periods == window`.

### Virtual destructor

```cpp
virtual ~RollingOperation() {}
```

Without `virtual`, deleting a `RollingMin` through a `RollingOperation*` would be undefined behaviour and would skip the derived destructor. Any class intended to be inherited from and deleted polymorphically needs this.

### `const` correctness

`apply()`, `name()`, and both accessors are marked `const`, guaranteeing at compile time that calling them cannot mutate the object. The input is taken as `const vector<double>&` — passed by reference to avoid copying the entire column, and `const` to promise the caller's data is never modified.

---

## 2. Algorithm

### The naive approach and why it is rejected

The obvious implementation is: for each position, loop over the last *w* elements and take the smallest. That is **O(n × w)**. On a dataframe column of a million rows with a 1000-element window, that is a billion comparisons — unusable.

### The monotonic deque (sliding-window minimum)

The implementation instead runs in **O(n) total**, using a double-ended queue holding **indices** into the data, maintained so their corresponding values are always strictly increasing from front to back.

The key insight:

> If `data[j] >= data[i]` and `j < i`, then index `j` is **useless forever**. Any window that still contains `j` also contains `i` (since `i` is newer), and `i`'s value is smaller or equal. So `j` can never be the answer again and can be discarded permanently.

This is why elements can be thrown away and never revisited, and it is what collapses the cost from O(n × w) to O(n).

### Main steps

For each element at index `i`:

**Step 1 — Pop dominated candidates from the back**

```cpp
while (!dq.empty() && data[dq.back()] >= v) dq.pop_back();
```

Remove every index at the back whose value is greater than or equal to the incoming value. By the insight above, they are permanently irrelevant. This is what preserves the increasing order of the deque.

**Step 2 — Push the new index**

```cpp
dq.push_back(i);
```

The newest element is always a valid candidate — it is in the current window and will outlive everything already present.

**Step 3 — Evict the front if it has expired**

```cpp
while (dq.front() <= i - window) dq.pop_front();
```

The window covers indices `i-window+1 .. i`. Anything at or below `i - window` has slid out and must go. Only the front can expire, because the deque is ordered by index.

**Step 4 — Read the answer and emit**

```cpp
const int count = min(i + 1, window);
if (count >= min_periods) result[i] = data[dq.front()];
```

The front is now the index of the smallest value in the current window — **O(1) lookup**. It is written to the output only if enough observations have accumulated; otherwise the position keeps its initial `NaN`.

### Complexity

| Measure | Cost |
| --- | --- |
| Time | **O(n)** total — amortised O(1) per element |
| Space | **O(w)** — the deque never holds more than one window of indices |

The amortisation argument: every index is pushed exactly once and popped at most once across the entire run. The inner `while` loops may run many times at one position, but the total number of pops over the whole pass cannot exceed the total number of pushes, which is *n*.

### Worked trace — `data = [4, 8, 6, 1, 9]`, `window = 3`

| i | v | Deque after pop-back | After push | After expiry | Output |
| --- | --- | --- | --- | --- | --- |
| 0 | 4 | `[]` | `[0]` | `[0]` | `nan` (count 1 < 3) |
| 1 | 8 | `[0]` (4 < 8, kept) | `[0,1]` | `[0,1]` | `nan` (count 2 < 3) |
| 2 | 6 | `[0]` (8 ≥ 6, popped) | `[0,2]` | `[0,2]` | `data[0] = 4` |
| 3 | 1 | `[]` (6 and 4 both ≥ 1) | `[3]` | `[3]` | `data[3] = 1` |
| 4 | 9 | `[3]` (1 < 9, kept) | `[3,4]` | `[3,4]` | `data[3] = 1` |

Result: `[nan, nan, 4, 1, 1]`

---

## 3. Possible Error Points

### Algorithmic / logic errors

| Error | Consequence | Guard in this code |
| --- | --- | --- |
| Using `>` instead of `>=` in step 1 | Duplicate equal values linger in the deque. The result stays correct but memory grows unnecessarily; on a column of all-identical values the deque degenerates to full length. | `>=` is used deliberately |
| Expiry test written as `dq.front() < i - window` | **Off-by-one.** Keeps an index one position too long, so the window silently spans *w+1* elements. | `<=` is used |
| Doing step 3 before step 2 | On the very first element the deque is empty and `dq.front()` is read — undefined behaviour. | Push always precedes the expiry check, so the deque is guaranteed non-empty |
| Storing *values* in the deque instead of *indices* | Expiry becomes impossible — a raw value carries no information about when it entered. | Indices are stored; values are looked up via `data[...]` |
| Calling `dq.front()` / `dq.back()` on an empty deque | Undefined behaviour, not an exception. Typically garbage output or a crash. | Step 1 tests `!dq.empty()`; step 3 is safe because step 2 just pushed |

### Input and edge cases

- **Empty input** — `n == 0`, the loop body never executes, and an empty vector is returned. Correct by construction, but worth confirming: any implementation that reads `data[0]` before the loop would crash here.
- **`window` larger than the data** — no position ever reaches `min_periods`, so the entire output is `NaN`. This is correct behaviour, not a bug; Pandas does the same.
- **`window == 1`** — each element is its own minimum; the output equals the input.
- **`window <= 0`** — rejected by the constructor with `invalid_argument`. Left unchecked, `i - window` would grow rather than shrink and the expiry loop would never terminate correctly.
- **`min_periods > window`** — rejected. It is unsatisfiable: `count` is capped at `window`, so no value could ever be emitted and the output would be silently all-`NaN`.

### C++-specific pitfalls

- **Comparing against `NaN`** — `NaN == NaN` is `false`. Testing output with `==` will always report a mismatch. The portable test for NaN is `v != v` (used in `print_vector`), or `std::isnan(v)`.
- **`NaN` in the *input*** — this implementation does not filter input `NaN`s. Because all comparisons with `NaN` return `false`, a `NaN` entering the deque is never popped by step 1 and can propagate into the output. Pandas skips null values by default. If the data may contain `NaN`, add an explicit `isnan(v)` branch.
- **Signed/unsigned mismatch** — `data.size()` returns `size_t` (unsigned). Comparing it directly against a signed `int` triggers a `-Wsign-compare` warning, and `i - window` computed in unsigned arithmetic **wraps to a huge positive number** instead of going negative, breaking the expiry test. The code casts to `int` once (`static_cast<int>(data.size())`) and keeps all index arithmetic signed.
- **Missing virtual destructor** — deleting through a base pointer without it is undefined behaviour.
- **Forgetting `#include <deque>`** — compiles on some standard libraries through transitive includes and fails on others. Include what you use.

### Build and environment errors

- **Compiling without `-std=c++11`** — older GCC defaults can reject the code. Use the documented build command.
- **An earlier version of this program used `<optional>`**, which requires **C++17 and GCC 7+**. This file deliberately avoids it, using a `-1` sentinel for `min_periods` instead, so it builds on C++11 and older toolchains.
- **IDE red squiggles that the compiler does not report** — VS Code's IntelliSense keeps its own `cppStandard` setting in `.vscode/c_cpp_properties.json`, separate from the build command. Both must be set.

---

## 4. Build and Run

```bash
g++ -std=c++11 -Wall -Wextra -O2 -o rolling_min rolling_min.cpp
./rolling_min
```

Compiles warning-free under `-Wall -Wextra`.

### Expected output

```
data:                     [4.00, 8.00, 6.00, 1.00, 9.00, 3.00, 7.00, 2.00, 5.00, 10.00]
rolling_min(window=2):    [nan, 4.00, 6.00, 1.00, 1.00, 3.00, 3.00, 2.00, 2.00, 5.00]
rolling_min(window=3):    [nan, nan, 4.00, 1.00, 1.00, 1.00, 3.00, 2.00, 2.00, 2.00]
rolling_min(window=5):    [nan, nan, nan, nan, 1.00, 1.00, 1.00, 1.00, 2.00, 2.00]
w=3, min_periods=1:       [4.00, 4.00, 4.00, 1.00, 1.00, 1.00, 3.00, 2.00, 2.00, 2.00]
```

## 5. Verification

All results above were cross-checked against the reference implementation in Pandas:

```python
import pandas as pd
s = pd.Series([4, 8, 6, 1, 9, 3, 7, 2, 5, 10])
s.rolling(3).min().tolist()
# [nan, nan, 4.0, 1.0, 1.0, 1.0, 3.0, 2.0, 2.0, 2.0]
```

Output matches for window sizes 2, 3, and 5, and for `min_periods = 1`.