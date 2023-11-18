import sys
b = [x for x in open(sys.argv[1],"rb").read(-1)]

print("const uint8_t test_bin[] = {")
print(",".join([str(x) for x in b]))
print("};\n")

print("#define TESTLOAD (0x{0:04x})".format(0xFC00))
print("#define TESTSIZE (0x{0:04x})".format(len(b)))