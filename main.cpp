#include <stdio.h>
#define TGBOT_COMPILE_DLL
#include <tgbot/tgbot.h>
#define SQLITECPP_COMPILE_DLL
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <ctime>
#include <unordered_set> // ��������� ������������ ���� ��� ������������� std::unordered_set

// ������� ���� ������ � ��������� �
SQLite::Database db("example.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
bool inputMode = false;
bool outputMode = false;
std::string links, name, surname,linkOfn;
void main() {
    TgBot::Bot bot("6775048544:AAFaQ-Q8iFalpZoNSzOEeKeGEYDFlEhCVQo");
    std::unordered_set<int> usedJokes; // ���������� std::unordered_set ��� �������� �������������� �������� �����

    // �������� �������, ���� �� ����������
    bot.getApi().deleteWebhook();


    // ������� /help
    bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
        printf("Received /help command\n");
    try {
        bot.getApi().sendMessage(message->chat->id, u8"��������� �������:\
                                                           \n/start - ������� ��� ������ ����.\
                                                           \n/help - ������� ��� ������ ���� ������.\
                                                           \n/joke - ������� ��� ��������� ��������� �����.\
                                                           \n/input - ������� ��� ����� ���������� � ��������.\
                                                           \n/output - ������� ��� ������ � ������ ���������� �� ���� ������.");
    }
    catch (const TgBot::TgException& e) {
        printf("Error sending message: %s\n", e.what());
    }
        });

    // ��������� ������� /start (�������������)
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        // ��������� ������
        printf("Received /start command\n");

        // �������� ������-���������� � ������� "����"
        TgBot::InlineKeyboardMarkup::Ptr inlineKeyboard(new TgBot::InlineKeyboardMarkup);
        std::vector<TgBot::InlineKeyboardButton::Ptr> row;

        // ������ "����"
        TgBot::InlineKeyboardButton::Ptr menuButton(new TgBot::InlineKeyboardButton);
        menuButton->text = u8"����";
        menuButton->callbackData = "menu";
        row.push_back(menuButton);

        // ���������� ������ ������ � ����������
        inlineKeyboard->inlineKeyboard.push_back(row);

        // �������� ��������� � ������-�����������
        try {
            bot.getApi().sendMessage(message->chat->id, u8"������! ������� ������ ���� ��� ��������� ���� �������.", false, 0, inlineKeyboard);

            //���������� ������������ � ���� ������ ��������
            SQLite::Statement query(db, "SELECT COUNT(*) FROM users WHERE idTg = :userId");
            query.bind(":userId", message->chat->id);
            if (query.executeStep()) {
                if (query.getColumn(0).getInt() <= 0) {
                    SQLite::Statement query(db, "INSERT INTO users (idTg) VALUES (?)");
                    query.bind(1, message->chat->id);
                    query.exec(); // ��������� ������ �� ������� ������
                }
            }
                
        }
        catch (const TgBot::TgException& e) {
            printf("Error sending message: %s\n", e.what());
        }
    });


    // ��������� ������� �� ������ "Start" � ������-����������
    bot.getEvents().onCallbackQuery([&bot, &usedJokes](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "menu") {
            printf("Button 'Menu' pressed\n");

            // �������� ������-���������� � ���������
            TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
            std::vector<TgBot::InlineKeyboardButton::Ptr> row;

            // ������ "/help"
            TgBot::InlineKeyboardButton::Ptr helpButton(new TgBot::InlineKeyboardButton);
            helpButton->text = "/help";
            helpButton->callbackData = "help";
            row.push_back(helpButton);

            // ������ "/joke"
            TgBot::InlineKeyboardButton::Ptr jokeButton(new TgBot::InlineKeyboardButton);
            jokeButton->text = "/joke";
            jokeButton->callbackData = "joke";
            row.push_back(jokeButton);

            //// ������ "/input"
            //TgBot::InlineKeyboardButton::Ptr inputButton(new TgBot::InlineKeyboardButton);
            //inputButton->text = "/input";
            //inputButton->callbackData = "input";
            //row.push_back(inputButton);

            //// ������ "/output"
            //TgBot::InlineKeyboardButton::Ptr outputButton(new TgBot::InlineKeyboardButton);
            //outputButton->text = "/output";
            //outputButton->callbackData = "output";
            //row.push_back(outputButton);

            // ���������� ������ ������ � ����������
            keyboard->inlineKeyboard.push_back(row);

            // ���������� ��������� � �����������
            try {
                bot.getApi().editMessageText(u8"�������� ���� �� ������� ����:", query->message->chat->id, query->message->messageId, "", "HTML", false, keyboard);
            }
            catch (const TgBot::TgException& e) {
                printf("Error editing message: %s\n", e.what());
            }
        }
        else if (query->data == "help") {
            try {
                bot.getApi().sendMessage(query->message->chat->id, u8"��������� �������:\
                                                           \n/start - ������� ��� ������ ����.\
                                                           \n/help - ������� ��� ������ ���� ������.\
                                                           \n/joke - ������� ��� ��������� ��������� �����.\
                                                           \n/input - ������� ��� ����� ���������� � ��������.\
                                                           \n/output - ������� ��� ������ � ������ ���������� �� ���� ������.");
            }
            catch (const TgBot::TgException& e) {
                printf("Error sending message: %s\n", e.what());
            }
        }
        else if (query->data == "joke") {
            // ������ �����
            const char* jokes[] = {
                u8"������ ������������ �� ����� �������? ������ ��� ��� ������� ����� �����.",
                u8"��� ���������� ������ ������? ����������.",
                u8"������ ����� �� ���������� ������ ��������? ������ ��� � ��� ������� ����� �������.",
                u8"������ ��������� ����� � �����? ������ ��� � ���� �����.",
                u8"������ � ����� ������� �� ������ ��������? ������ ��� �� ������, ��� ��� ����� ������.",
                u8"����� ��� ���� ���������� ������? ����-����-����.",
                u8"������ ������ �������� ��������? ������ ��� ��� ������ ���-�� ���������.",
                u8"������ ������� �� ����� ����� �� ������? ������ ��� ��� ���� �� ������ ������.",
                u8"��� ������ ���� ����� �������? ������, ��� ������ ������� �����."
            };

            int totalJokes = sizeof(jokes) / sizeof(jokes[0]); // ����� ���������� �����
            int randomIndex = -1;

            // ���� �� ������ ���������������� ������, ���������� �����
            do {
                randomIndex = std::rand() % totalJokes;
            } while (usedJokes.find(randomIndex) != usedJokes.end());

            // ��������� ������ � ������ ��������������
            usedJokes.insert(randomIndex);

            // ���� ��� ����� ������������, ������� ������ ��� ���������� �������������
            if (usedJokes.size() == totalJokes) {
                usedJokes.clear();
            }

            // �������� ������-���������� � �������� "���" � "����"
            TgBot::InlineKeyboardMarkup::Ptr inlineKeyboard(new TgBot::InlineKeyboardMarkup);
            std::vector<TgBot::InlineKeyboardButton::Ptr> row;

            // ������ "���"
            TgBot::InlineKeyboardButton::Ptr anotherJokeButton(new TgBot::InlineKeyboardButton);
            anotherJokeButton->text = u8"���";
            anotherJokeButton->callbackData = "joke";
            row.push_back(anotherJokeButton);

            // ������ "����"
            TgBot::InlineKeyboardButton::Ptr menuButton(new TgBot::InlineKeyboardButton);
            menuButton->text = u8"����";
            menuButton->callbackData = "menu";
            row.push_back(menuButton);

            // ���������� ������ ������ � ����������
            inlineKeyboard->inlineKeyboard.push_back(row);

            // �������� ��������� ����� � ������-�����������
            try {
                bot.getApi().sendMessage(query->message->chat->id, jokes[randomIndex], false, 0, inlineKeyboard);
            }
            catch (const TgBot::TgException& e) {
                printf("Error sending message: %s\n", e.what());
            }
        }
    });
    bot.getEvents().onCommand("input", [&bot](TgBot::Message::Ptr message) {
        printf("Received /input command\n");
        int chatId = message->chat->id;
        if (message->text == "/input") {
            inputMode = true;
            bot.getApi().sendMessage(chatId, u8"������� ������ �� ������������\
                                                \n/input <������>");
        }
        else if (inputMode) {
            if (links.empty()) {
                links = (message->text).erase(0, 7);
                linkOfn = "https://vk.com/";
                for (int i = 0; i < 15; i++) {
                    if (links[i] != linkOfn[i]) {
                        bot.getApi().sendMessage(chatId, u8"��������,�� ��� �� �� ������. ������������� ���� ������ � �������� /input ������");
                        inputMode = false;
                        break;
                    }
                }
                SQLite::Statement query(db, "SELECT COUNT(*) FROM person WHERE link = :userId");
                query.bind(":userId", links);
                if (query.executeStep()) {
                    if (query.getColumn(0).getInt() > 0) {
                        bot.getApi().sendMessage(chatId, u8"��������,�� ��� ������ �� ��� ��������� � ���� ������.");
                        inputMode = false;
                    }
                }
                if (inputMode)
                    bot.getApi().sendMessage(chatId, u8"������� ��� ��������\
                                                    \n/input <���>");
                else
                    links.clear();
            }   
            else if (name.empty()) {
                name = (message->text).erase(0, 7);
                bot.getApi().sendMessage(chatId, u8"������� ������� ��������\
                                                \n/input <�������>");
            }
            else {
                surname = (message->text).erase(0,7);
                SQLite::Statement query(db, "INSERT INTO person (link, name, surname) VALUES (?, ?, ?)");
                query.bind(1, links);
                links.clear();
                query.bind(2, name);
                name.clear();
                query.bind(3, surname);
                surname.clear();
                query.exec();
                inputMode = false;
                printf("Add person\n");
                bot.getApi().sendMessage(chatId, u8"������� �� ���� ����������!");
            }
        }
    });
    bot.getEvents().onCommand("output", [&bot](TgBot::Message::Ptr message) {
        printf("Received /output command\n");
        int chatId = message->chat->id;
        if (message->text == "/output") {
            outputMode = true;
            bot.getApi().sendMessage(chatId, u8"������� ������ �� ��������, � ������� �� ������ ������\
                                                \n/output <������>");
        }
        else if (outputMode) {
            links = (message->text).erase(0, 8);
            linkOfn = "https://vk.com/";
            for (int i = 0; i < 15; i++) {
                if (links[i] != linkOfn[i]) {
                    bot.getApi().sendMessage(chatId, u8"��������,�� ��� �� �� ������. ������������� ���� ������ � �������� /output ������");
                    outputMode = false;
                    break;
                }
            }
            SQLite::Statement query(db, "SELECT * FROM person WHERE link = :userId");
            query.bind(":userId", links);
            if (query.executeStep()) {
                // ��������� ���������� �� ������
                links = query.getColumn(0).getText();
                name = query.getColumn(1).getText();
                surname = query.getColumn(2).getText();
                bot.getApi().sendMessage(chatId, u8"����������:\
                                                \n������: " + links + u8"\
                                                \n���: " + name + u8"\
                                                \n�������: " + surname);
                links.clear();
                name.clear();
                surname.clear();
                outputMode = false;
            }
            else {
                bot.getApi().sendMessage(chatId, u8"���������� � ������������ ���");
                outputMode = false;
            }
        }
    });
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    }
    catch (TgBot::TgException& e) {
        printf("Error: %s\n", e.what());
    }
}