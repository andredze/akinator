CXX = g++

CXXFLAGS =  -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef 							\
			-Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations 						\
			-Wmissing-include-dirs -Wswitch-default -Weffc++ 										\
			-Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual -Wconversion 					\
			-Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers 		\
			-Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual	\
			-Wpointer-arith -Wsign-promo -Wstack-usage=8192 -Wstrict-aliasing 						\
			-Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG 				\
			-D_EJUDGE_CLIENT_SIDE \
			-Iinclude -Iinclude/tree

SOURCES = src/main.cpp \
		  src/akinator/ak_runprogram.cpp \
		  src/akinator/ak_addword.cpp \
		  src/akinator/ak_describe.cpp \
		  src/akinator/ak_compare.cpp \
		  src/akinator/ak_guess.cpp \
		  src/data_access/data_read.cpp \
		  src/data_access/data_write.cpp \
		  src/tree/tree_commands.cpp \
		  src/stack.cpp \
		  src/colors.cpp \
		  src/speak.cpp

ifdef DEBUG
CXXFLAGS += -DTREE_DEBUG
SOURCES  += src/tree/tree_debug.cpp
endif

EXECUTABLE = run

LOG_DIR = log/*

all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) $(LOG_DIR)
