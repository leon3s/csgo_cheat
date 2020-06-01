EXE := dko32.dll
CC := g++

EXE_DIR := bin
SRCS_DIR := srcs
OBJS_DIR := objs
INCLUDES_DIR = includes

SRCS := $(wildcard $(SRCS_DIR)/*.cpp)
OBJS := $(SRCS:$(SRCS_DIR)/%.cpp=$(OBJS_DIR)/%.o)

DX9_SDK = "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86"

CPPFLAGS := -I $(INCLUDES_DIR)
CFLAGS   := -Wall -std=c++14 -g
LDFLAGS  := -L $(DX9_SDK)
LDLIBS   := -ld3d9 -ld3dx9

.PHONY: all clean fclean re

all: install-env $(EXE)

re: fclean all

$(EXE): $(OBJS)
	$(CC) -shared $(LDFLAGS) $^ $(LDLIBS) -o $(EXE_DIR)/$@

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

install-env:
	@echo off
	IF exist $(EXE_DIR) ( echo $(EXE_DIR) exists ) ELSE ( mkdir $(EXE_DIR) && echo $(EXE_DIR) dir created )
	IF exist $(OBJS_DIR) ( echo $(OBJS_DIR) exists ) ELSE ( mkdir $(OBJS_DIR) && echo $(OBJS_DIR) dir created )


clean:
	del /f /Q $(OBJS_DIR)

fclean: clean
	del /f /Q $(EXE_DIR)
