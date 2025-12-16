#include "../include/dragon.h"
#include "../include/knight.h"
#include "../include/princess.h"

Princess::Princess(std::string name, int x, int y) : NPC(PrincessType, name, x, y) {}
Princess::Princess(std::istream &is) : NPC(PrincessType, is) {}

void Princess::print()
{
    std::cout << *this;
}

void Princess::print(std::ofstream& os) const{
    os << "Princess " << name << " (" << x << ", " << y << ")\n";
}

bool Princess::fight(std::shared_ptr<Dragon> other) 
{
    fight_notify(other, false);
    return false;
}

bool Princess::fight(std::shared_ptr<Knight> other) 
{
    fight_notify(other, false);
    return false;
}

bool Princess::fight(std::shared_ptr<Princess> other) 
{
    fight_notify(other, false);
    return false;
}

bool Princess::accept(std::shared_ptr<NPC> visitor){
    return visitor->fight(std::shared_ptr<Princess>(this,[](Princess*){}));
}


void Princess::save(std::ostream &os) 
{
    os << PrincessType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Princess &Princess)
{
    os << "Princess: " << *static_cast<NPC *>(&Princess) << std::endl;
    return os;
}