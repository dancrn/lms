include rules.make
include opts.make
include files.make

lms-build: $(LMS_OBJECTS)
	@cp $^ build/
	@make -s -C build lms-build

clean:
	@rm -f *.o
	@+make -s -C build clean

.PHONY: clean
