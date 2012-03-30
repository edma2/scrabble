SRC=src

scrabble: $(SRC)/scrabble.c $(SRC)/trie.c $(SRC)/movegen.c $(SRC)/lexicon.c $(SRC)/board.c $(SRC)/wordlist.c
	cd $(SRC); gcc -Wall scrabble.c trie.c movegen.c lexicon.c board.c wordlist.c -o ../scrabble

clean:
	rm -f scrabble
