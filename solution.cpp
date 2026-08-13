#include <bits/stdc++.h>
using namespace std;

// Big integer addition to safely handle arbitrarily large A, B (in case they
// exceed 64-bit range), while still being correct for normal-sized inputs.

struct BigInt {
    bool neg = false;
    vector<int> digits; // base 10, little-endian

    static BigInt fromString(string s) {
        BigInt b;
        int i = 0;
        if (!s.empty() && (s[0] == '+' || s[0] == '-')) {
            b.neg = (s[0] == '-');
            i = 1;
        }
        string num = s.substr(i);
        // strip leading zeros (but keep at least one digit)
        int start = 0;
        while (start + 1 < (int)num.size() && num[start] == '0') start++;
        num = num.substr(start);
        for (int j = (int)num.size() - 1; j >= 0; j--) {
            b.digits.push_back(num[j] - '0');
        }
        if (b.digits.empty()) b.digits.push_back(0);
        if (isZero(b)) b.neg = false;
        return b;
    }

    static bool isZero(const BigInt &b) {
        for (int d : b.digits) if (d != 0) return false;
        return true;
    }
};

// compare magnitude of a and b: returns 1 if |a|>|b|, -1 if <, 0 if equal
int cmpAbs(const BigInt &a, const BigInt &b) {
    if (a.digits.size() != b.digits.size())
        return a.digits.size() > b.digits.size() ? 1 : -1;
    for (int i = (int)a.digits.size() - 1; i >= 0; i--) {
        if (a.digits[i] != b.digits[i])
            return a.digits[i] > b.digits[i] ? 1 : -1;
    }
    return 0;
}

vector<int> addAbs(const vector<int> &a, const vector<int> &b) {
    vector<int> res;
    int carry = 0;
    size_t n = max(a.size(), b.size());
    for (size_t i = 0; i < n || carry; i++) {
        int sum = carry;
        if (i < a.size()) sum += a[i];
        if (i < b.size()) sum += b[i];
        res.push_back(sum % 10);
        carry = sum / 10;
    }
    if (res.empty()) res.push_back(0);
    return res;
}

// subtract b from a, assuming |a| >= |b|
vector<int> subAbs(const vector<int> &a, const vector<int> &b) {
    vector<int> res;
    int borrow = 0;
    for (size_t i = 0; i < a.size(); i++) {
        int diff = a[i] - borrow - (i < b.size() ? b[i] : 0);
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res.push_back(diff);
    }
    while (res.size() > 1 && res.back() == 0) res.pop_back();
    return res;
}

BigInt add(const BigInt &a, const BigInt &b) {
    BigInt res;
    if (a.neg == b.neg) {
        res.neg = a.neg;
        res.digits = addAbs(a.digits, b.digits);
    } else {
        int c = cmpAbs(a, b);
        if (c == 0) {
            res.neg = false;
            res.digits = {0};
        } else if (c > 0) {
            res.neg = a.neg;
            res.digits = subAbs(a.digits, b.digits);
        } else {
            res.neg = b.neg;
            res.digits = subAbs(b.digits, a.digits);
        }
    }
    if (BigInt::isZero(res)) res.neg = false;
    return res;
}

string toString(const BigInt &b) {
    string s;
    if (b.neg && !BigInt::isZero(b)) s += '-';
    for (int i = (int)b.digits.size() - 1; i >= 0; i--) {
        s += char('0' + b.digits[i]);
    }
    return s;
}

int main() {
    string a, b;
    if (!(cin >> a >> b)) return 0;
    BigInt A = BigInt::fromString(a);
    BigInt B = BigInt::fromString(b);
    BigInt C = add(A, B);
    cout << toString(C) << "\n";
    return 0;
}
