#include "ScoreBoard.hpp"
#include <fstream>

ScoreBoard::ScoreBoard()
    : filePath("saves/score.dat"), maxEntries(5)
{}

void ScoreBoard::load() {
  std::ifstream file;
  char name[3];
  int score;
  file.open(filePath, std::ios::binary);

  while (file >> name >> score) {
    addEntry(name, score);
  }

  file.close();
}

void ScoreBoard::save() const {
  std::ofstream file;
  file.open(filePath, std::ios::binary | std::ios::out);

  for(const auto& entry : entries) {
    file << entry.name << entry.score << "\n";
  }

  file.close();
}

void ScoreBoard::addEntry(char *name, int score) {
  if(entries.size() >= maxEntries) {
    entries.erase(std::prev(entries.end()));
  }

  entries.emplace(name, score);
}

void ScoreBoard::newEntry(int score) {

}

bool ScoreBoard::worthInsert(int score) const {
  return entries.size() < maxEntries || score > entries.rbegin()->score;
}
