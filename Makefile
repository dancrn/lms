
all: lms

lms: bin_dir lms-build
	@cp src/build/lms bin/lms

lms-build:
	@make -C src lms-build

bin_dir:
	@mkdir -p bin 

clean:
	@make -C src clean
