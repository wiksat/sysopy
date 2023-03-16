PROGRAM_STATICO = ../zad2/replStaticO
PROGRAM_SHAREDO = ../zad2/replSharedO
PROGRAM_DYNAMICO = ../zad2/replDynamicO

PROGRAM_STATICO2 = ../zad2/replStaticO2
PROGRAM_SHAREDO2 = ../zad2/replSharedO2
PROGRAM_DYNAMICO2 = ../zad2/replDynamicO2

PROGRAM_STATICOs = ../zad2/replStaticOs
PROGRAM_SHAREDOs = ../zad2/replSharedOs
PROGRAM_DYNAMICOs = ../zad2/replDynamicOs

STATIC_OUTO = result_static_O.txt
SHARED_OUTO = result_shared_O.txt
DYNAMIC_OUTO = result_dynamic_O.txt

STATIC_OUTO2 = result_static_O2.txt
SHARED_OUTO2 = result_shared_O2.txt
DYNAMIC_OUTO2 = result_dynamic_O2.txt

STATIC_OUTOs = result_static_Os.txt
SHARED_OUTOs = result_shared_Os.txt
DYNAMIC_OUTOs = result_dynamic_Os.txt

FINAL_REPORT = report.txt

INPUT = input.txt
all: test_static test_shared test_dynamic concatenate
test_static:
	echo "staticO:" > $(STATIC_OUTO)
	cat $(INPUT) | $(PROGRAM_STATICO) >> $(STATIC_OUTO)
	echo "staticO2:" > $(STATIC_OUTO2)
	cat $(INPUT) | $(PROGRAM_STATICO2) >> $(STATIC_OUTO2)
	echo "staticOs:" > $(STATIC_OUTOs)
	cat $(INPUT) | $(PROGRAM_STATICOs) >> $(STATIC_OUTOs)

test_shared:
	echo "sharedO:" > $(SHARED_OUTO)
	cat $(INPUT) | $(PROGRAM_SHAREDO) >> $(SHARED_OUTO)
	echo "sharedO2:" > $(SHARED_OUTO2)
	cat $(INPUT) | $(PROGRAM_SHAREDO2) >> $(SHARED_OUTO2)
	echo "sharedOs:" > $(SHARED_OUTOs)
	cat $(INPUT) | $(PROGRAM_SHAREDOs) >> $(SHARED_OUTOs)

test_dynamic:
	echo "dynamicO:" > $(DYNAMIC_OUTO)
	cat $(INPUT) | $(PROGRAM_DYNAMICO) >> $(DYNAMIC_OUTO)
	echo "dynamicO2:" > $(DYNAMIC_OUTO2)
	cat $(INPUT) | $(PROGRAM_DYNAMICO2) >> $(DYNAMIC_OUTO2)
	echo "dynamicOs:" > $(DYNAMIC_OUTOs)
	cat $(INPUT) | $(PROGRAM_DYNAMICOs) >> $(DYNAMIC_OUTOs)

concatenate:
	cat $(STATIC_OUTO) $(STATIC_OUTO2) $(STATIC_OUTOs) $(SHARED_OUTO) $(SHARED_OUTO2) $(SHARED_OUTOs) $(DYNAMIC_OUTO) $(DYNAMIC_OUTO2) $(DYNAMIC_OUTOs) > $(FINAL_REPORT)

clean:
	rm -f $(FINAL_REPORT) $(DYNAMIC_OUTO) $(SHARED_OUTO) $(STATIC_OUTO) $(DYNAMIC_OUTO2) $(SHARED_OUTO2) $(STATIC_OUTO2) $(DYNAMIC_OUTOs) $(SHARED_OUTOs) $(STATIC_OUTOs)