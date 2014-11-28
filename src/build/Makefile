include ../rules.make
include ../opts.make
include ../files.make

lms-build: $(LMS_OBJECTS)
	@echo "LD     lms"
	@$(CC) $^ -o lms -lm

clean:
	@rm -f $(LMS_OBJECTS) lms
