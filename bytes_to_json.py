import json
from array import array

if __name__ == "__main__":
    byte_array = array("b")
    with open("doubles.dat.sz", "rb") as file:
        byte_array.frombytes(file.read())

    with open("compressed.json", "w") as json_file:
        json.dump(byte_array.tolist(), json_file)
