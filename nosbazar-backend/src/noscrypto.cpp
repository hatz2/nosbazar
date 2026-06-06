#include "noscrypto.h"

namespace
{
	inline bool get_mask_part(uint8_t ch, const std::array<uint8_t, 16>& charset)
	{
		if (ch == 0) {
			return false;
		}

		return std::find(charset.begin(), charset.end(), ch) != charset.end();
	}

	inline std::vector<bool> get_mask(const std::vector<uint8_t>& packet, const std::array<uint8_t, 16>& charset)
	{
		std::vector<bool> output;

		for (uint8_t ch : packet) {
			if (ch == 0x00) {
				break;
			}

			output.push_back(get_mask_part(ch, charset));
		}

		return output;
	}

	inline size_t calc_len_of_mask(
		size_t start, const std::vector<bool>& mask, bool value)
	{
		size_t currentLen = 0;

		for (size_t i = start; i < mask.size(); ++i) {
			if (mask[i] == value) {
				++currentLen;
			}
			else {
				break;
			}
		}

		return currentLen;
	}

	inline std::vector<uint8_t> pack(const std::vector<uint8_t>& packet, const std::array<uint8_t, 16>& chars_to_pack)
	{
		std::vector<uint8_t> output;

		auto mask = get_mask(packet, chars_to_pack);

		size_t pos = 0;

		while (pos < mask.size()) {
			// Non packed chunk
			size_t current_chunk_len = calc_len_of_mask(pos, mask, false);

			for (size_t i = 0; i < current_chunk_len; ++i) {
				if (pos >= mask.size()) {
					break;
				}

				if ((i % 0x7E) == 0) {
					output.push_back(static_cast<uint8_t>(std::min<size_t>(current_chunk_len - i, 0x7E)));
				}

				output.push_back(packet[pos] ^ 0xFF);
				++pos;
			}

			// Packed chunk
			current_chunk_len = calc_len_of_mask(pos, mask, true);

			for (size_t i = 0; i < current_chunk_len; ++i) {
				if (pos >= mask.size()) {
					break;
				}

				if ((i % 0x7E) == 0) {
					output.push_back(static_cast<uint8_t>(std::min<size_t>(current_chunk_len - i, 0x7E) | 0x80));
				}

				auto it = std::find(chars_to_pack.begin(), chars_to_pack.end(), packet[pos]);

				uint8_t currentValue = static_cast<uint8_t>(std::distance(chars_to_pack.begin(), it));

				if ((i % 2) == 0) {
					output.push_back(static_cast<uint8_t>(currentValue << 4));
				}
				else {
					output.back() |= currentValue;
				}

				++pos;
			}
		}

		output.push_back(0xFF);

		return output;
	}

	std::vector<uint8_t> unpack(const std::vector<uint8_t>& packet, const std::array<uint8_t, 16>& chars_to_unpack)
	{
		std::vector<uint8_t> output;

		size_t pos = 0;

		while (pos < packet.size()) {
			if (packet[pos] == 0xFF) {
				break;
			}

			uint8_t current_chunk_len = packet[pos] & 0x7F;

			bool is_packed = (packet[pos] & 0x80) != 0;

			++pos;

			if (is_packed) {
				size_t iterations = static_cast<size_t>(std::ceil(current_chunk_len / 2.0));

				for (size_t i = 0; i < iterations; ++i) {
					if (pos >= packet.size()) {
						break;
					}

					uint8_t two_chars = packet[pos++];

					uint8_t left_char = two_chars >> 4;

					output.push_back(chars_to_unpack[left_char]);

					uint8_t rightChar = two_chars & 0x0F;

					if (rightChar == 0) {
						break;
					}

					output.push_back(chars_to_unpack[rightChar]);
				}
			}
			else {
				for (size_t i = 0; i < current_chunk_len; ++i) {
					if (pos >= packet.size()) {
						break;
					}

					output.push_back(packet[pos] ^ 0xFF);

					++pos;
				}
			}
		}

		return output;
	}
}

std::vector<uint8_t> nosbazar::noscrypto::Client::login_encrypt(std::vector<uint8_t> packet)
{
	std::vector<uint8_t> output;

	if (packet.empty() || packet.back() != 0xA) {
		packet.push_back(0xA);
	}

	for (uint8_t b : packet) {
		uint8_t v = static_cast<uint8_t>(((b ^ 0xC3) + 0x0F) & 0xFF);
		output.push_back(v);
	}

	return output;
}

std::vector<uint8_t> nosbazar::noscrypto::Client::login_decrypt(const std::vector<uint8_t>& packet)
{
	std::vector<uint8_t> output;

	for (uint8_t b : packet) {
		uint8_t v = static_cast<uint8_t>((b - 0x0F) & 0xFF);
		output.push_back(v);
	}

	return output;
}

std::vector<uint8_t> nosbazar::noscrypto::Client::world_encrypt(const std::vector<uint8_t>& packet)
{
	return pack(packet, encryption_table);
}

std::vector<uint8_t> nosbazar::noscrypto::Client::world_decrypt(const std::vector<uint8_t>& packet, uint32_t session, bool is_first_packet)
{
	std::vector<uint8_t> xored = world_xor(packet, session, is_first_packet);
	return unpack(xored);
}

std::vector<uint8_t> nosbazar::noscrypto::Client::world_xor(const std::vector<uint8_t>& packet, uint32_t session, bool is_first_packet)
{
	std::vector<uint8_t> output;
	int stype = is_first_packet ? -1 : ((session >> 6) & 3);
	uint8_t key = static_cast<uint8_t>(session & 0xFF);

	for (uint8_t i : packet) {
		uint8_t value = 0;

		switch (stype) {
		case 0:
			value = static_cast<uint8_t>((i - key - 0x40) & 0xFF);
			break;

		case 1:
			value = static_cast<uint8_t>((i + key + 0x40) & 0xFF);
			break;

		case 2:
			value = static_cast<uint8_t>(((i - key - 0x40) ^ 0xC3) & 0xFF);
			break;

		case 3:
			value = static_cast<uint8_t>(((i + key + 0x40) ^ 0xC3) & 0xFF);
			break;

		default:
			value = static_cast<uint8_t>((i - 0x0F) & 0xFF);
			break;
		}

		output.push_back(value);
	}

	return output;
}

std::vector<uint8_t> nosbazar::noscrypto::Client::unpack(const std::vector<uint8_t>& packet)
{
	return ::unpack(packet, decryption_table);
}

std::vector<uint8_t> nosbazar::noscrypto::Server::login_encrypt(std::vector<uint8_t> packet)
{
	std::vector<uint8_t> output;

	if (packet.empty() || packet.back() != 0x0A) {
		packet.push_back('\n');
	}

	for (uint8_t b : packet) {
		uint8_t v = static_cast<uint8_t>(((b ^ 0xC3) + 0x0F) & 0xFF);
		output.push_back(v);
	}

	return output;
}

std::vector<uint8_t> nosbazar::noscrypto::Server::login_decrypt(const std::vector<uint8_t>& packet)
{
	std::vector<uint8_t> output;

	for (uint8_t b : packet) {
		uint8_t v = static_cast<uint8_t>((b - 0x0F) & 0xFF);
		output.push_back(v);
	}

	return output;
}

std::vector<uint8_t> nosbazar::noscrypto::Server::world_encrypt(const std::vector<uint8_t>& packet, uint32_t session, bool is_first_packet)
{
	std::vector<uint8_t> packed = pack(packet, encryption_table);
	return world_xor(packed, session, is_first_packet);
}

std::vector<uint8_t> nosbazar::noscrypto::Server::world_decrypt(const std::vector<uint8_t>& packet)
{
	return unpack(packet, decryption_table);
}

std::vector<uint8_t> nosbazar::noscrypto::Server::world_xor(const std::vector<uint8_t>& packet, uint32_t session, bool is_first_packet)
{
	std::vector<uint8_t> output;
	int stype = is_first_packet ? -1 : ((session >> 6) & 3);
	uint8_t key = static_cast<uint8_t>(session & 0xFF);

	for (uint8_t i : packet) {
		uint8_t value = 0;

		switch (stype) {
		case 0:
			value = static_cast<uint8_t>((i + key + 0x40) & 0xFF);
			break;

		case 1:
			value = static_cast<uint8_t>((i - key - 0x40) & 0xFF);
			break;

		case 2:
			value = static_cast<uint8_t>((((i ^ 0xC3) + key + 0x40)) & 0xFF);
			break;

		case 3:
			value = static_cast<uint8_t>((((i ^ 0xC3) - key - 0x40)) & 0xFF);
			break;

		default:
			value = static_cast<uint8_t>((i + 0x0F) & 0xFF);
			break;
		}

		output.push_back(value);
	}

	return output;
}
