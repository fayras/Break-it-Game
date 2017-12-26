#ifndef BREAK_IT_SCOREBOARD_HPP
#define BREAK_IT_SCOREBOARD_HPP

#include <string>
#include <set>

class ScoreBoard {
  public:
    struct Entry {
      char name[3];
      int score;

      bool operator<(const Entry& e1, const Entry& e2) {
        return e1.score > e2.score;
      }
    };

    ScoreBoard();

    void load();
    void save() const;
    void addEntry(char name[3], int score);
    void newEntry(int score);
    bool worthInsert(int score) const;

  private:
    std::string filePath;
    std::size_t maxEntries;
    std::set<Entry> entries;
};

#endif //BREAK_IT_SCOREBOARD_HPP
