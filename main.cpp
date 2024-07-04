#include <stdio.h>
#define TGBOT_COMPILE_DLL
#include <tgbot/tgbot.h>
#include <cstdlib>
#include <ctime>
#include <unordered_set> // Добавляем заголовочный файл для использования std::unordered_set

int main() {
    TgBot::Bot bot("Token");
    std::unordered_set<int> usedJokes; // Используем std::unordered_set для хранения использованных индексов шуток

    // Удаление вебхука, если он установлен
    bot.getApi().deleteWebhook();

    // Обработка команды /start (инициализация)
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        printf("Received /start command\n");

        // Создание инлайн-клавиатуры с кнопкой "Меню"
        TgBot::InlineKeyboardMarkup::Ptr inlineKeyboard(new TgBot::InlineKeyboardMarkup);
        std::vector<TgBot::InlineKeyboardButton::Ptr> row;

        // Кнопка "Меню"
        TgBot::InlineKeyboardButton::Ptr menuButton(new TgBot::InlineKeyboardButton);
        menuButton->text = u8"Меню";
        menuButton->callbackData = "menu";
        row.push_back(menuButton);

        // Добавление строки кнопок в клавиатуру
        inlineKeyboard->inlineKeyboard.push_back(row);

        // Отправка сообщения с инлайн-клавиатурой
        try {
            bot.getApi().sendMessage(message->chat->id, u8"Привет! Нажмите кнопку ниже для просмотра всех функций.", false, 0, inlineKeyboard);
        }
        catch (const TgBot::TgException& e) {
            printf("Error sending message: %s\n", e.what());
        }
        });

    // Команда /help
    bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
        printf("Received /help command\n");
        try {
            bot.getApi().sendMessage(message->chat->id, "Available commands:\n/start - Start the bot\n/help - Show help\n/joke - Get a random joke");
        }
        catch (const TgBot::TgException& e) {
            printf("Error sending message: %s\n", e.what());
        }
        });

    // Обработка нажатия на кнопку "Start" в инлайн-клавиатуре
    bot.getEvents().onCallbackQuery([&bot, &usedJokes](TgBot::CallbackQuery::Ptr query) {
        if (query->data == "menu") {
            printf("Button 'Menu' pressed\n");

            // Создание инлайн-клавиатуры с командами
            TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
            std::vector<TgBot::InlineKeyboardButton::Ptr> row;

            // Кнопка "/help"
            TgBot::InlineKeyboardButton::Ptr helpButton(new TgBot::InlineKeyboardButton);
            helpButton->text = "/help";
            helpButton->callbackData = "help";
            row.push_back(helpButton);

            // Кнопка "/joke"
            TgBot::InlineKeyboardButton::Ptr jokeButton(new TgBot::InlineKeyboardButton);
            jokeButton->text = "/joke";
            jokeButton->callbackData = "joke";
            row.push_back(jokeButton);

            // Добавление строки кнопок в клавиатуру
            keyboard->inlineKeyboard.push_back(row);

            // Обновление сообщения с клавиатурой
            try {
                bot.getApi().editMessageText(u8"Выберите одну из функций ниже:", query->message->chat->id, query->message->messageId, "", "HTML", false, keyboard);
            }
            catch (const TgBot::TgException& e) {
                printf("Error editing message: %s\n", e.what());
            }
        }
        else if (query->data == "help") {
            try {
                bot.getApi().sendMessage(query->message->chat->id, u8"Доступные команды:\n/start - Запустить бота\n/help - Показать все функции\n/joke - Получить случайную шутку");
            }
            catch (const TgBot::TgException& e) {
                printf("Error sending message: %s\n", e.what());
            }
        }
        else if (query->data == "joke") {
            // Массив шуток
            const char* jokes[] = {
                u8"Почему программисты не любят природу? Потому что там слишком много багов.",
                u8"Как называется спящая корова? Сныкоровка.",
                u8"Почему книга по математике всегда грустная? Потому что у нее слишком много проблем.",
                u8"Почему компьютер пошел к врачу? Потому что у него вирус.",
                u8"Почему у слона никогда не бывает подружки? Потому что он боится, что она будет мышкой.",
                u8"Какой вид пчел производит молоко? Бубу-бубу-бубу.",
                u8"Почему нельзя доверять лестнице? Потому что она всегда что-то замышляет.",
                u8"Почему пианист не нашел ключи от машины? Потому что они были на другой октаве.",
                u8"Что сказал один океан другому? Ничего, они просто махнули рукой."
            };

            int totalJokes = sizeof(jokes) / sizeof(jokes[0]); // Общее количество шуток
            int randomIndex = -1;

            // Пока не найдем неиспользованный индекс, генерируем новый
            do {
                randomIndex = std::rand() % totalJokes;
            } while (usedJokes.find(randomIndex) != usedJokes.end());

            // Добавляем индекс в список использованных
            usedJokes.insert(randomIndex);

            // Если все шутки использованы, очищаем список для повторного использования
            if (usedJokes.size() == totalJokes) {
                usedJokes.clear();
            }

            // Создание инлайн-клавиатуры с кнопками "Еще" и "Меню"
            TgBot::InlineKeyboardMarkup::Ptr inlineKeyboard(new TgBot::InlineKeyboardMarkup);
            std::vector<TgBot::InlineKeyboardButton::Ptr> row;

            // Кнопка "Еще"
            TgBot::InlineKeyboardButton::Ptr anotherJokeButton(new TgBot::InlineKeyboardButton);
            anotherJokeButton->text = u8"Еще";
            anotherJokeButton->callbackData = "joke";
            row.push_back(anotherJokeButton);

            // Кнопка "Меню"
            TgBot::InlineKeyboardButton::Ptr menuButton(new TgBot::InlineKeyboardButton);
            menuButton->text = u8"Меню";
            menuButton->callbackData = "menu";
            row.push_back(menuButton);

            // Добавление строки кнопок в клавиатуру
            inlineKeyboard->inlineKeyboard.push_back(row);

            // Отправка случайной шутки с инлайн-клавиатурой
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
