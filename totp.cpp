#include "totp.h"
#include <cryptlib.h>
#include <hmac.h>
#include <sha.h>
#include <base32.h>
#include <secblock.h>
#include <chrono>
#include <string>

std::string getTOTPCode(const std::string& secret) {
    using namespace CryptoPP;

    // Decode base32 secret
    std::string decoded;
    Base32Decoder decoder;
    decoder.Put((const byte*)secret.data(), secret.size());
    decoder.MessageEnd();

    word64 size = decoder.MaxRetrievable();
    if (size && size <= SIZE_MAX) {
        decoded.resize(size);
        decoder.Get((byte*)&decoded[0], size);
    }

    // Get current Unix time
    uint64_t currentTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    uint64_t timeStep = currentTime / 30;

    // Convert time step to byte array (big endian)
    byte timeBytes[8];
    for (int i = 7; i >= 0; --i) {
        timeBytes[i] = timeStep & 0xFF;
        timeStep >>= 8;
    }

    // Generate HMAC-SHA1
    byte hash[SHA1::DIGESTSIZE];
    HMAC<SHA1> hmac((const byte*)decoded.data(), decoded.size());
    hmac.CalculateDigest(hash, timeBytes, 8);

    // Dynamic truncation
    int offset = hash[SHA1::DIGESTSIZE - 1] & 0x0F;
    uint32_t binary = ((hash[offset] & 0x7F) << 24) |
                      ((hash[offset + 1] & 0xFF) << 16) |
                      ((hash[offset + 2] & 0xFF) << 8) |
                      (hash[offset + 3] & 0xFF);

    // Return 6-digit code
    uint32_t otp = binary % 1000000;
    char code[7];
    snprintf(code, sizeof(code), "%06u", otp);
    return std::string(code);
}

int secondsUntilNextCode() {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto now_s = duration_cast<seconds>(now.time_since_epoch()).count();
    return 30 - (now_s % 30);
}

