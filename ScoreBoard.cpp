#include "ScoreBoard.hpp"
#include <fstream>
#include <algorithm>
#include <regex>

ScoreBoard::ScoreBoard()
    : filePath("score.dat"), maxEntries(5)
{
  load();
}

void ScoreBoard::load() {
  std::ifstream file;
  std::string name;
  int score;
  file.open(filePath, std::ios::binary);

  while (file >> name >> score) {
    addEntry(std::regex_replace(name, std::regex("__WS__"), " "), score);
  }

  file.close();
}

void ScoreBoard::save() const {
  std::ofstream file;
  file.open(filePath, std::ios::binary | std::ios::out | std::ios::trunc);

  for(const auto& entry : entries) {
    file << std::regex_replace(entry.name, std::regex(" "), "__WS__") << " " << entry.score << std::endl;
  }

  file.close();
}

void ScoreBoard::addEntry(std::string name, int score) {
  if(entries.size() >= maxEntries) {
    entries.erase(std::prev(entries.end()));
  }

  entries.emplace(name, score);
}

bool ScoreBoard::worthInsert(int score) const {
  return entries.size() < maxEntries || score > entries.rbegin()->score;
}

const std::multiset<ScoreBoard::Entry>& ScoreBoard::getEntries() const {
  return entries;
}
