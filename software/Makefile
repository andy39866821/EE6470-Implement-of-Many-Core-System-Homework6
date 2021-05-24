all : main.cpp
	riscv32-unknown-elf-g++ -std=c++14 main.cpp -o main -march=rv32ima -mabi=ilp32
	
sim: all
	../bin/riscv-vp-hw6 --intercept-syscalls main
	
clean:
	rm -f main
