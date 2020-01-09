#ifndef HELPER_H
#define HELPER_H

#include <string>
#include <memory>

class QByteArray;

std::string GetDetectedCodecICU(const QByteArray& array);
std::unique_ptr<char[]> ConvertToUTF8(const std::string& encoding, const QByteArray& arrayIn, int32_t* buffsize);
char* ConvertToUTF8_obsolete(const std::string& encoding);
#endif // HELPER_H
