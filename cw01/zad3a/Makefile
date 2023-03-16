PROGRAM_STATIC = ../zad2/replStatic
PROGRAM_SHARED = ../zad2/replShared
PROGRAM_DYNAMIC = ../zad2/replDynamic

STATIC_OUT = result_static.txt
SHARED_OUT = result_shared.txt
DYNAMIC_OUT = result_dynamic.txt

FINAL_REPORT = report.txt

INPUT = input.txt
all: test_static test_shared test_dynamic concatenate
test_static:
	echo "static:" > $(STATIC_OUT)
	cat $(INPUT) | $(PROGRAM_STATIC) >> $(STATIC_OUT)

test_shared:
	echo "shared:" > $(SHARED_OUT)
	cat $(INPUT) | $(PROGRAM_SHARED) >> $(SHARED_OUT)

test_dynamic:
	echo "dynamic:" > $(DYNAMIC_OUT)
	cat $(INPUT) | $(PROGRAM_DYNAMIC) >> $(DYNAMIC_OUT)

concatenate:
	cat $(STATIC_OUT) $(SHARED_OUT) $(DYNAMIC_OUT) > $(FINAL_REPORT)

clean:
	rm -f $(FINAL_REPORT) $(DYNAMIC_OUT) $(SHARED_OUT) $(STATIC_OUT)