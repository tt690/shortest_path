import ctypes
import pathlib
from typing import Tuple


class InitResult(ctypes.Structure):
    _fields_ = [("vertex_count", ctypes.c_int),
                ("edge_count", ctypes.c_int)]


class ShortestPathLib:
    """A class that provides an interface to the shortest path library."""

    def __init__(self) -> None:
        """
        Initialize the ShortestPathLib class.
        """
        so_path = pathlib.Path(__file__).parent.parent.parent / "shortest_path" / "shortest_path.so"
        self.lib = ctypes.CDLL(str(so_path))

        self.lib.set_random_seed.argtypes = [ctypes.c_uint]
        self.lib.set_random_seed.restype = None

        self.lib.generate_graph.argtypes = [ctypes.c_int, ctypes.c_int]
        self.lib.generate_graph.restype = None

        self.lib.set_start_end_nodes.argtypes = []
        self.lib.set_start_end_nodes.restype = None

        self.lib.initialize.argtypes = [ctypes.c_bool]
        self.lib.initialize.restype = InitResult

        self.lib.get_shortest_path.argtypes = []
        self.lib.get_shortest_path.restype = None

    def set_random_seed(self, seed: int) -> None:
        """
        Set the random seed for the graph generation.

        Parameters:
            seed: The random seed value.
        """
        self.lib.set_random_seed(seed)

    def generate_graph(self, num_vertices: int, num_edges: int) -> None:
        """
        Generate a random graph.

        Parameters:
            num_vertices: The number of vertices in the graph.
            num_edges: The number of edges in the graph.
        """
        self.lib.generate_graph(num_vertices, num_edges)

    def set_start_end_nodes(self) -> None:
        """
        Set the start and end nodes for the shortest path algorithm.
        """
        self.lib.set_start_end_nodes()

    def initialize(self, bmssp: bool = False) -> Tuple[int, int]:
        """
        Initialize the shortest path algorithm.

        Parameters:
            bmssp: Whether to use the Bidirectional Multi-Source Shortest Path (BMSSP) algorithm.

        Returns:
            The number of vertices and edges in the graph stored in a tuple.
        """
        result = self.lib.initialize(bmssp)
        return result.vertex_count, result.edge_count

    def get_shortest_path(self) -> None:
        """
        Get the shortest path from the source to the target.
        """
        self.lib.get_shortest_path()
