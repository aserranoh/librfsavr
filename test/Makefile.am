
TESTS = testusart.py testleds.py testpwm.py
TEST_EXTENSIONS = .py
PY_LOG_COMPILER = $(PYTHON)
AM_TESTS_ENVIRONMENT = AVR_DEV='$(AVR_DEV)'; export AVR_DEV; AVR_PROGRAMMING_BAUDS='$(AVR_PROGRAMMING_BAUDS)'; export AVR_PROGRAMMING_BAUDS;

.bin.hex:
	$(OBJCOPY) -O ihex -R .eeprom $< $@

check_PROGRAMS = testusart.bin testleds.bin testpwm.bin
TESTBIN_CFLAGS = $(CPU_FREQ) -mmcu=atmega328p -I$(top_srcdir)/src
TESTBIN_LDADD = $(top_builddir)/src/librfsavr-atmega328p.la

testusart_bin_SOURCES = testusart.c
testusart_bin_CFLAGS = $(TESTBIN_CFLAGS)
testusart_bin_LDADD = $(TESTBIN_LDADD)

testleds_bin_SOURCES = testleds.c
testleds_bin_CFLAGS = $(TESTBIN_CFLAGS)
testleds_bin_LDADD = $(TESTBIN_LDADD)

testpwm_bin_SOURCES = testpwm.c
testpwm_bin_CFLAGS = $(TESTBIN_CFLAGS)
testpwm_bin_LDADD = $(TESTBIN_LDADD)

check_SCRIPTS = testusart.hex testleds.hex testpwm.hex
CLEANFILES = $(check_SCRIPTS)
dist_check_SCRIPTS = testusart.py testleds.py testpwm.py avrloader.py autotests.py avrtests.py