#ifndef BREAK_IT_SCOREBOARD_HPP
#define BREAK_IT_SCOREBOARD_HPP

#include <string>
#include <set>
#include <utility>

class ScoreBoard {
  public:
    struct Entry {
      std::string name;
      int score;

      Entry(std::string name, int score) : name(std::move(name)), score(score) {}

      bool operator<(const Entry& entry) const {
        return score > entry.score;
      }
    };

    ScoreBoard();

    void load();
    void save() const;
    void addEntry(std::string, int score);
    bool worthInsert(int score) const;

  private:
    std::string filePath;
    std::size_t maxEntries;
    std::set<Entry> entries;
};

#endif //BREAK_IT_SCOREBOARD_HPP
