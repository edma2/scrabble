scrabble: scrabble.c trie.c movegen.c
	gcc -Wall scrabble.c trie.c movegen.c -o scrabble

clean:
	rm -f scrabble
