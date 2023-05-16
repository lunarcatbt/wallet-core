//
//  TaprootAddress.hpp
//  WalletCore
//
//  Created by Eric Li on 2023/5/14.
//
#pragma once

#include "../PublicKey.h"
#include "Data.h"

#include <cstdint>
#include <string>
#include <tuple>

namespace TW::Bitcoin {

class TaprootAddress {
  public:
    /// Human-readable part.
    ///
    /// \see https://github.com/satoshilabs/slips/blob/master/slip-0173.md
    std::string hrp;

    /// Witness program version.
    byte witnessVersion;

    /// Witness program.
    Data witnessProgram;

    // Prefix for Bitcoin Testnet Segwit addresses
    static constexpr auto TestnetPrefix = "tb";

    /// Determines whether a string makes a valid Bech32 address.
    static bool isValid(const std::string& string);

    /// Determines whether a string makes a valid Bech32 address, and the HRP
    /// matches.
    static bool isValid(const std::string& string, const std::string& hrp);

    /// Initializes a Bech32m address with a human-readable part, a witness
    /// version, and a witness program.
    TaprootAddress(std::string hrp, byte witver, Data witprog)
        : hrp(std::move(hrp)), witnessVersion(witver), witnessProgram(std::move(witprog)) {}

    /// Initializes a taproot-version-0 Bech32 address with a public key and a HRP prefix.
    TaprootAddress(const PublicKey& publicKey, std::string hrp);

    /// Create a testnet address
    static TaprootAddress createTestnetFromPublicKey(const PublicKey& publicKey) { return TaprootAddress(publicKey, TestnetPrefix); }

    /// Decodes a SegWit address.
    ///
    /// \returns a tuple with the address, hrp, and a success flag.
    static std::tuple<TaprootAddress, std::string, bool> decode(const std::string& addr);

    /// Encodes the SegWit address.
    ///
    /// \returns encoded address string, or empty string on failure.
    std::string string() const;

    /// Initializes a Bech32 address with raw data.
    static std::pair<TaprootAddress, bool> fromRaw(const std::string& hrp, const Data& data);

    bool operator==(const TaprootAddress& rhs) const {
        return hrp == rhs.hrp && witnessVersion == rhs.witnessVersion &&
               witnessProgram == rhs.witnessProgram;
    }

  private:
    TaprootAddress() = default;
};

} // namespace TW::Bitcoin

/// Wrapper for C interface.
struct TWTaprootAddress {
    TW::Bitcoin::TaprootAddress impl;
};
