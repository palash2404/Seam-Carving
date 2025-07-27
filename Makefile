BIN_NAME    := carve
TESTER_NAME := testrunner

BIN_FILES    := src/argparser.c src/energy.c src/image.c src/main.c src/indexing.c
TESTER_FILES := src/argparser.c src/energy.c src/image.c src/indexing.c src/unit_tests.c src/test_main.c
HEADERS      := $(wildcard src/*.h)

TEST_SCRIPT := test/run_tests.py
Q ?= @

DEBUG   := -O0 -g -fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer
OPT     := -O3

CFLAGS  += -I src -Wall -Wextra -pedantic -Wno-sign-compare
LDFLAGS +=

CLANG_FORMAT := clang-format
FORMAT_STYLE := -style=file
CLANG_TIDY := clang-tidy

.PHONY: all check clean

all: bin/$(BIN_NAME)_opt bin/$(BIN_NAME)_debug bin/$(TESTER_NAME)

bin/$(BIN_NAME)_opt: $(patsubst src/%.c, build/%.opt.o, $(BIN_FILES))
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(OPT) $+ $(LDFLAGS)

bin/$(BIN_NAME)_debug: $(patsubst src/%.c, build/%.debug.o, $(BIN_FILES))
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(DEBUG) $+ $(LDFLAGS)

bin/$(TESTER_NAME): $(patsubst src/%.c, build/%.debug.o, $(TESTER_FILES))
	$(Q)mkdir -p $(@D)
	@echo "===> LD $@"
	$(Q)$(CC) -o $@ $(CFLAGS) $(DEBUG) $+ $(LDFLAGS)

build/%.opt.o: src/%.c $(HEADERS)
	$(Q)mkdir -p $(@D)
	@echo "===> FORMAT $<"
	$(Q)$(CLANG_FORMAT) $(FORMAT_STYLE) -i $<
	@echo "===> CC $@"
	$(Q)$(CC) -o $@ -c $(CFLAGS) $(OPT) $<

build/%.debug.o: src/%.c $(HEADERS)
	$(Q)mkdir -p $(@D)
	@echo "===> FORMAT $<"
	$(Q)$(CLANG_FORMAT) $(FORMAT_STYLE) -i $<
	@echo "===> CC $@"
	$(Q)$(CC) -o $@ -c $(CFLAGS) $(DEBUG) $<

format:
	@echo "===> FORMATTING SOURCE FILES"
	$(Q)$(CLANG_FORMAT) $(FORMAT_STYLE) -i $(BIN_FILES) $(TESTER_FILES)

tidy:
	@echo "===> RUNNING CLANG-TIDY"
	$(Q)$(CLANG_TIDY) $(BIN_FILES) -- -I src

check: all
	@echo "===> CHECK"
	$(Q)$(TEST_SCRIPT)

clean:
	@echo "===> CLEAN"
	$(Q)rm -rf bin build
extract_imgs.tgz:
	@make -C test/data > /dev/null

all: extract_imgs.tgz

extract_refs.tgz:
	@make -C test/ref_output > /dev/null

all: extract_refs.tgz

CUSTOM_TESTS = bin/test_brightness bin/test_image_cutting bin/test_edge_cases

# Custom tests require indexing.c since image.c calls yx_index()
bin/test_brightness: test/custom_tests/test_brightness.c src/image.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

bin/test_image_cutting: test/custom_tests/test_image_cutting.c src/image.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

bin/test_edge_cases: test/custom_tests/test_edge_cases.c src/image.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

test_custom: $(CUSTOM_TESTS)
	for test in $(CUSTOM_TESTS); do ./$$test; done

.PHONY: test_custom

HARDER_TESTS = bin/test_color_processing bin/test_boundary_values bin/test_performance

bin/test_color_processing: test/custom_tests/test_color_processing.c src/image.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

bin/test_boundary_values: test/custom_tests/test_boundary_values.c src/image.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

bin/test_performance: test/custom_tests/test_performance.c src/image.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

test_harder: $(HARDER_TESTS)
	for test in $(HARDER_TESTS); do ./$$test; done


MORE_TESTS = bin/test_advanced_patterns bin/test_special_cases bin/test_seam_carving

bin/test_advanced_patterns: test/custom_tests/test_advanced_patterns.c src/image.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

bin/test_special_cases: test/custom_tests/test_special_cases.c src/image.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

bin/test_seam_carving: test/custom_tests/test_seam_carving.c test/custom_tests/seam_carving_adapter.c src/image.c src/energy.c src/indexing.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(DEBUG) $^ -o $@ -I./src

ALL_TESTS = $(CUSTOM_TESTS) $(HARDER_TESTS) $(MORE_TESTS)

test_all: $(ALL_TESTS)
	@echo "\033[1;36m=== Running All Tests ===\033[0m"
	@for test in $(ALL_TESTS); do ./$$test; done

.PHONY: test_all test_custom test_harder

