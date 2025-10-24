#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <chrono>
#include <openssl/sha.h> // For SHA256
using namespace std;
using namespace std::chrono;

// =======================
// 2.2 Convert text to bits
// =======================
vector<int> text_to_bits(const string& input) {
    vector<int> bits;
    for (unsigned char c : input) {
        for (int i = 7; i >= 0; --i)
            bits.push_back((c >> i) & 1);
    }
    return bits;
}

// =======================
// 2.1 Initialize state from bits
// =======================
vector<int> init_state(const vector<int>& input_bits, size_t width = 512) {
    vector<int> state(width, 0);
    for (size_t i = 0; i < input_bits.size() && i < width; ++i)
        state[i] = input_bits[i];
    return state;
}

// =======================
// Apply CA rule (30, 90, 110)
// =======================
vector<int> evolve(const vector<int>& current_state, int rule) {
    int n = current_state.size();
    vector<int> next_state(n, 0);
    for (int i = 0; i < n; ++i) {
        int left   = (i == 0)     ? 0 : current_state[i - 1];
        int center = current_state[i];
        int right  = (i == n - 1) ? 0 : current_state[i + 1];
        int index = (left << 2) | (center << 1) | right;
        next_state[i] = (rule >> index) & 1;
    }
    return next_state;
}

// =======================
// Run CA for several steps
// =======================
vector<int> run_ca(const vector<int>& initial_state, int rule, size_t steps) {
    vector<int> state = initial_state;
    vector<int> all_bits;
    for (size_t t = 0; t < steps; ++t) {
        state = evolve(state, rule);
        all_bits.insert(all_bits.end(), state.begin(), state.end());
    }
    return all_bits;
}

// =======================
// 2.3 Compress result to 256 bits
// =======================
vector<int> compress_to_256(const vector<int>& bits) {
    vector<int> hash_bits(256, 0);
    for (size_t i = 0; i < bits.size(); ++i)
        hash_bits[i % 256] ^= bits[i];
    return hash_bits;
}

// =======================
// Convert bits to hex string
// =======================
string bits_to_hex(const vector<int>& bits) {
    stringstream ss;
    for (size_t i = 0; i < bits.size(); i += 4) {
        int value = (bits[i] << 3) | (bits[i+1] << 2) | (bits[i+2] << 1) | bits[i+3];
        ss << hex << value;
    }
    return ss.str();
}

// =======================
// 2.1 ac_hash function
// =======================
string ac_hash(const string& input, uint32_t rule, size_t steps) {
    vector<int> bits = text_to_bits(input);
    vector<int> state = init_state(bits);
    vector<int> all_bits = run_ca(state, rule, steps);
    vector<int> hash_bits = compress_to_256(all_bits);
    return bits_to_hex(hash_bits);
}

// =======================
// SHA256 hash function
// =======================
string sha256_hash(const string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)input.c_str(), input.size(), hash);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    return ss.str();
}

// =======================
// 3. Blockchain: choose hash mode
// =======================
string compute_block_hash(const string& data, bool use_ac_hash, uint32_t rule, size_t steps) {
    if (use_ac_hash)
        return ac_hash(data, rule, steps);
    else
        return sha256_hash(data);
}




// =======================
// Helper: convert hex string to bits
// =======================
vector<int> hex_to_bits(const string& hex) {
    vector<int> bits;
    for (char c : hex) {
        int value = (c >= '0' && c <= '9') ? (c - '0') : (tolower(c) - 'a' + 10);
        for (int i = 3; i >= 0; --i)
            bits.push_back((value >> i) & 1);
    }
    return bits;
}

// =======================
// 5. Avalanche effect test
// =======================
void avalanche_test(uint32_t rule, size_t steps) {
    string input = "AvalancheTest"; // base input
    int trials = 10;
    double total_diff = 0;

    for (int t = 0; t < trials; ++t) {
        // Flip one bit in the input
        string modified = input;
        modified[t % input.size()] ^= 1; // flip one character bit

        string hash1 = ac_hash(input, rule, steps);
        string hash2 = ac_hash(modified, rule, steps);

        vector<int> bits1 = hex_to_bits(hash1);
        vector<int> bits2 = hex_to_bits(hash2);

        int diff = 0;
        for (size_t i = 0; i < bits1.size(); ++i)
            if (bits1[i] != bits2[i])
                diff++;

        double percent = (diff * 100.0) / bits1.size();
        total_diff += percent;
        cout << "Trial " << t+1 << ": " << percent << "% bits changed\n";
    }

    cout << "Average avalanche effect: " << (total_diff / trials) << "% of bits changed\n";
}



// =======================
// 4. Compare ac_hash vs SHA256
// =======================
int main() {
    string base_data = "Block #";
    uint32_t rule = 90;
    size_t steps = 20;
    int difficulty = 2; // number of leading zeros

    vector<double> times_ac, times_sha;
    vector<int> iter_ac, iter_sha;

    for (int i = 1; i <= 10; ++i) {
        string data = base_data + to_string(i) + ": test transaction";

        // --- AC_HASH mining ---
        auto start = high_resolution_clock::now();
        int ac_attempts = 0;
        string hash;
        do {
            hash = ac_hash(data + to_string(ac_attempts), rule, steps);
            ac_attempts++;
        } while (hash.substr(0, difficulty) != string(difficulty, '0'));
        auto end = high_resolution_clock::now();
        times_ac.push_back(duration_cast<microseconds>(end - start).count() / 1e6);
        iter_ac.push_back(ac_attempts);

        // --- SHA256 mining ---
        start = high_resolution_clock::now();
        int sha_attempts = 0;
        do {
            hash = sha256_hash(data + to_string(sha_attempts));
            sha_attempts++;
        } while (hash.substr(0, difficulty) != string(difficulty, '0'));
        end = high_resolution_clock::now();
        times_sha.push_back(duration_cast<microseconds>(end - start).count() / 1e6);
        iter_sha.push_back(sha_attempts);
    }

    // Compute averages
    double avg_time_ac = 0, avg_time_sha = 0;
    double avg_iter_ac = 0, avg_iter_sha = 0;
    for (int i = 0; i < 10; ++i) {
        avg_time_ac += times_ac[i]; avg_time_sha += times_sha[i];
        avg_iter_ac += iter_ac[i]; avg_iter_sha += iter_sha[i];
    }
    avg_time_ac /= 10; avg_time_sha /= 10;
    avg_iter_ac /= 10; avg_iter_sha /= 10;

    // ---  table output ---
    cout << "\nComparison of AC_HASH vs SHA256 (difficulty = " << difficulty << ")\n";
    cout << "Method\t\tAvg Time (s)\tAvg Iterations\n";
    cout << "AC_HASH\t\t" << avg_time_ac << "\t\t" << avg_iter_ac << "\n";
    cout << "SHA256\t\t" << avg_time_sha << "\t\t" << avg_iter_sha << "\n";

    // Run avalanche test exercice 5
    cout << "\n=== Avalanche Effect Test ===\n";
    avalanche_test(rule, steps);


    return 0;
}
