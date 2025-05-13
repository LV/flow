BUILD_DIR := build
EXECUTABLE := flow

.PHONY: all
all: run

.PHONY: build
build:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make -j

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: run
run: build
	./$(BUILD_DIR)/$(EXECUTABLE)

.PHONY: setup
setup:
	git submodule update --init --recursive
