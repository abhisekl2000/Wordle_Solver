#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

class WordleSolver{

  private:
    std::vector<std::string> ignoreWords; //hold letters that user whishes to ignore when searching for potential words
    std::vector<std::string> board;       //represent the board state


  public:
    //displays the 5x1 grid
    void display(std::vector<std::string> currentBoard){
      std::cout << "[";
      for(int i=0; i<currentBoard.size(); i++){
        std::cout << currentBoard[i];
        if(i < currentBoard.size()-1){
          std::cout << ", ";
        }
      }
      std::cout << "]";
    }

    //clears and initializes the board with underscores
    void resetBoard(){
      board.clear();
      for(int i=0; i<5; i++){
        board.push_back("_");
      }
    }
    
    std::vector<std::string> split(std::string s, char delimiter){

      std::vector<std::string> ignoreWords;
      std::stringstream ss(s);
      std::string token;
      while(std::getline(ss, token, delimiter)){
        ignoreWords.push_back(token);
      }

      return ignoreWords;
    }

    std::vector<std::string> remover(std::vector<std::string> bigList, const std::string ignore){
    std::vector<std::string> newList;
    

    for(int i=0; i<bigList.size(); i++){
      bool flag = true;
      std::string word = bigList[i];
      for(int j=0; j<ignore.length(); j++){
        if(word.find(ignore[j]) != std::string::npos){  // not not found = found
          flag = false;
          break;
        }
      }

      if(flag){
        newList.push_back(word);
      }
    }

    return newList;
  }

    void play(){
      std::string prompt = "Your current cell is marked with a '=' .\n * Enter a space if you want to skip to the next cell.\n * Enter '1' to move one cell back and make changes.\n * Enter '0' to exit the board editing process.";
      std::cout << prompt << std::endl;
      resetBoard();
      initializeBoard();

      while(true){    //run the wordle solver as long as it takes to guess the word
        std::vector<std::string> rows = letterWords();

        //ask user to provide any letters to ignroe
        std::string ignore = getInput("Type in the letters to ignore (if any): ");
      
        // convert string into vector of words. Ex: a b c becomes {"a", "b", "c"}
        std::istringstream iss(ignore);

        //beginning of the stream, end of the stream
        ignoreWords = std::vector<std::string>((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
      

        if(!ignoreWords.empty() && !ignoreWords[0].empty()){
          rows = remover(rows, ignoreWords[0]);
        }

        //after removing words with ignore letters, we match the lost of words(rows) against state of the board
        std::vector<std::string> results = matcher(rows, board);

        results = mustHave(results);

        std::cout << "Here are the results of the matches: " <<std::endl;

        if(results.empty()){
          std::cout << "[]" <<std::endl;
        }
        else{
          std::cout << "[";
          if(results.size() <= 10){
            for(size_t i=0; i<results.size(); i++){
              std::cout << results[i];
              if(i<results.size()-1){
                std::cout << ", ";
              }

            }
          }else{ //if more than 10, print only 10
            for(size_t i=0; i<10; i++){
              std::cout << results[i];
              if(i<9){
                std::cout << ", ";
              }
            }
          }

          std::cout << "]" <<std::endl;
          
        }

        //give user ability to continue, quit, or restart
        std::string nextMove = getInput("Do you want to continue (c), reset(r), exit(e)? ");

        if(nextMove == "e"){
          break;
        }
        else if(nextMove == "r"){
          resetBoard();
          initializeBoard();
        }
        else if(nextMove == "c"){
          initializeBoard();
        }


      }

      
      
    }

   


    //get input string from the terminal and return that string
    std::string getInput(std::string prompt){
      std::cout << prompt << " ";
      std::string input;
      std::getline(std::cin, input);
      return input;
    }

    //allow user to interact with the board cell-by-cell
    int userEntry(int location){
      std::string initialState = board[location];
      board[location] = "=";
      display(board);

      std::string userInput = getInput("");

      if(userInput == " " || userInput.empty()){
        board[location] = initialState;
        return location+1;
      }
      else if(userInput == "0"){
        board[location] = initialState;
        return 5;
      }
      else if(userInput == "1"){
        board[location] = "_";
        return std::max(location-1, 0);
      }
      else{
        board[location] = userInput;
        return location+1;
      }
    }

    void initializeBoard(){
      int i=0;

      while(i<5){
        i=userEntry(i); //clever way to run the userEntry function continuously
      }

      display(board);
      std::cout << std::endl;

    }

    std::vector<std::string> letterWords(){
      std::vector<std::string> rows;
      std::fstream file("Wordle_Solver/words.txt");
      if(file.is_open()){
        std::string line;
        while(std::getline(file, line)){
          rows.push_back(line);
        }

        file.close();
      }
      else{
        std::cerr << "Unable to open file.\n";
      }

      return rows;
    }

  std::vector<std::string> matcher(std::vector<std::string> bigList, std::vector<std::string> target){
    std::vector<std::string> results;

    for(int i=0; i<bigList.size(); i++){
      std::string word = bigList[i];
      bool isMatch = true;
      for(int j=0; j<5; j++){
        if(target[j] != "_" && target[j] != std::string(1,word[j])){  //known letters that must match otherwise discard those words
          isMatch = false;
          break;
        }
      }
      if(isMatch){
        results.push_back(word);
      }
    }

    return results;
  }

  std::vector<std::string> mustHave(std::vector<std::string> bigList){
    std::string must = getInput("Type in letters to include but not sure of location: ");
    std::vector<std::string> newList;

    for(const auto& word : bigList){
      bool flag= true;
      for(char ch : must){
        if(word.find(ch) == std::string::npos){ //if character is not found
          flag = false;
          break;
        }
      }

      if(flag){
        newList.push_back(word);
      }
    }
    return newList;
  }



};

int main(){
  WordleSolver solver;
  solver.play();

  return 0;
}