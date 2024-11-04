#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

using namespace std;


#define RESET "\033[0m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define BLUE "\033[34m"

struct GameData {
    string question;
    string word;
};


void loadQuestions(vector<GameData>& questions) {
    ifstream file("questions.txt");
    if (!file) {
        cerr << RED << "Error: Could not open questions.txt. Make sure the file is in the correct directory." << RESET << endl;
        return;
    }
    
    GameData data;
    int lineCount = 0;
    while (getline(file, data.question) && getline(file, data.word)) {
        questions.push_back(data);
        lineCount += 2;
    }
    file.close();
    
    if (questions.empty()) {
        cerr << RED << "Error: No questions loaded from questions.txt. Ensure the file is not empty and has questions in the correct format." << RESET << endl;
    } else {
        cout << GREEN << "Successfully loaded " << questions.size() << " questions from questions.txt." << RESET << endl;
    }
}


void displayWord(const string& word, const vector<bool>& revealed) {
    cout << CYAN;
    for (size_t i = 0; i < word.length(); i++) {
        if (revealed[i]) {
            cout << word[i] << " ";
        } else {
            cout << "_ ";
        }
    }
    cout << RESET << endl;
}


bool guessLetter(const string& word, vector<bool>& revealed, char letter) {
    bool correct = false;
    for (size_t i = 0; i < word.length(); i++) {
        if (tolower(word[i]) == tolower(letter) && !revealed[i]) {
            revealed[i] = true;
            correct = true;
        }
    }
    return correct;
}


void revealHint(const string& word, vector<bool>& revealed) {
    for (size_t i = 0; i < word.length(); i++) {
        if (!revealed[i]) {
            revealed[i] = true;
            break;
        }
    }
}


bool isWordRevealed(const vector<bool>& revealed) {
    for (bool r : revealed) {
        if (!r) return false;
    }
    return true;
}

// Main game logic
int main() {
    vector<GameData> questions;
    loadQuestions(questions);
    
    if (questions.empty()) {
        cout << RED << "No questions available for the game!" << RESET << endl;
        return 1;
    }
    
    srand(static_cast<unsigned>(time(0)));
    GameData& game = questions[rand() % questions.size()]; 
    
    cout << YELLOW << "Question: " << game.question << RESET << endl;
    vector<bool> revealed(game.word.length(), false);
    int hintCount = 0;
    time_t startTime = time(0);
    
    while (!isWordRevealed(revealed)) {
        cout << "\nCurrent word state: ";
        displayWord(game.word, revealed);
        
        cout << BLUE << "Choose an action:\n1. Guess a letter\n2. Use a hint\n3. Guess the word\nEnter choice: " << RESET;
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            cout << "Enter a letter: ";
            char letter;
            cin >> letter;
            if (!guessLetter(game.word, revealed, letter)) {
                cout << RED << "Incorrect letter!" << RESET << endl;
            } else {
                cout << GREEN << "Correct!" << RESET << endl;
            }
        } else if (choice == 2) {
            revealHint(game.word, revealed);
            hintCount++;
            cout << YELLOW << "Hint used!" << RESET << endl;
        } else if (choice == 3) {
            cout << "Enter the word: ";
            string guess;
            cin >> guess;
            if (guess == game.word) {
                cout << GREEN << "You guessed the word!" << RESET << endl;
                break;
            } else {
                cout << RED << "Incorrect word!" << RESET << endl;
            }
        } else {
            cout << RED << "Invalid choice, try again." << RESET << endl;
        }
    }
    
    time_t endTime = time(0);
    int totalTime = static_cast<int>(endTime - startTime) + hintCount * 2;
    cout << CYAN << "\nCongratulations! You revealed the word: " << game.word << RESET << endl;
    cout << YELLOW << "Total time: " << totalTime << " seconds (including hints)" << RESET << endl;

    return 0;
}
