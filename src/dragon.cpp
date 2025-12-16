#include "../include/dragon.h"
#include "../include/knight.h"
#include "../include/princess.h"

Dragon::Dragon(std::string name, int x, int y) : NPC(DragonType, name, x, y) {}
Dragon::Dragon(std::istream &is) : NPC(DragonType, is) {}

void Dragon::print()
{
    std::cout << *this;
}

void Dragon::print(std::ofstream& os) const{
    os << "Dragon " << name << " (" << x << ", " << y << ")\n";
}

bool Dragon::fight(std::shared_ptr<Dragon> other) 
{
    fight_notify(other, false);
    return false;
}

bool Dragon::fight(std::shared_ptr<Knight> other) 
{
    fight_notify(other, false);
    return false;
}

bool Dragon::fight(std::shared_ptr<Princess> other) 
{
    fight_notify(other, true);
    return true;
}

bool Dragon::accept(std::shared_ptr<NPC> visitor){
    return visitor->fight(std::shared_ptr<Dragon>(this,[](Dragon*){}));
}

void Dragon::save(std::ostream &os) 
{
    os << DragonType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Dragon &dragon)
{
    os << "dragon: " << *static_cast<NPC *>(&dragon) << std::endl;
    return os;
}