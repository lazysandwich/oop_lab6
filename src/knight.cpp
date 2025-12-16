#include "../include/dragon.h"
#include "../include/knight.h"
#include "../include/princess.h"

Knight::Knight(std::string name, int x, int y) : NPC(KnightType, name, x, y) {}
Knight::Knight(std::istream &is) : NPC(KnightType, is) {}

void Knight::print()
{
    std::cout << *this;
}

void Knight::print(std::ofstream& os) const{
    os << "Knight " << name << " (" << x << ", " << y << ")\n";
}

bool Knight::fight(std::shared_ptr<Dragon> other) 
{
    fight_notify(other, true);
    return true;
}

bool Knight::fight(std::shared_ptr<Knight> other) 
{
    fight_notify(other, false);
    return false;
}

bool Knight::fight(std::shared_ptr<Princess> other) 
{
    fight_notify(other, true);
    return true;
}

bool Knight::accept(std::shared_ptr<NPC> visitor){
    return visitor->fight(std::shared_ptr<Knight>(this,[](Knight*){}));
}

void Knight::save(std::ostream &os) 
{
    os << KnightType << std::endl;
    NPC::save(os);
}

std::ostream &operator<<(std::ostream &os, Knight &Knight)
{
    os << "Knight: " << *static_cast<NPC *>(&Knight) << std::endl;
    return os;
}