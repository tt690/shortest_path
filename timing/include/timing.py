import random
import itertools
import time
import csv
from typing import List, TextIO

from include.shortest_path import ShortestPathLib


class Timing:
    """A class to time the shortest path algorithm."""

    def __init__(
            self, 
            vertex_ranges: List[int], 
            edge_ranges: List[int], 
            iterations_per_combination: int, 
            graph_combinations: int
        ) -> None:
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

    def run(self, output_file: str) -> None:
        """
        Run the timing tests and write results to the specified CSV file.

        Parameters:
            output_file: The path to the output CSV file or folder.
        """
        if not "." in output_file:
            output_file = self._folder_to_file(output_file)

        if not output_file.endswith(".csv"):
            raise ValueError("Output file must be a .csv file or a folder")

        fieldnames = ["vertices", "edges", "iteration", "graph_config", "elapsed_seconds", "bmssp"]
        with open(output_file, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()

            for vertex_count, edge_count in itertools.product(self.vertex_ranges, self.edge_ranges):
                self._run_graph_configs(vertex_count, edge_count, writer, f)

    def _folder_to_file(self, folder: str) -> str:
        """
        Convert a folder path to a file path by appending a filename and auto-generating a timestamp.

        Parameters:
            folder: The folder path.
        """
        timestamp = time.strftime("%Y%m%d-%H%M%S")
        return f"{folder}/timing_results_{timestamp}.csv"

    def _run_graph_configs(
            self, 
            vertex_count: int, 
            edge_count: int, 
            writer: csv.DictWriter, 
            output_file: TextIO
        ) -> None:
        """
        Run tests for different graph configurations.
        Parameters:
            vertex_count: The number of vertices in the graph.
            edge_count: The number of edges in the graph.
            writer: The CSV writer object.
            f: The file object for flushing.
        """
        random_seed = random.randint(0, 100000)
        self.shortest_path_instance.set_random_seed(random_seed)
        self.shortest_path_instance.generate_graph(vertex_count, edge_count)

        for graph_config in range(self.graph_combinations):
            self.shortest_path_instance.set_start_end_nodes()
            self._run_iterations(graph_config, writer, output_file)

    def _run_iterations(
            self, 
            graph_config: int, 
            writer: csv.DictWriter, 
            output_file: TextIO
        ) -> None:
        """
        Run the specified number of iterations for a given graph configuration.
        Parameters:
            graph_config: The current graph configuration index.
            writer: The CSV writer object.
            output_file: The file object for flushing.
        """
        for bmssp in [False, True]:
            for i in range(self.iterations_per_combination):
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
                output_file.flush()
