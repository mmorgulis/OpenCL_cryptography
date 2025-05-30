#ifndef GCM_AES_H
#define GCM_AES_H

#include "aes_mode.h"
#include "safe_allocator.hpp"
#include <cstdint>
#include <string>
#include <array>
#include <span>

class AES_GCM : public AES_MODE
{
private:
	crypto::safe_vector<std::array<uint8_t, 16>> _blocks; // 16 byte blocks
	crypto::safe_vector<uint8_t> _last_block;
	crypto::safe_vector<uint8_t> _key;
	crypto::safe_vector<uint8_t> _round_keys;
	std::array<uint8_t, 12> _iv; // more efficient dimension for iv
	std::array<uint8_t, 16> _tag;
	crypto::safe_vector<uint8_t> _aad;

	// Tables (byte level) for mult in GF(2^218)
	// 16 tables, with 256 values, each values of 16 bytes
	std::array<std::array<std::array<uint8_t, 16>, 256>, 16> m_table; // 64 KiB
	std::array<uint8_t, 16> m_H;

public:
	AES_GCM();
	~AES_GCM() = default;
	// Remove the possibility to copy the object mode
	AES_GCM& operator=(const AES_GCM& g) = delete;
	AES_GCM(const AES_GCM& g) = delete;
	
	// Functions to set attributes
	void set_key(std::span<const uint8_t> key);
	void set_iv(std::span<const uint8_t> iv);
	void set_aad(std::span<const uint8_t> aad);
	void set_tag(std::span<const uint8_t> tag);
	std::span<const uint8_t> get_tag() const;

	bool control_attributes_encrypt();
	bool control_attributes_decrypt();

	std::string encrypt(std::string_view plain_text) override;
	std::string decrypt(std::string_view cipher_text) override;
	void encrypt(std::span<const uint8_t> plain_text, std::span<uint8_t> cipher_text);
	void decrypt(std::span<const uint8_t> cipher_text, std::span<uint8_t> plain_text);

	/**
	* @brief function that divides the input in 16 byte blocks with padding at the end
	* 
	* @param plain_text is a string view because the caller will pass a text
	* in different form (string, char *, string_view)	
	*/
	void elaborate_plain_text(std::span<const uint8_t> plain_text);

	void precompute_table(std::span<const uint8_t> counter);

	void clear();
	

};


#endif // GCM_AES_H