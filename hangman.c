#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORDS 100
#define MAX_LENGTH 50
#define MAX_TRIES 6

// Function to draw hangman
void drawHangman(int wrongGuesses) {
    printf("\n");
    printf("  +---+\n");
    printf("  |   |\n");

    if (wrongGuesses >= 1)
        printf("  O   |\n");
    else
        printf("      |\n");

    if (wrongGuesses == 2)
        printf("  |   |\n");
    else if (wrongGuesses == 3)
        printf(" /|   |\n");
    else if (wrongGuesses >= 4)
        printf(" /|\\  |\n");
    else
        printf("      |\n");

    if (wrongGuesses == 5)
        printf(" /    |\n");
    else if (wrongGuesses >= 6)
        printf(" / \\  |\n");
    else
        printf("      |\n");

    printf("      |\n");
    printf("=========\n");
}

void playGame(char words[MAX_WORDS][MAX_LENGTH], int wordCount) {
    srand(time(NULL));
    int randomIndex = rand() % wordCount;

    char *selectedWord = words[randomIndex];
    int wordLength = strlen(selectedWord);

    char guessedWord[MAX_LENGTH];
    int wrongGuesses = 0;
    int correctGuesses = 0;
    int hintUsed = 0;

    // Initialize guessed word with underscores
    for (int i = 0; i < wordLength; i++) {
        guessedWord[i] = '_';
    }
    guessedWord[wordLength] = '\0';

    printf("\n===== HANGMAN GAME =====\n");

    while (wrongGuesses < MAX_TRIES) {
        printf("\nWord: %s\n", guessedWord);
        drawHangman(wrongGuesses);

        char guess;
        printf("Enter a letter (or '?' for a hint, '0' to exit): ");
        scanf(" %c", &guess);

        if (guess == '0') {  // manual exit
            printf("Exiting the game.\n");
            return;
        }

        if (guess == '?' && !hintUsed) { // provide one hint
            for (int i = 0; i < wordLength; i++) {
                if (guessedWord[i] == '_') {
                    guessedWord[i] = selectedWord[i];
                    printf("Hint: The letter '%c' is in the word.\n", selectedWord[i]);
                    hintUsed = 1;
                    correctGuesses++;
                    break;
                }
            }
            continue;
        } else if (guess == '?' && hintUsed) {
            printf("You already used your hint!\n");
            continue;
        }

        int found = 0;

        for (int i = 0; i < wordLength; i++) {
            if (selectedWord[i] == guess && guessedWord[i] == '_') {
                guessedWord[i] = guess;
                found = 1;
                correctGuesses++;
            }
        }

        if (!found) {
            wrongGuesses++;
            printf("Wrong guess!\n");
        } else {
            printf("Correct guess!\n");
        }

        if (strcmp(selectedWord, guessedWord) == 0) {
            printf("\n🎉 Congratulations! You guessed the word: %s\n", selectedWord);
            break;
        }
    }

    if (wrongGuesses == MAX_TRIES) {
        drawHangman(wrongGuesses);
        printf("\n💀 Game Over! The correct word was: %s\n", selectedWord);

        char choice;
        printf("Do you want to retry? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            playGame(words, wordCount); // restart game
        } else {
            printf("Thanks for playing!\n");
        }
    }
}

int main() {
    char words[MAX_WORDS][MAX_LENGTH];
    int wordCount = 0;

    FILE *file = fopen("words.txt", "r");
    if (file == NULL) {
        printf("Error: Could not open words.txt\n");
        return 1;
    }

    // Read words from file
    while (fgets(words[wordCount], MAX_LENGTH, file) != NULL) {
        words[wordCount][strcspn(words[wordCount], "\n")] = 0; // remove newline
        wordCount++;
    }

    fclose(file);

    if (wordCount == 0) {
        printf("No words found in file.\n");
        return 1;
    }

    playGame(words, wordCount);

    return 0;
}