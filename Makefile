
all: lms

lms: bin_dir lms-build
	@cp src/build/lms bin/lms

lms-build:
	@make -s -C src lms-build

bin_dir:
	@mkdir -p bin 

install: lms
	install -c bin/lms /usr/local/bin 
	install -c bin/lms_module_script /usr/local/bin 

uninstall: /usr/local/bin/lms /usr/local/bin/lms_module_script
	rm -f /usr/local/bin/lms /usr/local/bin/lms_module_script

clean:
	@make -s -C src clean

.PHONY: clean
