import ctypes


class NSSinterlayer:

    def __init__(self, path_lib, path_lib_smime3, path):
        self.lib_nss = ctypes.CDLL(path_lib)
        self.lib_smime3 = ctypes.CDLL(path_lib_smime3)

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

        self.NSS_InitReadWrite(str("sql:" + path).encode("utf-8"))
        slot = self.PK11_GetInternalKeySlot()
        if self.PK11_NeedUserInit(slot):
            self.PK11_InitPin(slot, None, "".encode())
        self.PK11_FreeSlot(slot)

    def __enter__(self):
        return self

    def encrypt(self, plaintext):
        pass

    def decrypt(self, encrypt_data):
        pass

    def close(self):
        return self.NSS_Shutdown()

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.close()
