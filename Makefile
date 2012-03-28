scrabble: scrabble.c trie.c
	gcc -Wall scrabble.c trie.c -o scrabble

clean:
	rm -f scrabble
