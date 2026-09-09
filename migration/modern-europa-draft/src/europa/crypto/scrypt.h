// Europa legacy Scrypt PoW compatibility for Bitcoin Core 31.1.
// Derived from Colin Percival / ArtForz / pooler implementation.
// UNTESTED / DO NOT RELEASE.
#ifndef EUROPA_CRYPTO_SCRYPT_H
#define EUROPA_CRYPTO_SCRYPT_H

#include <cstddef>
#include <cstdint>

namespace europa {

inline constexpr std::size_t SCRYPT_SCRATCHPAD_SIZE{131072 + 63};

void PBKDF2_SHA256(const uint8_t* passwd, std::size_t passwdlen,
                   const uint8_t* salt, std::size_t saltlen,
                   uint64_t iterations, uint8_t* out, std::size_t outlen);

void Scrypt1024_1_1_256(const unsigned char* input80, unsigned char* output32);

} // namespace europa

#endif // EUROPA_CRYPTO_SCRYPT_H
