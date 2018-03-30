#include "TitleState.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <curl/curl.h>
#include "../system/ResourceHolder.hpp"
#include "../gui/Button.hpp"
#include "../Config.hpp"
#include "../system/json.hpp"

using json = nlohmann::json;

size_t writeString(void *ptr, size_t size, size_t nmemb, std::string* data) {
  data->append((char*) ptr, size * nmemb);
  return size * nmemb;
}

size_t writeFile(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  size_t written = fwrite(ptr, size, nmemb, stream);
  return written;
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
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);

    curl_easy_perform(curl);
    auto j3 = json::parse(response_string);
    std::string tag = j3["tag_name"];
    if(tag.compare(GAME_VERSION_TAG) > 0) {
        auto assets = j3["assets"];
        for(auto& asset : assets) {
          std::string browser_download_url = asset["browser_download_url"];
          if (browser_download_url.find(std::string(PLATFORM_NAME) + ".zip") != std::string::npos) {
            FILE* patch_file = fopen("temp.zip", "wb");
            FILE* logfile = fopen("dump.txt", "wb");
            CURL* fileCurl = curl_easy_init();
            curl_easy_setopt(fileCurl, CURLOPT_URL, browser_download_url.c_str());
            curl_easy_setopt(fileCurl, CURLOPT_WRITEFUNCTION, writeFile);
            curl_easy_setopt(fileCurl, CURLOPT_WRITEDATA, patch_file);
            curl_easy_setopt(fileCurl, CURLOPT_USERAGENT, "curl/7.59.0");
            curl_easy_setopt(fileCurl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(fileCurl, CURLOPT_CAINFO, "curl-ca-bundle.crt");
            curl_easy_setopt(fileCurl, CURLOPT_VERBOSE, 1L);
            curl_easy_setopt(fileCurl, CURLOPT_STDERR, logfile);

            curl_easy_perform(fileCurl);
            curl_easy_cleanup(fileCurl);
            fclose(patch_file);
            fclose(logfile);
          }
        }

    }
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
