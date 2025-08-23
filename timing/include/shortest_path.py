import ctypes
import pathlib


class ShortestPathLib:
    def __init__(self):
        dll_path = pathlib.Path(__file__).parent.parent.parent / "shortest_path" / "shortest_path.dll"
        self.lib = ctypes.CDLL(str(dll_path))

        # Set argument and return types for the DLL functions
        self.lib.generate_graph.argtypes = [ctypes.c_int, ctypes.c_int]
        self.lib.generate_graph.restype = None

        self.lib.initialize.argtypes = [ctypes.c_bool]
        self.lib.initialize.restype = None

        self.lib.get_shortest_path.argtypes = []
        self.lib.get_shortest_path.restype = None

    def generate_graph(self, num_vertices, num_edges):
        self.lib.generate_graph(num_vertices, num_edges)

    def initialize(self, bmssp=False):
        self.lib.initialize(bmssp)

    def get_shortest_path(self):
        self.lib.get_shortest_path()
