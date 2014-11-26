include rules.make
include opts.make
include files.make

lms-build: $(LMS_OBJECTS)
	@cp $^ build/
	@make -C build lms-build

clean:
	@rm -f *.o
	@+make -C build clean

.PHONY: clean
