#include <stdio.h>
#define TGBOT_COMPILE_DLL
#include <tgbot/tgbot.h>
#include <cstdlib>
#include <ctime>
#include <unordered_set> // ��������� ������������ ���� ��� ������������� std::unordered_set

int main() {
    TgBot::Bot bot("Token");
    std::unordered_set<int> usedJokes; // ���������� std::unordered_set ��� �������� �������������� �������� �����

    // �������� �������, ���� �� ����������
    bot.getApi().deleteWebhook();

    // ��������� ������� /start (�������������)
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
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
        }
        catch (const TgBot::TgException& e) {
            printf("Error sending message: %s\n", e.what());
        }
        });

    // ������� /help
    bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
        printf("Received /help command\n");
        try {
            bot.getApi().sendMessage(message->chat->id, "Available commands:\n/start - Start the bot\n/help - Show help\n/joke - Get a random joke");
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
                bot.getApi().sendMessage(query->message->chat->id, u8"��������� �������:\n/start - ��������� ����\n/help - �������� ��� �������\n/joke - �������� ��������� �����");
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

    return 0;
}
