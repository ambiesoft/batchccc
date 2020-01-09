#include <memory>
#include <string>

#include <QByteArray>

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
