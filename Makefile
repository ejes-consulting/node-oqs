# Makefile:  Make NODE-OQS binding for nodejs
LIBOQS_DIR = $(shell pwd)/external/liboqs
LIBOQS_BUILD_DIR = $(LIBOQS_DIR)/build
NODE_BIN := $(shell command -v node)
NODE_INCLUDE = $(shell $(NODE_BIN) -p process.execPath | sed -e 's/\/bin\/node$$/\/include\/node/')

CC=clang
CFLAGS = -std=c17 -Wall -fPIC
LDFLAGS = -Wl,-undefined,dynamic_lookup

all: liboqs.node

liboqs.node: liboqs-node.c src/binding.c src/kem.c src/signature.c $(LIBOQS_BUILD_DIR)/lib/liboqs.a
	$(CC) $(CFLAGS) -I$(NODE_INCLUDE) -I$(LIBOQS_BUILD_DIR)/include $(LDFLAGS) -L$(LIBOQS_BUILD_DIR)/lib -loqs -o $@ liboqs-node.c src/binding.c src/kem.c src/signature.c

.PHONY: liboqs-init liboqs.a
liboqs-init:
	git submodule update --init -- "$(LIBOQS_DIR)"

liboqs.a: $(LIBOQS_BUILD_DIR)/lib/liboqs.a

$(LIBOQS_BUILD_DIR)/lib/liboqs.a: liboqs-init
	mkdir -p $(LIBOQS_BUILD_DIR)
	(cd $(LIBOQS_BUILD_DIR) && cmake -GNinja .. && ninja)

clean:
	test -d $(LIBOQS_BUILD_DIR) && (cd $(LIBOQS_BUILD_DIR) && ninja clean)
	rm -f liboqs.node
	rm -rf $(LIBOQS_BUILD_DIR)

distclean: clean
	rm -r $(LIBOQS_DIR)

# eof
