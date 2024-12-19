# Targets

# Default target
all:
	@echo "Checking for TCC or GCC..."
	@command -v tcc >/dev/null 2>&1 && TCC=true || TCC=false
	@if [ "$(TCC)" = "true" ]; then \
		echo "Using TCC..."; \
		CC=tcc; \
	else \
		echo "Using GCC..."; \
		CC=gcc; \
	fi

	# Step 1: Change to libsynasm directory and create staticlib directory
	@cd libsynasm && \
	mkdir -p staticlib && \
	$(CC) -I. -fPIC -c arch_m68k.c libsynasm.c && \
	ar rcs staticlib/libsynasm.a arch_m68k.o libsynasm.o && \
	$(CC) -shared -fPIC -o libsynasm.so arch_m68k.o libsynasm.o

	# Step 2: Change back to the src directory and compile nlas with static library
	@cd .. && \
	$(CC) -I libsynasm -L libsynasm/staticlib -lsynasm -fPIE -pie nlas.c -o nlas.o

	# Step 3: Link the final binary using static library (libsynasm.a)
	@cd .. && \
	$(CC) -o nlas nlas.o -L libsynasm/staticlib -lsynasm -fPIE -pie

	@echo "Build completed."

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -f libsynasm/staticlib/libsynasm.a libsynasm/libsynasm.so nlas.o nlas

.PHONY: all clean
