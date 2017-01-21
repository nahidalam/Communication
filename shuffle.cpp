/*.

1) create a deck of cards, and print/display the cards
2) shuffle the deck of cards, and display the cards (in their shuffled order)
3) reshuffle the deck, and display the cards (in their shuffled order)
4) look for any 2-card sequence that was also present in the previous shuffle. If any are found, go back to step 3
5) when two back to back shuffles do not contain a duplicate sequence, you're done.
*/


#include <iostream>
#include <cstdlib>
#include <string>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <typeinfo>


using namespace std;


class Card { 
    
public:
    
    Card ();
    Card (string face, string suit);
    
    string print () const; 
    string getSuit () const;
    string getFace () const;
    
private: 
    
    string face;
    string suit;

};

Card::Card() {

}

Card :: Card (string face, string suit) {
    this->face = face;
    this->suit = suit;
    
}

string Card :: print () const{
    return (face + "-" + suit);
}


string Card :: getSuit () const {
    return suit;
}

string Card :: getFace () const {
    return face;
}

class DeckofCards { 
    
public: 
    
    DeckofCards ();
    DeckofCards (int size, string faces[], string suits[]);
    
    void printDeck ();
    void shuffle ();
    std::set<vector<Card>> createSet (int n);
    
private: 

    std::vector<Card> cards; 

};

DeckofCards :: DeckofCards () {
    
}

DeckofCards :: DeckofCards (int size, string faces[], string suits[]) {
    
    int DECK_SIZE = size;
    std::vector<Card>::iterator it;

    //create the original deck - inorder
    for (int i=0;i<DECK_SIZE;i++){
        it = cards.insert(it, Card (faces[i%13], suits [i/13]));
    }
}

void DeckofCards :: shuffle () {
    for (int i=cards.size()-1; i>=1; i--) {
        int j = (rand () +time (0)) % i;
        Card temp = cards[i];
        cards[i]=cards[j];
        cards[j]=temp;
    }
}

void DeckofCards :: printDeck () {
    for (int i=0; i< cards.size(); i++) {
        cout << cards[i].print();
        cout << "|";
        if ((i+1) %13 == 0)
            cout << endl;
    }

}

std::set<vector<Card>> DeckofCards :: createSet (int n) {
    std::set<vector<Card>> s;
    std::vector<Card> currentVector;

    for (int j=0;j<n;j++){
        currentVector.push_back(cards[j]);
    }
    s.insert(currentVector);
  
    
    for(int i=n; i<cards.size(); i++) {
        currentVector.erase(currentVector.begin());
        currentVector.push_back(cards[i]);
        s.insert(currentVector);    
            
    } 
    return s;
    
}



inline bool operator< (const Card& lhs, const Card& rhs) {
    
    string lhsSuit = lhs.getSuit();
    string rhsSuit = rhs.getSuit();
    string lhsFace = lhs.getFace();
    string rhsFace = rhs.getFace();
    
    if (lhsSuit.compare (rhsSuit) < 0){
        return true;
    }
    else if (lhsSuit.compare (rhsSuit) == 0) {
        if (lhsFace.compare (rhsFace)) {
            return true;
        }
    }
    return false;
}

int main () {
    
    int SEQ = 2;
    std::set<vector<Card>> prevSet;
    string faces [] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    string suits [] = {"Spades", "Clubs", "Hearts", "Diamonds"};
    
    DeckofCards deck (52, faces, suits);
    
    printf ("Display the original deck:\n\n");
    deck.printDeck ();

    printf ("\nAfter Shuffle:\n");
    deck.shuffle();

    prevSet = deck.createSet (SEQ);  
    deck.printDeck();
    
    
    while (true) {
        std::vector<vector<Card>> v_intersection;
    
        deck.shuffle();
        printf ("\nReshuffled:\n");
        deck.printDeck();

        std::set<vector<Card>> currentSet = deck.createSet (SEQ); 

        std::set_intersection(prevSet.begin(), prevSet.end(), currentSet.begin(), currentSet.end(),std::back_inserter(v_intersection));
        
        if (v_intersection.size() == 0) {
            cout << "\nAll good\n";
            break;
            
        }
        prevSet = currentSet;
        
    }
    
    return 0;
}
