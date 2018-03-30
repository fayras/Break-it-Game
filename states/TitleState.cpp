#include "TitleState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <curl/curl.h>
#include "../system/ResourceHolder.hpp"
#include "../gui/Button.hpp"
#include "../Config.hpp"

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
  data->append((char*) ptr, size * nmemb);
  return size * nmemb;
}

TitleState::TitleState(StateStack &stack, State::Context context)
    : State(stack, context),
      guiContainer(),
      background(context.textures->get(Textures::TITLE))
{
  auto btPlay = std::make_shared<gui::Button>(context);
  btPlay->setPosition(0, 0);
  btPlay->setText("Neues Spiel");
  btPlay->setCallback([this] () {
    this->context.saveData->set("player_level", 0);
    this->context.saveData->set("player_score", 0);
    requestStackPop();
    requestStackPush(States::ID::GAME);
  });

  auto btSettings = std::make_shared<gui::Button>(context);
  btSettings->setPosition(0, 60);
  btSettings->setText("Einstellungen");
  btSettings->setCallback([this] () {
    requestStackPush(States::ID::SETTINGS);
  });

  auto btExit = std::make_shared<gui::Button>(context);
  btExit->setPosition(0, 120);
  btExit->setText("Beenden");
  btExit->setShortcut(sf::Keyboard::Escape);
  btExit->setCallback([this] () {
    requestStackPop();
  });

  guiContainer.move(120, 450);
  guiContainer.pack(btPlay);
  guiContainer.pack(btSettings);
  guiContainer.pack(btExit);

  // Wenn es ein Savegame gibt
  if(context.saveData->exists()) {
    auto btContinue = std::make_shared<gui::Button>(context);
    btContinue->setPosition(0, -60);
    btContinue->setText("Fortsetzen");
    btContinue->setCallback([this]() {
      requestStackPop();
      requestStackPush(States::ID::GAME);
    });
    guiContainer.pack(btContinue);
  }
  // }
  CURL* curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, GITHUB_RELEASE_PATH);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "curl/7.59.0");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_CAINFO, "curl-ca-bundle.crt");

    std::string response_string;
    std::string header_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Accept: application/json");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "charsets: utf-8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    char* url;
    long response_code;
    double elapsed;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &elapsed);
    curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &url);

    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    curl = nullptr;
  }
}

void TitleState::draw() {
  sf::RenderWindow& window = *getContext().window;
  window.setView(window.getDefaultView());
  window.draw(background);
  window.draw(guiContainer);
}

bool TitleState::update(sf::Time dt) {
  guiContainer.update(dt);
  return true;
}

bool TitleState::handleEvent(const sf::Event &event) {
  guiContainer.handleEvent(event);
  return true;
}
