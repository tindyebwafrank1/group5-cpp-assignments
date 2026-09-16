// rolling_min.cpp
// ==========================================================
// Group 10 -- Rolling Operations
// Operation implemented: rolling_min()
//
// Re-implementation, from first principles, of the rolling
// minimum operation provided by dataframe engines such as
// Polars and Pandas.
//
// Reference behaviour:
//   https://docs.pola.rs/api/python/stable/reference/dataframe/index.html
//
// Build:  g++ -std=c++11 -Wall -Wextra -O2 -o rolling_min rolling_min.cpp
// Run:    ./rolling_min
// ==========================================================

#include <algorithm>
#include <cmath>
#include <deque>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// ----------------------------------------------------------
// Abstract base class: RollingOperation
// ----------------------------------------------------------
// Defines the common interface and shared state for every
// rolling-window operation. Each concrete operation only has
// to say HOW a single window is reduced to one value; the
// looping, validation and NaN-padding are handled here once.
class RollingOperation {
protected:
    int window;       // number of observations in the trailing window
    int min_periods;  // minimum observations required to emit a value

public:
    // `min_periods < 0` is a sentinel meaning "same as window",
    // which is the Pandas/Polars default.
    RollingOperation(int window_size, int min_obs = -1)
        : window(window_size),
          min_periods(min_obs < 0 ? window_size : min_obs) {
        validate();
    }

    // Virtual destructor: required so that deleting a derived
    // object through a base-class pointer is well defined.
    virtual ~RollingOperation() {}

    // Pure virtual -> makes this class abstract. Every derived
    // class must supply its own windowed computation.
    virtual vector<double> apply(const vector<double>& data) const = 0;

    // Pure virtual: each operation reports its own name.
    virtual string name() const = 0;

    int get_window() const { return window; }
    int get_min_periods() const { return min_periods; }

protected:
    void validate() const {
        if (window <= 0) {
            throw invalid_argument("window must be a positive integer");
        }
        if (min_periods <= 0) {
            throw invalid_argument("min_periods must be a positive integer");
        }
        if (min_periods > window) {
            throw invalid_argument("min_periods cannot exceed window");
        }
    }
};

// ----------------------------------------------------------
// Concrete class: RollingMin
// ----------------------------------------------------------
// Computes the minimum value within each trailing window.
//
// ALGORITHM -- monotonic deque (sliding-window minimum)
// -----------------------------------------------------
// The naive approach re-scans all `window` elements at every
// position, costing O(n * window). Instead we keep a deque of
// INDICES whose corresponding values are strictly increasing:
//
//   1. Before pushing index i, pop from the BACK every index
//      whose value is >= data[i]. Those elements are both
//      older AND larger than data[i], so they can never be
//      the minimum of any future window -- data[i] outlives
//      them and beats them. They are permanently useless.
//
//   2. Push index i onto the back.
//
//   3. Pop from the FRONT any index that has slid out of the
//      window (index <= i - window).
//
//   4. The front of the deque now holds the index of the
//      smallest value in the current window.
//
// Each index is pushed once and popped at most once, so the
// total work across the whole pass is O(n) -- amortised O(1)
// per element -- using O(window) extra memory.
class RollingMin : public RollingOperation {
public:
    RollingMin(int window_size, int min_obs = -1)
        : RollingOperation(window_size, min_obs) {}

    string name() const { return "rolling_min"; }

    vector<double> apply(const vector<double>& data) const {
        const int n = static_cast<int>(data.size());

        // Positions without enough observations yet are NaN,
        // matching the Pandas/Polars convention.
        vector<double> result(n, nan(""));

        deque<int> dq;  // indices; data[dq] is strictly increasing

        for (int i = 0; i < n; ++i) {
            const double v = data[i];

            // Step 1: discard dominated candidates at the back.
            while (!dq.empty() && data[dq.back()] >= v) {
                dq.pop_back();
            }

            // Step 2: the new element is always a candidate.
            dq.push_back(i);

            // Step 3: evict the front if it left the window.
            while (dq.front() <= i - window) {
                dq.pop_front();
            }

            // Step 4: emit only once enough data has been seen.
            const int count = min(i + 1, window);
            if (count >= min_periods) {
                result[i] = data[dq.front()];
            }
        }
        return result;
    }
};

// ----------------------------------------------------------
// Helper: pretty-print a vector, showing NaN as "nan"
// ----------------------------------------------------------
static void print_vector(const string& label, const vector<double>& v) {
    cout << left << setw(26) << label << "[";
    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] != v[i]) {          // NaN != NaN is the portable test
            cout << "nan";
        } else {
            cout << fixed << setprecision(2) << v[i];
        }
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]" << endl;
}

// ----------------------------------------------------------
// Demonstration
// ----------------------------------------------------------
int main() {
    vector<double> sample;
    sample.push_back(4);  sample.push_back(8);  sample.push_back(6);
    sample.push_back(1);  sample.push_back(9);  sample.push_back(3);
    sample.push_back(7);  sample.push_back(2);  sample.push_back(5);
    sample.push_back(10);

    print_vector("data:", sample);

    // --- Normal usage, several window sizes ---
    RollingMin rm2(2);
    RollingMin rm3(3);
    RollingMin rm5(5);
    print_vector("rolling_min(window=2):", rm2.apply(sample));
    print_vector("rolling_min(window=3):", rm3.apply(sample));
    print_vector("rolling_min(window=5):", rm5.apply(sample));

    // --- min_periods: emit partial windows early ---
    RollingMin rm_partial(3, 1);
    print_vector("w=3, min_periods=1:", rm_partial.apply(sample));

    // --- Polymorphism: used through a base-class pointer ---
    RollingOperation* op = &rm3;
    cout << "\nVia base pointer -> " << op->name()
         << " (window=" << op->get_window()
         << ", min_periods=" << op->get_min_periods() << ")" << endl;
    print_vector("polymorphic call:", op->apply(sample));

    // --- Edge case: empty input ---
    vector<double> empty_data;
    print_vector("\nempty input:", rm3.apply(empty_data));

    // --- Edge case: window larger than the data ---
    vector<double> tiny;
    tiny.push_back(5); tiny.push_back(2);
    RollingMin rm_big(4);
    print_vector("window > size:", rm_big.apply(tiny));

    // --- Error handling: invalid window ---
    try {
        RollingMin bad(0);
        cout << "ERROR: expected an exception" << endl;
    } catch (const invalid_argument& e) {
        cout << "\nCaught as expected: " << e.what() << endl;
    }

    return 0;
}