ASM=nasm
ASMFLAGS=-f elf64

%.o:	%.asm
	$(ASM) $(ASMFLAGS) -o $@ $<
dict.o: lib.inc
	$(ASM) $(ASMFLAGS) -o  $@  dict.asm
main.o:	lib.inc dict.inc words.inc
	$(ASM) $(ASMFLAGS) -o  $@  main.asm
main:	lib.o dict.o main.o
	ld -o  $@  $^
.PHONY: clean
clean: 
	rm *.o program
 Tests := "Yatoro" "Larl" "Miposhka" "Arthas" "256aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
.PHONY: test
test:
	@i=1; \
        for word in $(Tests); do \
                result=$$(echo $$word | ./main 2>&1); \
                echo "Test $$i: message: $$word result: $$result"; \
                i=$$((i+1)); \
        done;
