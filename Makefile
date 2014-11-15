
all: lms

lms: bin_dir
	make -C src lms-build

bin_dir:
	@mkdir -p bin 
