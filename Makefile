CXX      := g++
CXXFLAGS := -std=c++17
LDFLAGS  :=

SRC      := naniwa.cpp
BIN      := naniwa

# --- Release build ---
$(BIN): CXXFLAGS += -O2 -DNDEBUG
$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

# --- Debug build ---
debug: CXXFLAGS += -g -O0 -fsanitize=address,undefined
debug: LDFLAGS  += -fsanitize=address,undefined
debug: $(SRC)
	$(CXX) $(CXXFLAGS) $< -o $(BIN) $(LDFLAGS)

# --- Clean ---
.PHONY: clean
clean:
	rm -f $(BIN)
