include rules.make
include opts.make

lms-build: main.o lms.o modules.o
	@cp $^ build/
	@make -C build lms-build

clean:
	@rm -f *.o
	@+make -C build clean

.PHONY: clean
