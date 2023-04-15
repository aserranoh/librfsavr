
TESTS = testusart.py
TEST_EXTENSIONS = .py
PY_LOG_COMPILER = $(PYTHON)
AM_TESTS_ENVIRONMENT = AVR_DEV='$(AVR_DEV)'; export AVR_DEV;

.bin.hex:
	$(OBJCOPY) -O ihex -R .eeprom $< $@

check_PROGRAMS = testusart.bin
testusart_bin_SOURCES = testusart.c
testusart_bin_CFLAGS = $(CPU_FREQ) $(MCU_FLAG) -I$(top_srcdir)/src
testusart_bin_LDADD = $(top_builddir)/src/librfsavr.la

check_SCRIPTS = testusart.hex 
CLEANFILES = $(check_SCRIPTS)
dist_check_SCRIPTS = testusart.py avrloader.py autotests.py