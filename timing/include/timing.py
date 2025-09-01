import random
import time
import csv
from typing import List

from include.shortest_path import ShortestPathLib


class Timing:
    """A class to time the shortest path algorithm."""

    def __init__(
            self, 
            vertex_ranges: List[int], 
            edge_ranges: List[int], 
            iterations_per_combination: int, 
            graph_combinations: int
        ):
        """
        Initialize the Timing class with the given parameters.

        Parameters:
            vertex_ranges: List of vertex counts to test.
            edge_ranges: List of edge counts to test.
            iterations_per_combination: Number of iterations to run for each graph configuration.
            graph_combinations: Number of different graph configurations to test.
        """
        self.vertex_ranges = vertex_ranges
        self.edge_ranges = edge_ranges
        self.iterations_per_combination = iterations_per_combination
        self.graph_combinations = graph_combinations
        self.shortest_path_instance = ShortestPathLib()

    def run(self, output_file: str):
        if not output_file.endswith(".csv"):
            raise ValueError("Output file must be a .csv file")

        fieldnames = ["vertices", "edges", "iteration", "graph_config", "elapsed_seconds", "bmssp"]
        with open(output_file, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()

            for vertex_count in self.vertex_ranges:
                for edge_count in self.edge_ranges:
                    random_seed = random.randint(0, 100000)
                    self.shortest_path_instance.set_random_seed(random_seed)
                    self.shortest_path_instance.generate_graph(vertex_count, edge_count)

                    for graph_config in range(self.graph_combinations):
                        self.shortest_path_instance.set_start_end_nodes()
                        
                        for i in range(self.iterations_per_combination):
                            for bmssp in [False, True]:
                                vertex_count, edge_count = self.shortest_path_instance.initialize(bmssp=bmssp)
                                start = time.time()
                                self.shortest_path_instance.get_shortest_path()
                                elapsed = time.time() - start
                                writer.writerow({
                                    "vertices": vertex_count,
                                    "edges": edge_count,
                                    "iteration": i + 1,
                                    "graph_config": graph_config,
                                    "elapsed_seconds": elapsed,
                                    "bmssp": bmssp
                                })
                                f.flush()  # Ensure data is written to disk
