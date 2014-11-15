%.o: %.c
	@echo "CC     $^"
	@$(CC) $(CFLAGS) -c $^
