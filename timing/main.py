import ctypes
import pathlib

lib = ctypes.CDLL(pathlib.Path(__file__).parent.parent / "shortest_path" / "dijkstra_c_api.dll")

print(lib.dijkstra_test())