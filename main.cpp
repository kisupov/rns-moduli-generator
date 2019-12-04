#include <iostream>
#include <iomanip>
#include <vector>
#include <mpfr.h>

/*
 * Standard C function: Greatest Common Divisor
 */
long gcd(long a, long b) {
    long c;
    while (a != 0) {
        c = a;
        a = b % a;
        b = c;
    }
    return b;
}

/*
 * Return x^p
 */
unsigned long iPow(unsigned long x, unsigned long p) {
    if (p == 0) return 1;
    if (p == 1) return x;
    return x * iPow(x, p - 1);
}

/*
 * Generates and prints a set of n odd coprime moduli, sorted in ascending order, starting from the least
 * significant modulus. The logarithm of the moduli product (i.e., the actual RNS dynamic range) is also printed.
 * @param n - required size of the moduli set
 * @param m0 - most significant modulus
 */
void moduliGenerator1(long n, long m0) {
    auto *moduli = new long[n];
    moduli[0] = m0;
    long temp = moduli[0] + 2;
    int count = 1;
    mpfr_t M;
    mpfr_t logM;
    mpfr_init2(M, 10000);
    mpfr_init2(logM, 10000);
    mpfr_set_si(M, moduli[0], MPFR_RNDN);
    mpfr_log2(logM, M, MPFR_RNDD);
    while (count < n) {
        bool coprime = true;
        for (int i = 0; i < count; i++) {
            if (gcd(moduli[i], temp) > 1) {
                coprime = false;
                break;
            }
        }
        if (coprime) {
            moduli[count] = temp;
            mpfr_mul_si(M, M, moduli[count], MPFR_RNDN);
            mpfr_log2(logM, M, MPFR_RNDD);
            count = count + 1;
        } else {
            temp = temp + 2;
        }
    }
    std::cout << "- Moduli set size: " << count << std::endl;
    std::cout << "- Dynamic range, log2(M): " << std::setprecision(10) << mpfr_get_d(logM, MPFR_RNDD) << std::endl;
    std::cout << "- Moduli set:" << std::endl;
    for (int j = 0; j < count; j++) {
        std::cout << moduli[j] << ",";
        if((j+1) % 8 == 0){
            std::cout << std::endl;
        }
    }
    mpfr_clear(M);
    mpfr_clear(logM);
    delete [] moduli;
}

/*
 * Generates and prints a set of odd coprime moduli, sorted in descending order, starting from the most significant
 * modulus. The generated moduli set satisfy: log2(M) >= p. For details, see http://dx.doi.org/10.1007/978-3-319-62932-2_18
 * @param p - required precision in bits
 * @param k - bit size of the most significant modulus (i.e., the most significant modulus will be 2^{k} - 1)
 */
void moduliGenerator2(unsigned long p, unsigned long k) {
    int i = 1;  //size of the moduli set
    std::vector<unsigned long> moduli(1);
    moduli[0] = iPow(2, k) - 1; // m_1 = 2^k - 1
    mpfr_t M;
    mpfr_t logM;
    mpfr_init2(M, 10000);
    mpfr_init2(logM, 10000);
    mpfr_set_ui(M, moduli[0], MPFR_RNDN);
    mpfr_log2(logM, M, MPFR_RNDD);
    unsigned long m = moduli[0] - 2; // initial value of the new modulus
    while (mpfr_get_d(logM, MPFR_RNDD) < (double) p) {
        bool coprime = true;
        for (int j = 0; j < i; j++) {
            if (gcd(m, moduli[j]) != 1) {
                coprime = false;
                break;
            }
        }
        if (coprime) {
            moduli.insert(moduli.end(), m);
            i++;
            m = moduli[i - 1] - 2;
            mpfr_mul_ui(M, M, moduli[i - 1], MPFR_RNDN);
            mpfr_log2(logM, M, MPFR_RNDD);
        } else {
            m = m - 2;
        }
        if (m <= 1) {
            i = -1;
            break;
        }
    }
    if (i == -1) {
        std::cout << "Error: invalid parameters." << std::endl;
    } else {
        std::cout << "- Moduli set size: " << i << std::endl;
        std::cout << "- Moduli bitlength: " << k << std::endl;
        std::cout << "- Given precision in bits: " << p << std::endl;
        std::cout << "- Actual precision (RNS dynamic range, log2(M)): " << std::setprecision(10) << mpfr_get_d(logM, MPFR_RNDD) << std::endl;
        std::cout << "- Moduli set:" << std::endl;
        for (int j = 0; j < i; j++) {
            std::cout << moduli[j] << ",";
            if((j+1) % 8 == 0){
                std::cout << std::endl;
            }
        }
    }
    mpfr_clear(M);
    mpfr_clear(logM);
}

int main() {
    moduliGenerator1(128, 96997537);
    //moduliGenerator2(1024, 31);
    return 0;
}