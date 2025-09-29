# shortest_path

## Dependencies

- google benchmark

Install with:

```bash
git clone https://github.com/google/benchmark.git
cd benchmark
cmake -E make_directory "build"
cmake -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_DOWNLOAD_DEPENDENCIES=ON -S . -B build
cmake --build build --config Release
sudo cmake --install build
```

## Run the main program

```bash
make main
```

```bash
./main
```

## Run the tests

```bash
make test_app
```

```bash
./test_app
```

## Make the libraries

```bash
make sharedlibs
```

## Benchmark

```bash
make run-benches
```

## Clean

```bash
make clean
```