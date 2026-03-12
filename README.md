# JobDispatcher

A distributed job dispatching system designed to process computational tasks efficiently using parallel processing with MPI or serial execution. The system handles multiple client requests for mathematical computations including prime number analysis and word permutations.

## Overview

JobDispatcher is a C-based project that implements a master-worker architecture for processing job requests. It supports both:
- **Parallel Implementation** (`job_dispatcher.c`) - Uses MPI (Message Passing Interface) for distributed computing across multiple processes
- **Serial Implementation** (`job_dispatcher_serial.c`) - Single-threaded version for baseline comparison

## Features

### Supported Operations

#### 1. **PRIMES** - Prime Number Analysis
- **Description**: Counts the number of prime numbers in the range from 2 to n (where n is the input)
- **Input Format**: `<CLIENT_ID> PRIMES <number>`
- **Example**: `CLI0 PRIMES 10000987`
- **Output**: Number of primes found (written to `<CLIENT_ID>.txt`)

#### 2. **PRIMEDIVISORS** - Prime Divisor Count
- **Description**: Counts the number of distinct prime divisors of a given number
- **Input Format**: `<CLIENT_ID> PRIMEDIVISORS <number>`
- **Example**: `CLI0 PRIMEDIVISORS 452876`
- **Output**: Count of distinct prime divisors (written to `<CLIENT_ID>.txt`)

#### 3. **ANAGRAMS** - Word Permutation Generation
- **Description**: Generates all possible permutations/anagrams of a given word (up to 8 characters)
- **Input Format**: `<CLIENT_ID> ANAGRAMS <word>`
- **Example**: `CLI1 ANAGRAMS tralala`
- **Output**: All permutations (one per line, written to `<CLIENT_ID>.txt`)

#### 4. **WAIT** - Delay Command
- **Description**: Introduces a delay in command processing
- **Input Format**: `WAIT <duration>`
- **Example**: `WAIT 2` (delays processing by 2 × 250ms)
- **Effect**: Useful for simulating time-dependent scenarios in testing

## Project Structure

```
JobDispatcher/
├── job_dispatcher.c          # Parallel implementation using MPI
├── job_dispatcher_serial.c   # Serial/single-threaded implementation
├── requests_file.txt         # Sample job requests (for testing)
├── requests_file2.txt        # Alternative job requests file
├── log_file.txt              # Generated execution log with timestamps
├── README.md                 # This file
└── *.txt                     # Generated output files (one per client)
```

## Key Data Structures & Functions

### Core Computational Functions

- **`isPrime(long n)`** - Efficient primality test using trial division
- **`count_primes(long n)`** - Counts primes in range [2, n)
- **`count_prime_divisors(long n)`** - Finds distinct prime divisors
- **`generate_anagrams(char *str)`** - Uses backtracking to generate all permutations

### Utility Functions

- **`valid()`** - Validates permutation candidates (checks for duplicates)
- **`solution()`** - Determines when a complete permutation is formed
- **`factorial(int n)`** - Calculates factorial (used for memory allocation)
- **`generate_permutations()`** - Backtracking algorithm for permutation generation

### I/O Functions

- **`write_to_client_file()`** - Writes results to client-specific output files (MPI version)
- Logging with microsecond-precision timestamps

## Building & Running

### Prerequisites

**For Parallel Version:**
- MPI compiler (e.g., `mpicc` from OpenMPI or MPICH)
- C compiler with standard library support

**For Serial Version:**
- Standard C compiler (gcc, clang, etc.)

### Compilation

```bash
# Parallel version (MPI)
mpicc -o job_dispatcher job_dispatcher.c

# Serial version
gcc -o job_dispatcher_serial job_dispatcher_serial.c
```

### Execution

**Parallel Version (requires MPI environment):**
```bash
# Run with N processes (master + N-1 workers)
mpirun -np 4 ./job_dispatcher
```

**Serial Version:**
```bash
./job_dispatcher_serial
```

## Input/Output

### Input Format

The system reads job requests from a file (typically `requests_file2.txt` for serial or via MPI for parallel). Each line contains:

```
<CLIENT_ID> <OPERATION> <ARGUMENT>
```

or for WAIT commands:

```
WAIT <duration_units>
```

### Example Input File (`requests_file.txt`)

```
CLI0 PRIMEDIVISORS 452876
CLI1 ANAGRAMS tralala
WAIT 2
CLI2 PRIMEDIVISORS 129072
CLI3 PRIMES 2908764
WAIT 1
CLI4 PRIMES 10000987
CLI5 PRIMES 12043876
CLI6 PRIMES 20876
CLI7 ANAGRAMS friend
WAIT 3
CLI8 ANAGRAMS sunshine
CLI9 ANAGRAMS rainbow
CLI10 PRIMES 3451629
```

### Output Format

**Client Output Files:**
- Each client receives a file named `<CLIENT_ID>.txt`
- Contains the result of their operation

Example output for `CLI0.txt` (PRIMEDIVISORS):
```
4
```

Example output for `CLI1.txt` (ANAGRAMS):
```
tralala
traala
talrala
...
```

**Log File:**
- `log_file.txt` - Contains timestamped records of all operations processed
- Format: `Timestamp <time>: '<COMMAND>' <status_info>`

## Architecture Details

### Parallel Implementation (MPI)

- **Master Process (rank 0)**:
  - Reads job requests from input file
  - Distributes tasks to worker processes
  - Collects results and writes them to individual client output files
  - Maintains execution log with timestamps

- **Worker Processes (rank 1..N-1)**:
  - Wait for tasks from master
  - Perform computational work
  - Send results back to master

- **Communication Protocol**:
  - `WORK_TAG (1)`: Task assignment from master to worker
  - `FINISH_TAG (2)`: Termination signal

### Serial Implementation

- Single-threaded execution
- Processes jobs sequentially as read from input file
- Simpler flow but identical computational logic
- Useful for performance comparison and debugging

## Computational Complexity

| Operation | Time Complexity | Notes |
|-----------|-----------------|-------|
| PRIMES (n) | O(n√n) | Optimized trial division for primality |
| PRIMEDIVISORS (n) | O(n√n) | Checks all divisors up to n |
| ANAGRAMS (m chars) | O(m!) | Generates all permutations; max 8 chars |

## Limitations

- Maximum word length for ANAGRAMS: 8 characters
- Limited to positive integers for PRIMES and PRIMEDIVISORS
- Buffer sizes are fixed (command strings: 50 chars, words: 20-10 chars)
- Serial version processes one request at a time

## Performance Considerations

- The parallel version (MPI) should outperform the serial version for large numbers or multiple simultaneous PRIMES/PRIMEDIVISORS operations
- ANAGRAMS performance is factorial-dependent on input length
- Memory allocation for ANAGRAMS results can be significant for longer words

## Testing

The project includes sample input files (`requests_file.txt`, `requests_file2.txt`) for testing both implementations. Compare output files and log timestamps to verify correctness and performance.

## Author Information

This project was developed as part of the PDA (Parallel and Distributed Algoriths) course.

## Notes

- Time units in WAIT commands are multiplied by 250ms internally (`usleep(time * 250000)`)
- Timestamps are recorded to microsecond precision
- Client files are created/appended using `fopen(..., "a+")` mode for flexibility
- Memory is dynamically allocated based on factorial of word length or maximum permutation set size