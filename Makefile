scrabble: main.c trie.c
	gcc -Wall main.c trie.c -o scrabble

clean:
	rm -f scrabble
