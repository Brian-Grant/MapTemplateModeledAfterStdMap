
BUILDID=$(shell date +%m/%d/%Y-%H:%M:%S)

TEMPLATE_DIR=Template

TEMPLATE_FILES=$(TEMPLATE_DIR)/ConstIterator_Template.hpp		\
		$(TEMPLATE_DIR)/Iterator_Template.hpp					\
		$(TEMPLATE_DIR)/ReverseIterator_Template.hpp			\
		$(TEMPLATE_DIR)/FreeFunctions_Template.hpp				\
		$(TEMPLATE_DIR)/Map_Template.hpp						\
		$(TEMPLATE_DIR)/SkipList_Template.hpp					\
		$(TEMPLATE_DIR)/Map.hpp

TEMPLATE_TESTS_DIR=tests

TESTS=$(TEMPLATE_TESTS_DIR)/morseex.cpp 		\
		$(TEMPLATE_TESTS_DIR)/test-kec.cpp		\
		$(TEMPLATE_TESTS_DIR)/minimal.cpp 		\
		$(TEMPLATE_TESTS_DIR)/test.cpp 			\
		$(TEMPLATE_TESTS_DIR)/test-scaling.cpp

TEST_BUILD_RULES=t_minimal 		\
				t_morseex 		\
				t_test 			\
				t_test-kec 		\
				t_test-scaling


MY_TEST_DIR=mytests


SKIP_TEST_DIR=skip_tests

SKIP_TEST=$(SKIP_TEST_DIR)/skip_test.cpp				\
		$(SKIP_TEST_DIR)/skip_test_template.hpp		\
		$(SKIP_TEST_DIR)/skip_test.hpp				\
		$(SKIP_TEST_DIR)/test.sh

SKIP_TEST_DEP=$(SKIP_TEST_DIR)/skip_test.cpp				\
		$(SKIP_TEST_DIR)/skip_test_template.hpp		\
		$(SKIP_TEST_DIR)/skip_test.hpp

RECIPES=~/cs/recipes.txt

DIR=$(shell pwd)

OUTPUT_DIR=$(DIR)/out


TESTS_INCLUDE=-I$(TEMPLATE_DIR)

all: tests


mytest: clean $(MY_TEST_DIR)/mytest.cpp 
	@printf "\n\n-----------------------------------$(BUILDID)\n\n\n"
	g++ -std=c++1z -g -Wall -Wextra -pedantic $(TESTS_INCLUDE) -o mytest \
													 $(MY_TEST_DIR)/mytest.cpp

run: mytest
	./mytest >$(OUTPUT_DIR)/debug-mytest.txt 2>$(OUTPUT_DIR)/error-mytest.txt

val: t_test-scaling
	#valgrind -v --leak-check=full --show-leak-kinds=all ./mytest
	valgrind -v --track-origins=yes --leak-check=full \
			--show-leak-kinds=all ./t_test-kec -i 1000000 

gdb: mytest
	gdb mytest


# provided tests ------------------------------------------------------------

tests: $(TEST_BUILD_RULES)

# THIS IS BAD DESIGN
t_minimal: 
	g++ -std=c++1z -O2 -Wall -Wextra -pedantic $(TESTS_INCLUDE) \
					-o t_minimal $(TEMPLATE_TESTS_DIR)/minimal.cpp

t_morseex: 
	g++ -std=c++1z -O2 -Wall -Wextra -pedantic $(TESTS_INCLUDE) \
					-o t_morseex $(TEMPLATE_TESTS_DIR)/morseex.cpp

t_test: 
	g++ -std=c++1z -O2 -Wall -Wextra -pedantic $(TESTS_INCLUDE) \
						-o t_test $(TEMPLATE_TESTS_DIR)/test.cpp

t_test-kec:
	g++ -std=c++1z -O2 -Wall -Wextra -pedantic $(TESTS_INCLUDE) \
				-o t_test-kec $(TEMPLATE_TESTS_DIR)/test-kec.cpp

t_test-scaling:
	g++ -std=c++1z -O2 -Wall -Wextra -pedantic $(TESTS_INCLUDE) \
		-o t_test-scaling $(TEMPLATE_TESTS_DIR)/test-scaling.cpp

runtests:
	./t_minimal 
	#./t_morseex 
	./t_test #> deb.txt 2>deberr.txt
	./t_test-kec -i 1000000 #-p > deb.txt 2>deberr.txt
	./t_test-scaling > scaling.txt


# doubly linked circular list-------------------------------------------------

ll: misc/doubly_linked_circular_list.cpp
	g++ -std=c++1z -g -Wall -Wextra -pedantic -o d_c_ll \
		misc/doubly_linked_circular_list.cpp

runll: ll
	./d_c_ll

# skip_list tests ------------------------------------------------------------
low=0
high=1000
print=1
seed=00


low_script=0
high_script=10000

skip_test: $(SKIP_TEST_DEP)
	@printf "\n\n-----------------------------------$(BUILDID)\n\n\n"	
	g++ -std=c++1z -g -Wall -Wextra -pedantic -o skip_test \
		$(SKIP_TEST_DIR)/skip_test.cpp


skipt: skip_test
	#- ./skip_test $(low) $(high) $(print) $(seed) \
	./skip_test $(low) $(high) $(print) \
				>$(OUTPUT_DIR)/debug-$(low)-$(high)-$(seed).txt \
				2>$(OUTPUT_DIR)/error.txt
	rm $(OUTPUT_DIR)/error.txt

skipts: skip_test
	- ./$(SKIP_TEST_DIR)/test.sh $(low_script) $(high_script)
	- grep -a -B 1 'Seg' $(OUTPUT_DIR)/master_out.txt >$(OUTPUT_DIR)/tmp.txt
	- grep -a -x -E '[[:digit:]]+' $(OUTPUT_DIR)/tmp.txt \
										>$(OUTPUT_DIR)/seg-$(low)-$(high).txt
	rm $(OUTPUT_DIR)/master_out.txt $(OUTPUT_DIR)/tmp.txt


print2=0
t1: skip_test
	./skip_test 0 30 40 > $(OUTPUT_DIR)/1.txt

t2: skip_test
	./skip_test 0 30 40 > $(OUTPUT_DIR)/2.txt

d: skip_test
	diff $(OUTPUT_DIR)\1.txt $(OUTPUT_DIR)/2.txt

valt: skip_test
	valgrind -v --track-origins=yes --leak-check=full \
			--show-leak-kinds=all ./skip_test $(low) $(high) $(print2) $(seed)

gdbt: skip_test
	gdb --args skip_test $(low) $(high) $(print2) $(seed)

# open rules ----------------------------------------------------------------
recipes: $(RECIPES)
	gedit $(RECIPES) &


open: openmap
	gedit mytest.cpp &
	#$(win)
	gedit makefile &
	#$(win)

openmap: $(TEMPLATE_FILES)
	#gedit $^ &
	gedit $(TEMPLATE_FILES) &


opentests: $(TESTS)
	gedit $^ &

openskip:
	#gedit $^ &
	gedit $(SKIP_TEST) makefile &


# git ----------------------------------------------------------------------

ig: .gitignore
	vi .gitignore

git: clean cleantxt
	git add -A .
	git commit -m "commit on $(BUILDID)"
	git push

git-%: clean cleantxt
	git add -A
	git commit -m "$(@:git-%=%)  $(BUILDID)"
	git push origin master

# clean --------------------------------------------------------------------

clean:
	@rm -f *.o mytest d_c_ll skip_test t_minimal t_morseex t_test \
		t_test-kec t_test-scaling

cleantxt:
	rm -f $(OUTPUT_DIR)/*.txt
	rm -f *.txt
	touch $(OUTPUT_DIR)/dummy.txt
