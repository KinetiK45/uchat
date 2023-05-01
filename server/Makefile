# name
SERVER = uchat_server

EMPTY = 
SPACE = $(EMPTY) $(EMPTY)
VPATH = VPATH = $(subst $(SPACE), :, $(SRCD))

# client working directory

#SSL_LIB = /opt/homebrew/Cellar/openssl@1.1/1.1.1t

SSL_LIB = /opt/homebrew/Cellar/openssl@1.1/1.1.1t

OBJD = obj
INCD_LIBS = -I../libraries/jsonc/inc -I../libraries/libmx/inc -I../libraries/sqlite3/inc \
-I$(SSL_LIB)/include
LIBD_LIBS = -L../libraries/jsonc -L../libraries/libmx -L../libraries/sqlite3 \
-L$(SSL_LIB)/lib
INCD_SERVER = -Iinc/

SRCD = src
INCD = inc
SRCS = $(foreach dir, $(SRCD), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJD)/, $(notdir $(SRCS:%.c=%.o)))
INCS = $(foreach dir, $(INCD), $(wildcard $(dir)/*.h))

#compilier
CC = clang

CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic \
no-unused-parameter)
CPPFLAGS = $(INCD_SERVER) -D_GNU_SOURCE $(INCD_LIBS)
LIBS = $(LIBD_LIBS) \
-lssl -lcrypto -ljsonc -lmx -lsqlite3 -lpthread -ldl

all: $(SERVER)

$(SERVER): $(OBJS)
	@$(CC) -fsanitize=address -g -o $@ $^ $(LIBS)
	@printf "\033[32;1m$@ created\033[0m\n"

$(OBJS): obj/%.o: %.c $(INCS) | $(OBJD)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -g -c -o $@ $<
	@printf "\033[32mcompiled: \033[0m$(notdir $<)\n"

$(OBJD):
	@mkdir -p $@
	@printf "\033[32;1m$@ created\033[0m\n"

uninstall: clean
	@rm -rf $(SERVER)
	@printf "\033[34;1mdeleted $(SERVER)\033[0m\n"

clean:
	@rm -rf $(OBJD)
	@printf "\033[34;1mdeleted $(OBJD)\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
