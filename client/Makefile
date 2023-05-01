# name
CLIENT = uchat

EMPTY = 
SPACE = $(EMPTY) $(EMPTY)
VPATH = VPATH = $(subst $(SPACE), :, $(SRCD))

# client working directory

# #SSL_LIB =  /opt/homebrew/Cellar/openssl@1.1/1.1.1t

OBJD = obj

SSL_LIB = /opt/homebrew/Cellar/openssl@1.1/1.1.1t

OBJD = obj
INCD_LIBS = -I../libraries/jsonc/inc -I../libraries/libmx/inc -I../libraries/sqlite3/inc \
-I$(SSL_LIB)/include
LIBD_LIBS = -L../libraries/jsonc -L../libraries/libmx -L../libraries/sqlite3 \
-L$(SSL_LIB)/lib
INCD_SERVER = -Iinc/

GTK_LIBS = `pkg-config --libs gtk+-3.0`
GTK_INC = `pkg-config --cflags gtk+-3.0`

SRCD = src
INCD = inc
SRCS = $(foreach dir, $(SRCD), $(wildcard $(dir)/*.c))
OBJS = $(addprefix $(OBJD)/, $(notdir $(SRCS:%.c=%.o)))
INCS = $(foreach dir, $(INCD), $(wildcard $(dir)/*.h))

OBJTEST = $(OBJS)
#compilier
CC = clang

TEST_FLAG = -DMX_UNIT_TEST
CFLAGS = -std=c11 $(addprefix -W, all extra error pedantic \
no-unused-command-line-argument no-unused-variable no-unused-function no-unused-parameter \
no-incompatible-pointer-types-discards-qualifiers no-incompatible-pointer-types)
CPPFLAGS = $(INCD_CLIENT) -D_GNU_SOURCE \
$(GTK_INC) $(INCD_LIBS)
LIBS = $(GTK_LIBS) $(LIBD_LIBS) \
-lssl -ljsonc -lmx -lcrypto -lsqlite3 -lpthread -lgdk-3.0 -lgtk-3 -lgdk_pixbuf-2.0 \
-lpangocairo-1.0 -lpango-1.0 -lharfbuzz -latk-1.0 -lcairo-gobject -lcairo -lgio-2.0 -lgobject-2.0 -lglib-2.0

all: $(CLIENT)

unit_test: CPPFLAGS += $(TEST_FLAG)

unit_test: all

$(CLIENT): $(OBJS)
	@$(CC) -fsanitize=address -g -o $@ $^ $(LIBS)
	@printf "\033[32;1m$@ created\033[0m\n"

$(OBJS): obj/%.o: %.c $(INCS) | $(OBJD)
	@$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
	@printf "\033[32mcompiled: \033[0m$(notdir $<)\n"

$(OBJD):
	@mkdir -p $@
	@printf "\033[32;1m$@ created\033[0m\n"

	

uninstall: clean
	@rm -rf $(CLIENT)
	@printf "\033[34;1mdeleted $(CLIENT)\033[0m\n"

clean:
	@rm -rf $(OBJD)
	@printf "\033[34;1mdeleted $(OBJD)\033[0m\n"

reinstall: uninstall all

.PHONY: all uninstall clean reinstall
