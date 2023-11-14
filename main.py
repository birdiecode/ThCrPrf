from NSSinterlayer import NSSinterlayer

LIB_PATH_NSS = "/usr/lib/x86_64-linux-gnu/libnss3.so"
LIB_PATH_SMIME3 = "/usr/lib/x86_64-linux-gnu/libsmime3.so"

with NSSinterlayer(LIB_PATH_NSS, LIB_PATH_SMIME3, "./key") as nss:
    data = nss.encrypt("test_text")
    print(data)
    print(nss.decrypt(data))
