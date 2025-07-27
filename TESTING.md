# Testing and Running the Seam Carving Project

This document explains how to test the seam carving implementation and run the application.

## Prerequisites

Before testing, ensure you have the following installed:
- A C compiler (gcc or clang)
- Python 3 (for test scripts)
- clang-format (for code formatting)
- make (for build automation)

On macOS, you can install clang-format using Homebrew:
```bash
brew install clang-format
```

## Building the Project

To build all components of the project:

```bash
make all
```

This will create:
- `bin/carve_opt` - Optimized version of the seam carving application
- `bin/carve_debug` - Debug version with AddressSanitizer and UndefinedBehaviorSanitizer
- `bin/testrunner` - Unit test runner

## Running Tests

### 1. Main Test Suite

Run the comprehensive Python test suite:

```bash
python3 test/run_tests.py
```

**Note:** The `make check` command may fail due to missing image archives, but the direct Python command works correctly.

This runs 17 public tests covering:
- Image statistics (brightness calculations)
- Minimum energy path finding
- Seam carving functionality

**Expected output:** All tests should pass with "All 17 tests successful!"

### 2. Custom Test Categories

Run specific test categories:

```bash
# Basic custom tests (brightness, image cutting, edge cases)
make test_custom

# Advanced tests (color processing, boundary values, performance)
make test_harder

# All custom tests combined
make test_all
```

### 3. Individual Test Categories

The custom tests are organized into three groups:

**Basic Tests (`test_custom`):**
- `bin/test_brightness` - Brightness calculation tests
- `bin/test_image_cutting` - Image manipulation tests  
- `bin/test_edge_cases` - Edge case handling

**Advanced Tests (`test_harder`):**
- `bin/test_color_processing` - Color channel processing
- `bin/test_boundary_values` - Boundary condition tests
- `bin/test_performance` - Performance benchmarks

**Comprehensive Tests (`test_all`):**
- `bin/test_advanced_patterns` - Complex pattern recognition
- `bin/test_special_cases` - Special case handling
- `bin/test_seam_carving` - End-to-end seam carving tests

**Note:** Individual test binaries are built when you run the respective `make` commands (e.g., `make test_custom` builds the basic test binaries).

## Verifying Test Success

All tests should pass without errors. Look for:
- "PASS" or "PASSED" messages for individual tests
- "All X tests successful!" for the main test suite
- No error messages or segmentation faults

If any test fails, check:
1. Build completed successfully
2. All required files are present in `test/data/`
3. No compiler warnings that might indicate issues

## Running the Seam Carving Application

### Basic Usage

```bash
./bin/carve_debug <image_file>
./bin/carve_opt <image_file>
```

### Command Line Options

- `-n <count>` - Carve the specified number of vertical seams (creates `out.ppm`)
- `-p` - Print the minimum energy path coordinates to stdout
- `-s` - Show image statistics (width, height, brightness) to stdout

**Note:** When using `-n`, the carved image is saved as `out.ppm` in the current directory.

### Examples

1. **Show image statistics:**
   ```bash
   ./bin/carve_debug -s test/data/small1.ppm
   ```
   Output: width, height, and brightness values

2. **Find minimum energy path:**
   ```bash
   ./bin/carve_debug -p test/data/small1.ppm
   ```
   Output: y-coordinates of the minimum energy path

3. **Carve one seam:**
   ```bash
   ./bin/carve_debug -n 1 test/data/small1.ppm
   ```
   Output: Creates `out.ppm` with one vertical seam removed

4. **Carve multiple seams:**
   ```bash
   ./bin/carve_debug -n 5 test/data/owl2.ppm
   ```
   Output: Creates `out.ppm` with 5 vertical seams removed

5. **Save carved image with custom name:**
   ```bash
   ./bin/carve_debug -n 3 test/data/small2.ppm && mv out.ppm carved_small2.ppm
   ```

### Test Images

Sample images are available in `test/data/`:
- `small1.ppm` - 3x3 test image
- `small2.ppm` - Small test image
- `owl.ppm` - Larger test image
- `owl2.pgm` - Grayscale version

### Output Files

When carving seams (using `-n` flag), the application creates:
- `out.ppm` - The carved image with specified number of seams removed

The original image dimensions are preserved by adding black pixels on the right side where seams were removed.

## Performance Testing

The debug version includes AddressSanitizer for memory safety checking but runs slower. Use the optimized version for performance testing:

```bash
# Performance comparison (using small images for quick testing)
time ./bin/carve_debug -n 1 test/data/small1.ppm > /dev/null
time ./bin/carve_opt -n 1 test/data/small1.ppm > /dev/null
```

## Troubleshooting

### Build Issues

1. **clang-format not found:**
   ```bash
   brew install clang-format
   ```

2. **Missing test images:**
   Check if `test/data/` contains PPM/PGM files

3. **Compiler warnings:**
   Review and fix any warnings in the source code

### Test Failures

1. **Memory errors in debug mode:**
   - Check for buffer overflows
   - Verify array bounds checking
   - Review pointer arithmetic

2. **Incorrect output:**
   - Compare with reference outputs in `test/ref_output/`
   - Verify algorithm implementation
   - Check edge case handling

### Application Issues

1. **Invalid image format:**
   - Ensure input is a valid PPM/PGM file
   - Check image dimensions are positive

2. **Path not found:**
   - Verify energy calculation is correct
   - Check dynamic programming implementation

## Continuous Integration

To verify the repository is passing all tests, run:

```bash
make clean && make all && python3 test/run_tests.py && make test_all
```

This command:
1. Cleans previous builds
2. Rebuilds everything
3. Runs the main test suite
4. Runs all custom tests

All steps should complete successfully for the repository to be considered passing.

**Note:** The build may show warnings about missing image archives and function prototypes, but these don't affect functionality.
