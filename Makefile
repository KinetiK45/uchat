CLIENT = uchat
SERVER = uchat_server
MX = libmx
CJSON = libjsonc
SQLITE = libsqlite3

CLIENTD = client
SERVERD = server
MXD = libraries/libmx
CJSOND = libraries/jsonc
SQLITED = libraries/sqlite3

CLIENTO = $(CLIENTD)/$(CLIENT)
SERVERO = $(SERVERD)/$(SERVER)
MXO = $(MXD)/$(MX).a
CJSONO = libraries/lib/$(CJSON).a
SQLITEO = libraries/lib/$(SQLITE).a


all: $(SQLITE) $(CJSON) $(MX) $(CLIENT) $(SERVER)

$(SQLITE): $(SQLITEO)
$(CJSON): $(CJSONO)
$(MX): $(MXO)
$(CLIENT): $(CLIENTO)
$(SERVER): $(SERVERO)

$(SQLITEO): $(SQLITED)
$(CJSONO): $(CJSOND)
$(MXO): $(MXD)
$(CLIENTO): $(CLIENTD)
$(SERVERO): $(SERVERD)

$(CLIENTO) $(SERVERO):
	@make -sC $<
	@mv $@ .

$(SQLITEO) $(CJSONO) $(MXO): |
	@make -sC $<
	
install:
	brew install openssl gtk+3

uninstall: clean
	@make -sC $(SQLITED) $@
	@make -sC $(CJSOND) $@
	@make -sC $(MXD) $@
	@make -sC $(CLIENTD) $@
	@make -sC $(SERVERD) $@

clean:
	@rm -rf $(CLIENT)
	@rm -rf $(SERVER)
	@make -sC $(SQLITED) $@
	@make -sC $(CJSOND) $@
	@make -sC $(MXD) $@
	@make -sC $(CLIENTD) $@
	@make -sC $(SERVERD) $@

reinstall: uninstall all

.PHONY: + uninstall clean reinstall $(CLIENTO) $(SERVERO) $(SQLITEO) $(CJSONO) $(MXO)
