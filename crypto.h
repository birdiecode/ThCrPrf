//
// Created by birdiecode on 17.02.2024.
//

#ifndef THCRPRF_CRYPTO_H
#define THCRPRF_CRYPTO_H
#include <windows.h>

typedef enum {
    SECWouldBlock = -2,
    SECFailure = -1,
    SECSuccess = 0
} SECStatus;

typedef void* PK11SlotInfo;

typedef enum {
    PR_FALSE = 0,
    PR_TRUE = 1
} PRBool;

typedef enum {
    siBuffer = 0,
    siClearDataBuffer = 1,
    siCipherDataBuffer = 2,
    siDERCertBuffer = 3,
    siEncodedCertBuffer = 4,
    siDERNameBuffer = 5,
    siEncodedNameBuffer = 6,
    siAsciiNameString = 7,
    siAsciiString = 8,
    siDEROID = 9,
    siUnsignedInteger = 10,
    siUTCTime = 11,
    siGeneralizedTime = 12,
    siVisibleString = 13,
    siUTF8String = 14,
    siBMPString = 15
} SECItemType;

typedef struct SECItemStr SECItem;

struct SECItemStr {
    SECItemType type;
    unsigned char *data;
    unsigned int len;
};

typedef struct {
    enum {
        PW_NONE = 0,
        PW_FROMFILE = 1,
        PW_PLAINTEXT = 2,
        PW_EXTERNAL = 3
    } source;
    char *data;
} secuPWData;

int CRPAPI_init(HINSTANCE hDll, char *profile);
int CRPAPI_encrypt(HINSTANCE hDll, char *str, char **ret);
int CRPAPI_shutdown(HINSTANCE hDll);
#endif //THCRPRF_CRYPTO_H
