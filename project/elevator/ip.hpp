#include <stdint.h>
#include <string>

typedef uint32_t enc_ip;

enc_ip encode_ip(std::string ip);
std::string decode_ip(enc_ip);
