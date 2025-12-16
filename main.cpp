#include "include/NPC.h"
#include "include/dragon.h"
#include "include/knight.h"
#include "include/princess.h"
#include "src/NPC.cpp"
#include "src/dragon.cpp"
#include "src/knight.cpp"
#include "src/princess.cpp"

#include <algorithm>
#include <memory>
#include <fstream>
#include <iostream>

class TextObserver : public IFightObserver
{
private:
    TextObserver(){};

public:
    static std::shared_ptr<IFightObserver> get()
    {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (win)
        {
            std::cout << std::endl
                      << "Murder --------" << std::endl;
            attacker->print();
            defender->print();
        }
    }
};

class FileObserver : public IFightObserver {
public:
    FileObserver(const std::string& filename) : log(filename, std::ios::app) {}

    static std::shared_ptr<FileObserver> get() {
        static std::shared_ptr<FileObserver> instance =
            std::make_shared<FileObserver>("log.txt");
        return instance;
    }

    void on_fight(const std::shared_ptr<NPC> attacker,
                  const std::shared_ptr<NPC> defender,
                  bool win) override {
        if (win) {
            log << "Убийца: ";
            attacker->print(log);
            defender->print(log);
            log << std::endl;
        }
    }

private:
    std::ofstream log;
};

std::shared_ptr<NPC> factory(std::istream &is)
{
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type)
    {
        switch (type)
        {
        case DragonType:
            result = std::make_shared<Dragon>(is);
            break;
        case KnightType:
            result = std::make_shared<Knight>(is);
            break;
        case PrincessType:
            result = std::make_shared<Princess>(is);
            break;
        }
    }
    else
        std::cerr << "unexpected NPC type:" << type << std::endl;

    if (result)
        result->subscribe(TextObserver::get());

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, std::string name, int x, int y)
{
    std::cout << "Type:" << (int)type << std::endl;
    std::shared_ptr<NPC> result;
    switch (type)
    {
    case DragonType:
        result = std::make_shared<Dragon>(name, x, y);
        break;
    case KnightType:
        result = std::make_shared<Knight>(name, x, y);
        break;
    case PrincessType:
        result = std::make_shared<Princess>(name, x, y);
        break;
    default:
        break;
    }
    if (result) {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

void save(const set_t &array, const std::string &filename)
{
    std::ofstream fs(filename);
    fs << array.size() << std::endl;
    for (auto &n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load(const std::string &filename)
{
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open())
    {
        int count;
        is >> count;
        for (int i = 0; i < count; ++i)
            result.insert(factory(is));
        is.close();
    }
    else
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
    return result;
}

std::ostream &operator<<(std::ostream &os, const set_t &array)
{
    for (auto &n : array)
        n->print();
    return os;
}

set_t fight(const set_t &array, size_t distance)
{
    set_t dead_list;

    for (const auto &attacker : array)
        for (const auto &defender : array)
            if ((attacker != defender) && 
                (attacker->is_close(defender, distance))) {
                bool success = defender->accept(attacker);
                if (success)
                    dead_list.insert(defender);
            }

    return dead_list;
}

int main()
{   
    int task = 0;
    set_t array;
    do {
        std::cout << "Команды:" << std::endl;
        std::cout << "1 - ввод NPC" << std::endl;
        std::cout << "2 - сохранить NPC в файл" << std::endl;
        std::cout << "3 - загрузка NPC из файла" << std::endl;
        std::cout << "4 - начать битву" << std::endl;
        std::cout << "5 - выход" << std::endl;
        std::cin >> task;
        if (task == 1) {
            std::cout << "введите NPC(тип, имя, координаты x и y) ";
            std::string npc_type, npc_name;
            int x_, y_;
            std::cin >> npc_type >> npc_name >> x_ >> y_;
            x_ = std::clamp(x_, 0, 500);
            y_ = std::clamp(y_, 0, 500);
            if (npc_type == "princess")
                array.insert(factory(NpcType(3), npc_name, x_, y_));
            else if (npc_type == "knight")
                array.insert(factory(NpcType(2), npc_name, x_, y_));
            else if (npc_type == "dragon")
                array.insert(factory(NpcType(1), npc_name, x_, y_));
            else
                std::cout << "Неверный тип NPC\n";
        }
        else if (task == 3) {
            std::cout << "Загрузка ..." << std::endl;
            array = load("npc.txt");
        }
        else if (task == 2) {
            std::cout << "Сохранение ..." << std::endl;
            save(array, "npc.txt");
        }
        else if (task == 4) {
            std::cout << "Битва ..." << std::endl;
            for (size_t distance = 20; (distance <= 100) && !array.empty(); distance += 10) {
                auto dead_list = fight(array, distance);
                for (auto& dead : dead_list)
                    array.erase(dead);
            }
        }
        else if(task > 5) {
            std::cout << "Неверный задача" << std::endl;
        }
    } while (task != 5);

    return 0;
}