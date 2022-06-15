#include "utility.h"
#include <Windows.h>
#include <squak/compression/sha1.h>
#include <sstream>
#include <vector>
#include <wincrypt.h>

namespace compression {

std::string sha1(std::string message) {
  HCRYPTPROV p;
  HCRYPTHASH h;

  Assert(CryptAcquireContext(&p, NULL, NULL, PROV_RSA_FULL, 0), "failed");
  Assert(CryptCreateHash(p, CALG_SHA1, 0, 0, &h), "hash failed");
  Assert(CryptHashData(h, (BYTE*)message.c_str(), message.size(), 0),
         "hash failed");

  DWORD length, count;
  Assert(CryptGetHashParam(h, HP_HASHSIZE, (BYTE*)&length, &count, 0), "");

  std::vector<uint8_t> value{};
  value.resize(length);

  Assert(CryptGetHashParam(h, HP_HASHVAL, (BYTE*)value.data(), &length, 0), "");

  CryptDestroyHash(p);
  CryptReleaseContext(h, 0);

  std::stringstream stream{};
  char hex[3]{};
  for (size_t i = 0; i < value.size(); i++) {
    sprintf(hex, "%02x", value[i]);
    stream << hex;
  }

  return stream.str();
}
};