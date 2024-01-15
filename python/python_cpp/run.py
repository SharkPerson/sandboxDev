import ctypes
import pathlib

c_lib = ctypes.CDLL("/Users/joshuaw7922/Documents/Development/cs/sandboxDev/python_cpp/libmylib.so")
c_lib.saySomething()
