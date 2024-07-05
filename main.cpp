#include <stdio.h>
#define TGBOT_COMPILE_DLL
#include <tgbot/tgbot.h>
#define SQLITECPP_COMPILE_DLL
#include <SQLiteCpp/SQLiteCpp.h>
#include <cstdlib>
#include <ctime>
#include <unordered_set> // Добавляем заголовочный файл для использования std::unordered_set

// Создаем базу данных и открываем её
SQLite::Database db("example.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
bool inputMode = false;
bool outputMode = false;
std::string links, name, surname,linkOfn;
void main() {
    TgBot::Bot bot("6775048544:AAFaQ-Q8iFalpZoNSzOEeKeGEYDFlEhCVQo");
    std::unordered_set<int> usedJokes; // Используем std::unordered_set для хранения использованных индексов шуток

    // Удаление вебхука, если он установлен
    bot.getApi().deleteWebhook();


    // Команда /help
    bot.getEvents().onCommand("help", [&bot](TgBot::Message::Ptr message) {
        printf("Received /help command\n");
    try {
        bot.getApi().sendMessage(message->chat->id, u8"Доступные команды:\
                                                           \n/start - Команда для запуск бота.\
                                                           \n/help - Команда для вывода всех команд.\
                                                           \n/joke - Команда для получения случайной шутки.\
                                                           \n/input - Команда для ввода информации о человеке.\
                                                           \n/output - Команда для поиска и вывода информации из базы данных.");
    }
    catch (const TgBot::TgException& e) {
        printf("Error sending message: %s\n", e.what());
    }
        });

    // Обработка команды /start (инициализация)
    bot.getEvents().onCommand("start", [&bot](TgBot::Message::Ptr message) {
        // Добавляем данные
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

            //Запоминаем пользователя в базе данных ЕДИНОЖДЫ
            SQLite::Statement query(db, "SELECT COUNT(*) FROM users WHERE idTg = :userId");
            query.bind(":userId", message->chat->id);
            if (query.executeStep()) {
                if (query.getColumn(0).getInt() <= 0) {
                    SQLite::Statement query(db, "INSERT INTO users (idTg) VALUES (?)");
                    query.bind(1, message->chat->id);
                    query.exec(); // Выполняем запрос на вставку данных
                }
            }
                
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

            //// Кнопка "/input"
            //TgBot::InlineKeyboardButton::Ptr inputButton(new TgBot::InlineKeyboardButton);
            //inputButton->text = "/input";
            //inputButton->callbackData = "input";
            //row.push_back(inputButton);

            //// Кнопка "/output"
            //TgBot::InlineKeyboardButton::Ptr outputButton(new TgBot::InlineKeyboardButton);
            //outputButton->text = "/output";
            //outputButton->callbackData = "output";
            //row.push_back(outputButton);

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
                bot.getApi().sendMessage(query->message->chat->id, u8"Доступные команды:\
                                                           \n/start - Команда для запуск бота.\
                                                           \n/help - Команда для вывода всех команд.\
                                                           \n/joke - Команда для получения случайной шутки.\
                                                           \n/input - Команда для ввода информации о человеке.\
                                                           \n/output - Команда для поиска и вывода информации из базы данных.");
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
    bot.getEvents().onCommand("input", [&bot](TgBot::Message::Ptr message) {
        printf("Received /input command\n");
        int chatId = message->chat->id;
        if (message->text == "/input") {
            inputMode = true;
            bot.getApi().sendMessage(chatId, u8"Введите ссылку на пользователя\
                                                \n/input <ссылка>");
        }
        else if (inputMode) {
            if (links.empty()) {
                links = (message->text).erase(0, 7);
                linkOfn = "https://vk.com/";
                for (int i = 0; i < 15; i++) {
                    if (links[i] != linkOfn[i]) {
                        bot.getApi().sendMessage(chatId, u8"Извините,но это не ВК ссылка. Перепроверьте вашу ссылку и напишите /input заново");
                        inputMode = false;
                        break;
                    }
                }
                SQLite::Statement query(db, "SELECT COUNT(*) FROM person WHERE link = :userId");
                query.bind(":userId", links);
                if (query.executeStep()) {
                    if (query.getColumn(0).getInt() > 0) {
                        bot.getApi().sendMessage(chatId, u8"Извините,но эта ссылка вк уже добавлена в базу данных.");
                        inputMode = false;
                    }
                }
                if (inputMode)
                    bot.getApi().sendMessage(chatId, u8"Введите имя человека\
                                                    \n/input <имя>");
                else
                    links.clear();
            }   
            else if (name.empty()) {
                name = (message->text).erase(0, 7);
                bot.getApi().sendMessage(chatId, u8"Введите фамилию человека\
                                                \n/input <фамилия>");
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
                bot.getApi().sendMessage(chatId, u8"Спасибо за вашу информацию!");
            }
        }
    });
    bot.getEvents().onCommand("output", [&bot](TgBot::Message::Ptr message) {
        printf("Received /output command\n");
        int chatId = message->chat->id;
        if (message->text == "/output") {
            outputMode = true;
            bot.getApi().sendMessage(chatId, u8"Введите ссылку на человека, о котором вы хотите узнать\
                                                \n/output <ссылка>");
        }
        else if (outputMode) {
            links = (message->text).erase(0, 8);
            linkOfn = "https://vk.com/";
            for (int i = 0; i < 15; i++) {
                if (links[i] != linkOfn[i]) {
                    bot.getApi().sendMessage(chatId, u8"Извините,но это не ВК ссылка. Перепроверьте вашу ссылку и напишите /output заново");
                    outputMode = false;
                    break;
                }
            }
            SQLite::Statement query(db, "SELECT * FROM person WHERE link = :userId");
            query.bind(":userId", links);
            if (query.executeStep()) {
                // Получение информации из строки
                links = query.getColumn(0).getText();
                name = query.getColumn(1).getText();
                surname = query.getColumn(2).getText();
                bot.getApi().sendMessage(chatId, u8"Информация:\
                                                \nСсылка: " + links + u8"\
                                                \nИмя: " + name + u8"\
                                                \nФамилия: " + surname);
                links.clear();
                name.clear();
                surname.clear();
                outputMode = false;
            }
            else {
                bot.getApi().sendMessage(chatId, u8"Информации о пользователе нет");
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