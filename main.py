from NSSinterlayer import NSSinterlayer

nss = NSSinterlayer()
print(nss.init("./key"))
print(nss.close())

data = nss.encrypt("test_text")
print(data)
print(nss.dectypt(data))