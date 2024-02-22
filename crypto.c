//
// Created by birdiecode on 17.02.2024.
//

#include "crypto.h"
#include <windows.h>
#include <stdio.h>

typedef SECStatus (*func1)(char *profile); // NSS_InitReadWrite
typedef PK11SlotInfo (*func2)(); // PK11_GetInternalKeySlot
typedef void (*func3)(PK11SlotInfo *slot); // PK11_FreeSlot
typedef PRBool (*func4)(PK11SlotInfo *slot); // PK11_NeedUserInit
typedef SECStatus (*func5)(PK11SlotInfo *slot, const char *ssopw,
                           const char *pk11_userpwd); // PK11_InitPin
typedef SECStatus (*func6)(SECItem *keyid, SECItem *data,
                           SECItem *result, void *cx); // PK11SDR_Encrypt
typedef char* (*func7)(SECItem *keyid, SECItem *data, int type,
                       SECItem *input); // NSSBase64_EncodeItem
typedef SECStatus (*func8)(); // NSS_Shutdown

int CRPAPI_init(HINSTANCE hDll, char *profile){
    SECStatus rv;

    func1 NSS_InitReadWrite;
    func2 PK11_GetInternalKeySlot;
    func3 PK11_FreeSlot;
    func4 PK11_NeedUserInit;
    func5 PK11_InitPin;


    NSS_InitReadWrite = (func1)GetProcAddress(hDll, "NSS_InitReadWrite");
    if (NSS_InitReadWrite == NULL) return 2;

    PK11_GetInternalKeySlot = (func2)GetProcAddress(hDll, "PK11_GetInternalKeySlot");
    if (PK11_GetInternalKeySlot == NULL) return 3;

    PK11_FreeSlot = (func3)GetProcAddress(hDll, "PK11_FreeSlot");
    if (PK11_FreeSlot == NULL) return 4;

    PK11_NeedUserInit = (func4)GetProcAddress(hDll, "PK11_NeedUserInit");
    if (PK11_NeedUserInit == NULL) return 5;

    PK11_InitPin = (func5)GetProcAddress(hDll, "PK11_InitPin");
    if (PK11_InitPin == NULL) return 6;

    char *profile_path;
    sprintf(profile_path, "sql:%s", profile);
    rv = NSS_InitReadWrite(profile_path);
    if (rv != SECSuccess) {
        return 20;
    }
    PK11SlotInfo *slot = NULL;
    slot = PK11_GetInternalKeySlot();

    if (slot && PK11_NeedUserInit(slot)) {
        rv = PK11_InitPin(slot, NULL, "");
        if (rv != SECSuccess) {
            return 21;
        }
    }
    if (slot) {
        PK11_FreeSlot(slot);
    }
}

int CRPAPI_encrypt(HINSTANCE hDll, char *str, char **ret){
    SECStatus rv;

    func6 PK11SDR_Encrypt;
    func7 NSSBase64_EncodeItem;

    PK11SDR_Encrypt = (func6)GetProcAddress(hDll, "PK11SDR_Encrypt");
    if (PK11SDR_Encrypt == NULL) return 7;

    NSSBase64_EncodeItem = (func7)GetProcAddress(hDll, "NSSBase64_EncodeItem");
    if (NSSBase64_EncodeItem == NULL)
        NSSBase64_EncodeItem = (func7)GetProcAddress(hDll, "NSSBase64_EncodeItem_Util");
    if (NSSBase64_EncodeItem == NULL) return 8;

    SECItem keyid = { 0, 0, 0 };
    SECItem data;
    data.data = (unsigned char *)str;
    data.len = strlen(str);

    SECItem result = { 0, 0, 0 };
    secuPWData pwdata = { PW_NONE, 0 };

    rv = PK11SDR_Encrypt(&keyid, &data, &result, &pwdata);
    if (rv != SECSuccess) {
        return 22;
    }

    *ret = NSSBase64_EncodeItem(NULL, NULL, 0, &result);
    memmove(&(*ret)[64], &(*ret)[66], 6);
    return 0;
}

int CRPAPI_shutdown(HINSTANCE hDll) {
    func8 NSS_Shutdown;

    NSS_Shutdown = (func8)GetProcAddress(hDll, "NSS_Shutdown");
    if (NSS_Shutdown == NULL) return 9;

    return NSS_Shutdown();
}
