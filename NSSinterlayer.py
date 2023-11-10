import ctypes


class NSSinterlayer:

    def __init__(self):
        nsslib = "/usr/lib/x86_64-linux-gnu/libnss3.so"
        self.libnss = ctypes.CDLL(nsslib)

        self.NSS_Init = self.libnss.NSS_Init
        self.NSS_Init.argtypes = [ctypes.c_char_p]
        self.NSS_Init.restype = ctypes.c_int

        self.NSS_Shutdown = self.libnss.NSS_Shutdown
        self.NSS_Init.restype = ctypes.c_int

    def init(self, path):
        return self.NSS_Init(str("sql:"+path).encode("utf-8"))

    def encrypt(self, plaintext):
        pass

    def dectypt(self, endata):
        pass

    def close(self):
        return self.NSS_Shutdown()