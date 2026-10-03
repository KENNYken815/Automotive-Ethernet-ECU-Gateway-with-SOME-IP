CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Iinclude -O2
BUILD := build
CORE := src/someip.c src/gateway.c
.PHONY: all demo test clean
all: demo test
$(BUILD):
	mkdir -p $(BUILD)
$(BUILD)/gateway_demo: $(CORE) src/main.c | $(BUILD)
	$(CC) $(CFLAGS) $(CORE) src/main.c -o $@
$(BUILD)/test_gateway: $(CORE) tests/test_gateway.c | $(BUILD)
	$(CC) $(CFLAGS) $(CORE) tests/test_gateway.c -o $@
demo: $(BUILD)/gateway_demo
	./$(BUILD)/gateway_demo
test: $(BUILD)/test_gateway
	./$(BUILD)/test_gateway
clean:
	rm -rf $(BUILD)
