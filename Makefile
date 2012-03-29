scrabble: scrabble.c trie.c movegen.c lexicon.c board.c wordlist.c
	gcc -Wall scrabble.c trie.c movegen.c lexicon.c board.c wordlist.c -o scrabble

clean:
	rm -f scrabble
