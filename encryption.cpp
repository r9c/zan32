#include <string>
#include <random>
#include <malloc.h>  // for _aligned_malloc and _aligned_free


std::string generateBase32Secret() {
    const std::string base32Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    std::string secret;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, base32Chars.size() - 1);

    for (int i = 0; i < 16; ++i) {
        secret += base32Chars[dist(gen)];
    }

    return secret;
}
