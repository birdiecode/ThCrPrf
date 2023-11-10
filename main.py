from NSSinterlayer import NSSinterlayer

nss = NSSinterlayer()
data = nss.encrypt("test_text")
print(data)
print(nss.dectypt(data))