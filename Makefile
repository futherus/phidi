export CXX      := g++
export CXXFLAGS ?=  -O2 -g -std=c++17 -fmax-errors=100 -Wall -Wextra  	    \
				-Weffc++ -Waggressive-loop-optimizations 	   					\
				-Wcast-align -Wcast-qual 	   					\
				-Wchar-subscripts -Wconditionally-supported -Wconversion        				\
				-Wctor-dtor-privacy -Wempty-body -Wfloat-equal 		   						\
				-Wformat-nonliteral -Wformat-security -Wformat-signedness       				\
				-Wformat=2 -Wlarger-than=8192 -Wlogical-op 	           				\
				-Wmissing-declarations -Wnon-virtual-dtor -Wopenmp-simd 	   					\
				-Woverloaded-virtual -Wpacked -Wpointer-arith -Wredundant-decls 				\
				-Wshadow -Wsign-conversion -Wsign-promo -Wstack-usage=8192      				\
				-Wstrict-null-sentinel -Wstrict-overflow=2 			   						\
				-Wsuggest-attribute=noreturn -Wsuggest-final-methods 	   					\
				-Wsuggest-final-types -Wsuggest-override -Wswitch-default 	   				\
				-Wsync-nand -Wundef -Wunreachable-code -Wunused   				\
				-Wuseless-cast -Wvariadic-macros -Wno-literal-suffix 	   					\
				-Wno-missing-field-initializers -Wno-narrowing 	           					\
				-Wno-old-style-cast -Wno-varargs -fcheck-new 		   						\
				-fsized-deallocation -fstack-check -fstack-protector            				\
				-fstrict-overflow -flto-odr-type-merging 	   		   						\
				-fno-omit-frame-pointer                                         				\
				# -fsanitize=address 	                                           				\
				# -fsanitize=alignment                                            				\
				# -fsanitize=bool                                                 				\
				# -fsanitize=bounds                                               				\
				# -fsanitize=enum                                                 				\
				# -fsanitize=float-cast-overflow 	                           					\
				# -fsanitize=float-divide-by-zero 			           							\
				# -fsanitize=integer-divide-by-zero                               				\
				# -fsanitize=leak 	                                           					\
				# -fsanitize=nonnull-attribute                                    				\
				# -fsanitize=null 	                                           					\
				# -fsanitize=object-size                                          				\
				# -fsanitize=return 		                                   					\
				# -fsanitize=returns-nonnull-attribute                            				\
				# -fsanitize=shift                                                				\
				# -fsanitize=signed-integer-overflow                              				\
				# -fsanitize=undefined                                            				\
				# -fsanitize=unreachable                                          				\
				# -fsanitize=vla-bound                                            				\
				# -fsanitize=vptr                                                 				\
				# -fPIE                                                           				\
				# -lm -pie
# Make shure to define rule for each target
SHELL := bash

CXX := g++
CXXFLAGS := -Wall
include txx.mk

all: out init.so toolbrush.so tool_pallette.so tools.so
# Libraries and headers
LIBS := sfml-system sfml-window sfml-graphics
INCLUDE := libs
>>>>>>> Stashed changes

# Determine the object files
OBJ :=

# Use submodules instead of recursion
MODULES := lot
# Include the description for each module
include $(patsubst %, %/module.mk, $(MODULES))

LDFLAGS  += $(addprefix -l, $(LIBS))
CXXFLAGS += $(addprefix -I, $(INCLUDE))

# Target specific object files
lot: $(OBJ) lot/main.o
	$(CXX) -o lot.exe lot/main.o $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGETS)
	rm -rf $(addsuffix /*.o, $(MODULES))
	rm -rf $(addsuffix /*.d, $(MODULES))

.PHONY: clean $(TARGETS)

# Dependencies
include $(OBJ:.o=.d)

%.d: %.cc
	./depend.sh `dirname $*` $(CXXFLAGS) $< > $@

