#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <utility>
using namespace std;

struct Card {
  string value;
  string suit;
};

string printCard(Card c) { return (c.value + c.suit); }

struct Deck {
  int currentCard = 0;
  Card card[52];
};

// TODO: replace the cards suits in the deck with the symbols
// METHOD 1 - use a function that swaps out the value of the card suit.
Card suitReplace(Card c) {
  if (c.suit == "S") {
    c.suit = "♠";
  } else if (c.suit == "H") {
    c.suit = "♥";
  } else if (c.suit == "C") {
    c.suit = "♣";
  } else if (c.suit == "D") {
    c.suit = "♦";
  } else {
    cout << "ERROR: invalid suit value in card: " << c.value << c.suit << endl;
  }
  return c;
}

Deck create_deck() {
  const int NUMCARDS = 52;
  const int CARDSIZE = 2;
  Deck deck;
  char suits[] = {'C', 'D', 'H', 'S'};
  char cardValues[] = {'A', 'K', 'Q', 'J', 'T', '9', '8',
                       '7', '6', '5', '4', '3', '2'};
  int count = 0;
  Card current;
  // for each suit
  for (int n = 0; n < 4; n++) {
    ;
    // for each of the unique card calues
    for (int m = 0; m < 13; m++) {

      current.suit = suits[n];
      current.value = cardValues[m];
      current = suitReplace(current);
      deck.card[count] = current;
      count++;
    }
  }
  if (count == 52) {
    cout << "deck size OK..." << endl;
  } else {
    cout << "deck size: " << count << ", ERROR...exiting." << endl;
    exit(-1);
  }
  return deck;
}

struct Hand {
  string owner;
  int numCards = 0;
  // max possible hand is A,A,A,A,2,2,2,2,3,3,3
  Card card[12];
};

Hand createHand(string owner) {
  Hand h;
  h.owner = owner;
  return h;
}

void printHand(Hand h) {
  // handarr[2] - value;
  string s;
  string d;
  cout << "\n" << h.owner << "'s hand:";
  for (int i = 0; i < h.numCards; i++) {
    s += (" " + printCard(h.card[i]));
  }
  cout << s << endl;
  if (h.owner == "Dealer") {
    cout << " ?? " << endl;
    for (int t = 1; t < h.numCards; t++) {
      d += (" " + printCard(h.card[t]));
    }
    cout << d << endl;
  }
}
// TODO: create a version of the above to print the dealer's hand, with the
// first (face-down) card obscured by ?? This is easier than it may first
// appear.  Examine the function carefully.

Hand addCard(Hand h, Card c) {
  // h.numCards is the last element in the Hand's Card array.
  // Don't attempt to add past element 11 (0-11 is 12 items).
  if (h.numCards < 11) {
    h.card[h.numCards] = c;
    // the hand is one larger.
    h.numCards++;
    //  cout << "DEBUG: " << h.owner << "'s hand size: " << h.numCards << endl;
  } else {
    cout << "Max amount of cards in hand! You can no longer hit.\n";
  }
  return h;
}

Hand dealCard(Hand h, Deck d) {
  int i = d.currentCard;

  // cout << h.owner << "'s hand pre-deal: ";
  // printHand(h);
  // cout << "DEBUG: About to deal: " << d.card[i].value << d.card[i].suit <<
  // endl;
  h = addCard(h, d.card[d.currentCard]);

  cout << "DEBUG: Current hand:";
  printHand(h);
  //  cout << "DEBUG: Next deck card: " << d.card[i + 1].value << endl;

  return h;
}

int eval(Hand h) {
  int score = 0;
  int aceCount = 0;

  map<string, int> cardMap = {
      {"A", 11}, {"K", 10}, {"Q", 10}, {"J", 10}, {"T", 10}, {"9", 9}, {"8", 8},
      {"7", 7},  {"6", 6},  {"5", 5},  {"4", 4},  {"3", 3},  {"2", 2}};

  // DEBUG: Print the cardVal map -- comment this out in your program
  // cout << "\nThe map is:\n";
  // cout << "KEY\tVALUE\n";
  /**
  for (const auto &[key, value] : cardMap) {
    cout << key << ":\t" << value << " points\n";
  }
**/

  printHand(h);

  // for each card in the hand, add the score:
  for (int n = 0; n < h.numCards; n++) {

    // cout << "DEBUG: Looking for " << h.card[n].value << endl;
    auto search = cardMap.find(h.card[n].value);
    if (h.card[n].value == "A") {
      aceCount++;
    }
    if (search != cardMap.end()) {
      // cout << "Found key: " << search->first << ", value: " << search->second
      //     << '\n';
      score += static_cast<int>(search->second);
    } else {
      cout << "ERROR: key not found\n";
    }
  }
  if (score > 21 && aceCount > 0) {
    while (score > 21) {
      score -= 10;
      aceCount--;
    }
  }
  cout << "Total hand score is: " << score << endl;
  return score;
}

void printDeck(Deck d) {
  for (int i = 0; i < 52; i++) {
    cout << d.card[i].value << d.card[i].suit;
    if (i != 51) {
      cout << ',';
    }
  }
  cout << endl;
  return;
}
// TODO: shuffle the deck
Deck shuffleDeck(Deck d) {
  Card temp;
  int sindex = 0;
  srand(time(0));
  for (int i = 0; i < 52; i++) {
    sindex = rand() % 52;
    temp = d.card[i];
    d.card[i] = d.card[sindex];
    d.card[sindex] = temp;
  }
  // DEBUG:
  // cout << "Shuffled deck: ", printDeck() << endl;
  // cout << "first item: " << deck[0] << endl;
  return d;
}

// === MAIN ===
int main() {
  const int NUMCARDS = 52;
  const int CARDSIZE = 2;
  Deck deck;

  char answer;
  char hitStand;
  bool p2 = false;
  bool p3 = false;
  bool p1BlackJack = false;
  bool p2BlackJack = false;
  bool p3BlackJack = false;
  bool dBlackJack = false;
  int gameCount = 0;
  int p1Wins = 0;
  int p2Wins = 0;
  int p3Wins = 0;
  int dWins = 0;
  int p1Losses = 0;
  int p2Losses = 0;
  int p3Losses = 0;
  int dLosses = 0;
  int p1Ties = 0;
  int p2Ties = 0;
  int p3Ties = 0;
  int dTies = 0;
  Hand dealerHand;
  Hand player1Hand;
  Hand player2Hand;
  Hand player3Hand;
  int p1HandVal;
  int p2HandVal;
  int p3HandVal;
  int dHandVal;

  do {
    gameCount++;
    // There are zero cards when you declare a new Hand.
    dealerHand = createHand("Dealer");
    player1Hand = createHand("Player 1");
    player2Hand = createHand("Player 2");
    player3Hand = createHand("Player 3");
    deck = create_deck();
    printDeck(deck);

    // Replace the card letters with symbols
    // Method 1: via function

    cout << endl;
    // DEBUG: deck validation.
    deck = shuffleDeck(deck);
    printDeck(deck);

    cout << "Player 2? (y/n) ";
    cin >> answer;
    if (answer == 'y') {
      p2 = true;
    }
    cout << "Player 3? (y/n) ";
    cin >> answer;
    if (answer == 'y') {
      p3 = true;
    }

    // TODO: (optional) replace card letters with symbols
    //  Method 2: don't do method 1 above, make the deck in create_deck using
    //  the suit symbols Example: string suitSymbols[] = {"♣", "♦", "♥", "♠"};
    //  for (auto n : suitSymbols){ // the initializer may be an array
    //  	cout << n << endl;
    //  }
    //  cout << endl;

    // TODO: Deal cards out from the deck to make hands

    // cout << "\nDEBUG: current card: " << deck.currentCard << endl;

    // The player gets dealt the first card.
    player1Hand = dealCard(player1Hand, deck);
    deck.currentCard++;
    if (p2) {
      player2Hand = dealCard(player2Hand, deck);
      deck.currentCard++;
    }
    if (p3) {
      player3Hand = dealCard(player3Hand, deck);
      deck.currentCard++;
    }
    // increment the position (how many cards deep in the deck)
    // cout << "DEBUG: back in main() from dealCard." << endl;
    // printHand(player1Hand);
    // cout << "DEBUG: Current number of cards dealt (deck list index): " <<
    // deck.currentCard << endl;

    dealerHand = dealCard(dealerHand, deck);
    // increment the position (how many cards deep in the deck)
    deck.currentCard++;
    // cout << "DEBUG: back in main() from dealCard." << endl;
    // printHand(dealerHand);
    // cout << "DEBUG: Current number of cards dealt (deck list index): " <<
    // deck.currentCard << endl;

    // The player gets dealt the first card.
    player1Hand = dealCard(player1Hand, deck);
    // increment the position (how many cards deep in the deck)
    deck.currentCard++;
    // cout << "DEBUG: back in main() from dealCard." << endl;
    // printHand(player1Hand);
    // cout << "DEBUG: Current number of cards dealt (deck list index): " <<
    // deck.currentCard << endl;
    if (p2) {
      player2Hand = dealCard(player2Hand, deck);
      deck.currentCard++;
    }
    if (p3) {
      player3Hand = dealCard(player3Hand, deck);
      deck.currentCard++;
    }
    dealerHand = dealCard(dealerHand, deck);
    // increment the position (how many cards deep in the deck)
    deck.currentCard++;
    // cout << "DEBUG: back in main() from dealCard." << endl;
    // printHand(dealerHand);
    // cout << "DEBUG: Current number of cards dealt (deck list index): " <<
    // deck.currentCard << endl;

    // TODO: print out hand of all players

    printHand(player1Hand);
    printHand(player2Hand);
    printHand(player3Hand);
    printHand(dealerHand);

    // cout << "Player1 Hand: " << player1Hand << endl;
    // cout << "Player2 Hand: " << player2Hand << endl;
    // cout << "Player3 Hand: " << player3Hand << endl;

    //  print out the dealer's hand, hiding the hole card:

    // cout << "dealer cards: ?? "
    //      << dealerHand.card[1].value + dealerHand.card[1].suit << endl;
    //  subsequent printing will require a function in the case that the dealer
    //  hits (on a hand < 16).

    //  cout << "DEBUG In main(): " << player1Hand.owner
    //       << "'s hand value: " << eval(player1Hand) << endl;
    //  cout << "DEBUG In main(): " << dealerHand.owner
    //       << "'s hand value: " << eval(dealerHand) << endl;

    p1HandVal = eval(player1Hand);
    p2HandVal = eval(player2Hand);
    p3HandVal = eval(player3Hand);
    dHandVal = eval(dealerHand);

    if (p1HandVal == 21) {

      p1BlackJack = true;
    }
    if (p2HandVal == 21) {
      p2BlackJack = true;
    }
    if (p3HandVal == 21) {
      p3BlackJack = true;
    }
    if (dHandVal == 21) {
      dBlackJack = true;
    }
    if (dBlackJack) {
      dWins++;
      cout << "BLACKJACK! DEALER WINS!\n";
      p1Losses++;
      p2Losses++;
      p3Losses++;

    } else {
      if (p1BlackJack) {
        p1Wins++;
        cout << "BLACKJACK! PLAYER 1 WINS!\n";
      }
      if (p2BlackJack) {
        p2Wins++;
        cout << "BLACKJACK! PLAYER 2 WINS!\n";
      }
      if (p3BlackJack) {
        p3Wins++;
        cout << "BLACKJACK! PLAYER 3 WINS!\n";
      }
    }

    if (p1HandVal < 21) {
      do {
        cout << player1Hand.owner << "'s hand value: " << eval(player1Hand)
             << endl;
        cout << "Hit or Stand? (h/s): ";
        cin >> hitStand;
        if (hitStand == 'h') {

          player1Hand = dealCard(player1Hand, deck);
          deck.currentCard++;
          p1HandVal = eval(player1Hand);
        }
      } while (p1HandVal < 21 && hitStand != 's');
    }
    if (p1HandVal > 21) {
      cout << "BUST! YOU LOSE!\n";
      p1Losses++;
    }
    if (p2) {
      if (p2HandVal < 21) {
        do {
          cout << player2Hand.owner << "'s hand value: " << eval(player2Hand)
               << endl;
          cout << "Hit or Stand? (h/s): ";
          cin >> hitStand;
          if (hitStand == 'h') {

            player2Hand = dealCard(player2Hand, deck);
            deck.currentCard++;
            p2HandVal = eval(player2Hand);
          }
        } while (p2HandVal < 21 && hitStand != 's');
      }
      if (p2HandVal > 21) {
        cout << "BUST! YOU LOSE!\n";
        p2Losses++;
      }
    }
    if (p3) {
      if (p3HandVal < 21) {
        do {
          cout << player3Hand.owner << "'s hand value: " << eval(player3Hand)
               << endl;
          cout << "Hit or Stand? (h/s): ";
          cin >> hitStand;
          if (hitStand == 'h') {

            player3Hand = dealCard(player3Hand, deck);
            deck.currentCard++;
            p3HandVal = eval(player3Hand);
          }
        } while (p3HandVal < 21 && hitStand != 's');
      }
      if (p3HandVal > 21) {
        cout << "BUST! YOU LOSE!\n";
        p3Losses++;
      }
    }
    while (dHandVal < 17) {
      dealerHand = dealCard(dealerHand, deck);
      deck.currentCard++;
      dHandVal = eval(dealerHand);
      if (dHandVal > 21) {
        cout << "BUST! DEALER LOSES!\n";
        dLosses++;
      }
    }
    if (dHandVal <= 21 && dHandVal >= p1HandVal) {
      dWins++;
      p1Losses++;
      cout << dHandVal << " > " << p1HandVal << "\n"
           << "DEALER BEATS PLAYER 1!\n";
    } else if (dHandVal < p1HandVal && p1HandVal <= 21) {
      dLosses++;
      p1Wins++;
      cout << dHandVal << " < " << p1HandVal << "\n"
           << "PLAYER 1 BEATS DEALER!\n";
    }
    if (p2) {
      if (dHandVal <= 21 && dHandVal >= p2HandVal) {
        dWins++;
        p2Losses++;
        cout << dHandVal << " > " << p2HandVal << "\n"
             << "DEALER BEATS PLAYER 2!\n";
      } else if (dHandVal < p2HandVal && p2HandVal <= 21) {
        dLosses++;
        p2Wins++;
        cout << dHandVal << " < " << p2HandVal << "\n"
             << "PLAYER 2 BEATS DEALER!\n";
      }
    }
    if (p3) {
      if (dHandVal <= 21 && dHandVal >= p3HandVal) {
        dWins++;
        p3Losses++;
        cout << dHandVal << " > " << p3HandVal << "\n"
             << "DEALER BEATS PLAYER 3!\n";
      } else if (dHandVal < p3HandVal && p3HandVal <= 21) {
        dLosses++;
        p3Wins++;
        cout << dHandVal << " < " << p3HandVal << "\n"
             << "PLAYER 3 BEATS DEALER!\n";
      }
    }

    cout << "Play again? ";
    cin >> answer;
  } while (answer != 'n');

  cout << "RESULTS:\n";
  cout << "Games: " << gameCount << endl;
  cout << "PLAYER 1 WIN TOTAL: " << p1Wins << endl;
  cout << "PLAYER 1 LOSS TOTAL: " << p1Losses << endl;
  if (p2) {

    cout << "PLAYER 2 WIN TOTAL: " << p2Wins << endl;
    cout << "PLAYER 2 LOSS TOTAL: " << p2Losses << endl;
  }
  if (p3) {
    cout << "PLAYER 3 WIN TOTAL: " << p3Wins << endl;
    cout << "PLAYER 3 LOSS TOTAL: " << p3Losses << endl;
  }

  return 0;
}