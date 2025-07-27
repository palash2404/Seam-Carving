# Seam Carving Algorithm Implementation

A C implementation of the seam carving algorithm for content-aware image resizing, based on the seminal paper "Seam Carving for Content-Aware Image Resizing" by Avidan and Shamir (SIGGRAPH 2007).

## Overview

Seam carving is an algorithm for content-aware image resizing that removes (or adds) pixels in a way that preserves the most important visual content. Instead of traditional scaling that can distort images, seam carving identifies and removes the least noticeable pixel paths (seams) from the image.

## Features

- **Energy Calculation**: Computes pixel importance using gradient-based energy functions
- **Dynamic Programming**: Efficiently finds optimal seams using DP algorithms  
- **Vertical & Horizontal Seams**: Supports both width and height reduction
- **P3 PPM Support: Handles P3 (ASCII) portable pixmap format for image I/O
- **Performance Optimized**: Includes both debug and optimized builds
- **Comprehensive Testing**: Full test suite with various image scenarios

## Building

```bash
make
```

This creates:
- `bin/carve_debug` - Debug version with sanitizers and debugging symbols
- `bin/carve_opt` - Optimized version for performance
- `bin/testrunner` - Unit test executable

## Usage

```bash
# Basic usage - shows image statistics
./bin/carve_opt input.ppm

# Resize image (example - check argparser.h for full options)
./bin/carve_opt -w 400 -h 300 input.ppm

# Run with debug version for development
./bin/carve_debug input.ppm
```

## Testing

Run the comprehensive test suite:

```bash
# Run all tests
make check

# Run custom tests
make test_all

# List available tests
python3 test/run_tests.py -l

# Run specific test
python3 test/run_tests.py -f [TESTNAME]
```

📋 **For detailed testing instructions and troubleshooting, see [TESTING.md](TESTING.md)**

## Algorithm Overview

1. **Energy Calculation**: For each pixel, calculate energy based on color gradients
2. **Seam Detection**: Use dynamic programming to find minimum energy vertical/horizontal path
3. **Seam Removal**: Remove the seam and shift remaining pixels
4. **Iteration**: Repeat until desired dimensions are reached

## Project Structure

```
src/
├── main.c          # Main program and CLI interface
├── image.c/.h      # Image loading, saving, and basic operations
├── energy.c/.h     # Energy calculation algorithms
├── argparser.c/.h  # Command line argument parsing
├── indexing.c/.h   # 2D array indexing utilities
└── util.h          # Common definitions and utilities

test/
├── run_tests.py    # Python test runner
├── data/           # Test images (PPM format)
├── ref_output/     # Expected outputs for validation
└── custom_tests/   # Additional C test files
```

## Implementation Highlights

- **Memory Management**: Careful allocation and deallocation in C
- **Performance**: Optimized algorithms with O(n*m) complexity for seam finding
- **Robustness**: Comprehensive error handling and edge case management
- **Code Quality**: Clean, well-documented C code following best practices
- **Testing**: Extensive test coverage including boundary conditions

## Technical Details

### Energy Functions
The implementation uses gradient-based energy functions that measure the rate of color change at each pixel. High-energy pixels (edges, textures) are preserved while low-energy pixels (uniform areas) are candidates for removal.

### Dynamic Programming Approach
Seam detection uses dynamic programming to efficiently find the minimum energy path through the image, ensuring optimal seam selection in linear time.

### Image Format
Works with P3 PPM (Portable Pixmap) format - ASCII text-based variant with human-readable RGB values.

## Academic Context

This project demonstrates understanding of:
- **Advanced Algorithms**: Dynamic programming and optimization
- **Image Processing**: Digital image manipulation and analysis  
- **Systems Programming**: Memory management and performance optimization in C
- **Software Engineering**: Testing, documentation, and code organization

## References

- Avidan, S., & Shamir, A. (2007). Seam carving for content-aware image resizing. ACM SIGGRAPH 2007 papers.

## License

This project was developed for educational purposes as part of a computer graphics course.
