import ctypes


class NSSinterlayer:
    class SECItem(ctypes.Structure):
        _fields_ = [("type", ctypes.c_uint), ("data", ctypes.c_char_p), ("len", ctypes.c_uint)]

    class SECuPWData(ctypes.Structure):
        _fields_ = [("source", ctypes.c_uint), ("data", ctypes.c_uint)]

    def __init__(self, path_lib, path_lib_smime3, path):
        self.lib_nss = ctypes.CDLL(path_lib)
        self.lib_smime3 = ctypes.CDLL(path_lib_smime3)

        ptr_seci = ctypes.POINTER(self.SECItem)
        prt_secupwdata = ctypes.POINTER(self.SECuPWData)

        # region NSS_InitReadWrite
        self.NSS_InitReadWrite = self.lib_nss.NSS_InitReadWrite
        self.NSS_InitReadWrite.argtypes = [ctypes.c_char_p]
        self.NSS_InitReadWrite.restype = ctypes.c_int
        # endregion
        # region NSS_Shutdown
        self.NSS_Shutdown = self.lib_nss.NSS_Shutdown
        self.NSS_Shutdown.restype = ctypes.c_int
        # endregion
        # region PK11_GetInternalKeySlot
        self.PK11_GetInternalKeySlot = self.lib_nss.PK11_GetInternalKeySlot
        self.PK11_GetInternalKeySlot.restype = ctypes.c_void_p
        # endregion
        # region PK11_NeedUserInit
        self.PK11_NeedUserInit = self.lib_nss.PK11_NeedUserInit
        self.PK11_NeedUserInit.argtypes = [ctypes.c_void_p]
        self.PK11_NeedUserInit.restype = ctypes.c_int
        # endregion
        # region PK11_InitPin
        self.PK11_InitPin = self.lib_smime3.PK11_InitPin
        self.PK11_InitPin.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_char_p]
        self.PK11_InitPin.restype = ctypes.c_int
        # endregion
        # region PK11_FreeSlot
        self.PK11_FreeSlot = self.lib_nss.PK11_FreeSlot
        self.PK11_FreeSlot.argtypes = [ctypes.c_void_p]
        self.PK11_FreeSlot.restype = ctypes.c_int
        # endregion
        # region PK11SDR_Encrypt
        self.PK11SDR_Encrypt = self.lib_nss.PK11SDR_Encrypt
        self.PK11SDR_Encrypt.argtypes = [ptr_seci, ptr_seci, ptr_seci, prt_secupwdata]
        self.PK11SDR_Encrypt.restype = ctypes.c_int
        # endregion
        # region NSSBase64_EncodeItem
        self.NSSBase64_EncodeItem = self.lib_nss.NSSBase64_EncodeItem
        self.NSSBase64_EncodeItem.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_uint, ptr_seci]
        self.NSSBase64_EncodeItem.restype = ctypes.c_char_p
        # endregion

        self.NSS_InitReadWrite(str("sql:" + path).encode("utf-8"))
        slot = self.PK11_GetInternalKeySlot()
        if self.PK11_NeedUserInit(slot):
            self.PK11_InitPin(slot, None, "".encode())
        self.PK11_FreeSlot(slot)

    def __enter__(self):
        return self

    def encrypt(self, plaintext: str):
        b_str = plaintext.encode()
        keyid = self.SECItem(0, 0, 0)
        data = self.SECItem(0, b_str, len(b_str))
        result = self.SECItem(0, 0, 0)
        pw_data = self.SECuPWData(0, 0)

        self.PK11SDR_Encrypt(keyid, data, result, pw_data)
        e_data: bytes = self.NSSBase64_EncodeItem(None, None, 0, result)

        return e_data.decode("utf-8").replace("\r\n", "")

    def decrypt(self, encrypt_data):
        pass

    def close(self):
        return self.NSS_Shutdown()

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()
