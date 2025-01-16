LIBOQS_DIR = $(shell pwd)/external/liboqs
LIBOQS_BUILD_DIR = $(LIBOQS_DIR)/build

# node -p process.execPath
CC=clang
CFLAGS = -std=c17 -Wall -fPIC -I/opt/homebrew/Cellar/node@16/16.20.2_2/include/node
LDFLAGS = -Wl,-undefined,dynamic_lookup

# Compiler flags
CFLAGS += -I$(LIBOQS_BUILD_DIR)/include
LDFLAGS += -L$(LIBOQS_BUILD_DIR)/lib -loqs

all: liboqs.node

liboqs.node: liboqs-node.c src/binding.c src/kem.c src/signature.c liboqs
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ liboqs-node.c src/binding.c src/kem.c src/signature.c liboqs

.PHONY: liboqs
liboqs:
	mkdir -p $(LIBOQS_BUILD_DIR)
	(cd $(LIBOQS_BUILD_DIR) && cmake -DCMAKE_BUILD_TYPE=Release .. && make)

clean:
	rm -f liboqs.node
	rm -rf $(LIBOQS_BUILD_DIR)
