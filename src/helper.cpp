#include <memory>
#include <string>

#include <QByteArray>
#include <QDebug>

#include "unicode/utypes.h"
#include "unicode/ucsdet.h"
#include "unicode/ucnv.h"
#include "unicode/ustring.h"

#include "helper.h"

using namespace std;

string GetDetectedCodecICU(const QByteArray& array)
{
    UErrorCode status = U_ZERO_ERROR;
    unique_ptr<UCharsetDetector, void(*)(UCharsetDetector*)> csd(ucsdet_open(&status), ucsdet_close);
    if ( U_FAILURE(status) ) { // [2]
        qDebug() <<(u_errorName(status)) << __FUNCTION__;
        return string();
    }
    ucsdet_setText(csd.get(), array.data(), array.length(), &status);
    const UCharsetMatch *match = ucsdet_detect(csd.get(), &status);

    if (match == nullptr)
    {
        return string();
    }

    const char *name = ucsdet_getName(match, &status);
    //int32_t conf  = ucsdet_getConfidence(match, &status);

    return name;
}


unique_ptr<char[]> ConvertToUTF8(const string& encoding, const QByteArray& arrayIn, int32_t* buffsize)
{
    UErrorCode error = U_ZERO_ERROR;
    int32_t ret = ucnv_convert("utf-8",
                 encoding.c_str(),
                 nullptr,
                 0,
                 arrayIn.data(),
                 arrayIn.size(),
                 &error);
    if(error != U_BUFFER_OVERFLOW_ERROR)
        return nullptr;

    *buffsize = ret;
    unique_ptr<char[]> buff(new char[*buffsize]);
    error = U_ZERO_ERROR;
    ret = ucnv_convert("utf-8",
                 encoding.c_str(),
                 buff.get(),
                 *buffsize,
                 arrayIn.data(),
                 arrayIn.size(),
                 &error);
    if ( U_FAILURE(error) )
        return unique_ptr<char[]>();

    return buff;
}
char* ConvertToUTF8_obsolete(const string& encoding)
{
    UErrorCode error = U_ZERO_ERROR;
    unique_ptr<UConverter, void(*)(UConverter*)> conv(ucnv_open(encoding.c_str(), &error), ucnv_close);
    if ( U_FAILURE(error) ) { // [2]
        qDebug() << u_errorName(error) << __FUNCTION__;
        return nullptr;
    }

    const char  euc_jp[]     = "\xC6\xFC\xCB\xDC"; // "日本"のeuc-jp表現
    const char* source       = euc_jp; // 入力の先頭位置
    size_t      sourceLength = 4;      // 入力の長さ(バイト数)
    size_t      targetLength = (sourceLength*2) + 1;   // *2 for surrogate pair, +1 for last 0

    UChar* wbuffer = new UChar[targetLength]; // 出力領域の確保
    UChar* wtarget = wbuffer;

    // マルチバイト文字列(euc-jp) から Unicode へ変換
    ucnv_toUnicode(conv.get(),
                   &wtarget, wtarget + targetLength,
                   &source,  source  + sourceLength,
                   0, TRUE, &error);
    if ( U_FAILURE(error) ) {
        delete[] wbuffer;
        qDebug() << u_errorName(error);
        return nullptr;
    }


    // Unicode to UTF-8
    unique_ptr<UConverter, void(*)(UConverter*)> convUtoU8(ucnv_open("utf-8", &error), ucnv_close);
    if ( U_FAILURE(error) ) { // [2]
        qDebug() << u_errorName(error) << __FUNCTION__;
        return nullptr;
    }
    size_t maxSize1Utf8char = 4;
    size_t utf8bytesize = ((wtarget-wbuffer)*maxSize1Utf8char) + 1;

    char* p = new char[utf8bytesize];
    memset(p,0,utf8bytesize);
    ucnv_fromUnicode(convUtoU8.get(),
                     &p, p + utf8bytesize,
                     (const UChar**)&wbuffer, wtarget,
                     0, TRUE, &error);

    // *p = 0;
    return p;
}
